#include "spi.h"
#include "fpga.h"
#include "flash_pgm.h"
#include "dump.h"
#include "command.h"
#include "ota.h"
#include "fpga_ota.h"
#include "sna.h"
#include "res.h"
#include "strtoint.h"
#include "netcomms.h"
#include "spectcmd.h"
#include "wifi.h"
#include "led.h"
#include "nvs.h"
#include "gpio.h"
#include "interfacez_resources.h"
#include "spectint.h"
#include "videostreamer.h"
#include "netcmd.h"
#include "sdcard.h"
#include "tapeplayer.h"
#include "rom.h"
#include "usbh.h"
#include "usb_ll.h"
#include <esp32/rom/uart.h>
#include "keyboard.h"
#include "webserver.h"
#include "config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "devmap.h"
#include "audio.h"
#include "console.h"
#include "version.h"
#include "buttons.h"
#include "vga.h"

static int8_t videomode = 0;

uint32_t loglevel;

volatile int restart_requested = 0;

#if 0
static void start_capture2()
{
    // Stop, clear
    fpga__set_clear_flags( FPGA_FLAG_CAPCLR, FPGA_FLAG_CAPRUN);
    fpga__set_flags( FPGA_FLAG_CAPRUN);
}

static void start_capture()
{
    // Reset spectrum, stop capture
    fpga__set_clear_flags( FPGA_FLAG_RSTSPECT, FPGA_FLAG_CAPRUN );
    // Reset fifo, clear capture
    fpga__set_flags( FPGA_FLAG_CAPCLR | FPGA_FLAG_RSTFIFO);

    // Remove resets, start capture
    fpga__set_clear_flags( FPGA_FLAG_CAPRUN | FPGA_FLAG_ENABLE_INTERRUPT , FPGA_FLAG_RSTFIFO| FPGA_FLAG_RSTSPECT| FPGA_FLAG_CAPCLR );

}
#endif


void request_restart()
{
    restart_requested = 1;
}

static void io0_long_press()
{
    char romname[128];

    if (nvs__fetch_str("io0rom", romname, sizeof(romname), "/spiffs/DiagROMv47.rom")<0) {
        ESP_LOGE(TAG, "Cannot fetch rom for IO0");
        return ;
    }
    ESP_LOGI(TAG, "Loading ROM %s", romname);

    int f = rom__load_custom_from_file(romname);
    if (f<0) {
        ESP_LOGE(TAG, "Cannot load ROM %s", romname);
        return;
    }

    ESP_LOGI(TAG, "Resetting to custom ROM");
    if (fpga__reset_to_custom_rom(false)<0) {
        ESP_LOGE(TAG, "Cannot reset");
    }
}

static void event_button_switch(button_event_type_e type)
{
    if (type==BUTTON_RELEASED || type==BUTTON_LONG_RELEASED) {
        ESP_LOGI(TAG, "Requesting NMI!");
        fpga__set_trigger(FPGA_FLAG_TRIG_FORCENMI_ON);
    }
}

static void event_button_io0(button_event_type_e type)
{
    if (type==BUTTON_RELEASED || type==BUTTON_LONG_RELEASED) {
        ESP_LOGI(TAG, "IO0 released");
    }
    if (type==BUTTON_PRESSED) {
        ESP_LOGI(TAG, "IO0 pressed");
    }
    if (type==BUTTON_LONG_PRESSED) {
        ESP_LOGI(TAG, "IO0 long pressed");
        io0_long_press();
    }
}

static void process_buttons()
{
    button_event_t event;
    if (buttons__getevent(&event, false)==0) {
        switch (event.button) {
        case BUTTON_SWITCH:
            event_button_switch(event.type);
            break;
        case BUTTON_IO0:
            event_button_io0(event.type);
        }
    }
}

void app_main()
{
    int lstatus = 0;
    int do_restart = 0;

    gpio__init();
    led__set(LED1, 1);
    led__set(LED2, 0);
    spi__init_bus();
    sdcard__init();
    nvs__init();
    wifi__init();


    if (fpga__init()<0) {
        ESP_LOGE(TAG, "Cannot proceed without FPGA!");
        while (1) {
            led__set(LED1, 1);
            vTaskDelay(100 / portTICK_RATE_MS);
            led__set(LED1, 0);
            vTaskDelay(100 / portTICK_RATE_MS);

        }
    }

    spectint__init();

    resource__init();

    resource__register( 0x00, &versionresource);
    resource__register( 0x02, &statusresource.r);
    resource__register( 0x03, &directoryresource.r);
    resource__register( 0x04, &wificonfigresource.r);
    resource__register( 0x05, &aplistresource.r);

    videomodeconfigresource.valptr = &videomode;

    resource__register( 0x06, &videomodeconfigresource.r);
    resource__register( 0x10, &opstatusresource.r);

    if (rom__load_from_flash()!=0) {
        ESP_LOGW(TAG,"Cannot load ROM from flash, continuing with no ROM");
    }

    config__init();
    spectcmd__init();
    videostreamer__init();
    netcmd__init();
    tapeplayer__init();
    devmap__init();
    usbh__init();
    keyboard__init();
    audio__init();
    webserver__init();
    vga__init();
    buttons__init();

    ESP_LOGI(TAG, "InterfaceZ version: %s %s", version, gitversion);
    ESP_LOGI(TAG, "  built %s", builddate);

    // Start capture
    //start_capture();
    unsigned iter = 0;
    while(1) {
        uint8_t c;
        led__set(LED1, !!(lstatus&0x4));
        lstatus++;
        /*int sw = gpio_get_level(PIN_NUM_SWITCH);
        if (sw==0 && lastsw==1) {
            ESP_LOGI(TAG, "Start capture");
            start_capture2();
        }
        lastsw = sw;
        */
        if (restart_requested) {
            do_restart = 20;
            restart_requested = 0;
        }

        vTaskDelay(100 / portTICK_RATE_MS);

        if (do_restart==1) {
            esp_restart();
        } else if (do_restart>0) {
            printf("Restart tick %d\n", do_restart);
            do_restart--;
        }


        fpga__get_status();
        iter++;
        if (iter==50) {
            // 5-second mark.
            //ESP_LOGI(TAG,"Interrupts: %d", videostreamer__getinterrupts());
            iter=0;
        }
        STATUS s;
        do {
            s = uart_rx_one_char(&c);
            if (s==OK) {
                console__char(c);
            }
        } while (s==OK);
        // Process buttons
        process_buttons();
    }
}
