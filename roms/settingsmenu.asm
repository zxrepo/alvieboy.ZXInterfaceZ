SETTINGS__REDRAW:
        ;LD	HL, SETTINGS_MENU
        LD	D, 8
        CALL	MENU__INIT
        JP 	MENU__DRAW  	; TAILCALL

SETTINGS__KEYHANDLER:
	;LD	HL, SETTINGS_MENU
	
        CP	$26 	; A key
        JR	NZ, _n1
        JP	MENU__CHOOSENEXT
_n1:
        CP	$25     ; Q key
        JR	NZ, _n2
        JP	MENU__CHOOSEPREV
_n2:
        CP	$21     ; ENTER key
        JP	Z, MENU__ACTIVATE
_n3:
	; Check for cancel
       	LD	DE,(CURKEY) ; Don't think is needed.
        LD	A, D
        CP	$27
        JR	NZ, _n4
        LD	A, E	
        CP	$20
        JR	Z, EXITSETTINGS
_n4:    RET
        
EXITSETTINGS:
	JP	WIDGET__CLOSE

SETTINGSMENU__HANDLEKEY:
	LD	HL, SETTINGS_MENU
	
        CP	$26 	; A key
        JR	NZ, _n1
        JP	MENU__CHOOSENEXT
_n1:
        CP	$25     ; Q key
        JR	NZ, _n2
        JP	MENU__CHOOSEPREV
_n2:
        CP	$21     ; ENTER key
        JR	NZ, _n3
        JP	MENU__ACTIVATE
_n3:
	RET
        

        
SETTINGS__INIT:
       	LD	IX, SETTINGS_MENU
        LD	(IX + FRAME_OFF_WIDTH), 24 ; Menu width 24
        LD	(IX + FRAME_OFF_NUMBER_OF_LINES), 5 ; Menu visible entries
        LD	(IX + MENU_OFF_DATA_ENTRIES), 5 ; Menu actual entries 
        LD 	(IX + MENU_OFF_SELECTED_ENTRY), 0 ; Selected entry
        LD	(IX+FRAME_OFF_TITLEPTR), LOW(SETTINGSMENUTITLE)
        LD	(IX+FRAME_OFF_TITLEPTR+1), HIGH(SETTINGSMENUTITLE)
        ; Entry 1
        LD	(IX+MENU_OFF_FIRST_ENTRY), 0 ; Flags
        LD	(IX+MENU_OFF_FIRST_ENTRY+1), LOW(SETTINGSENTRY1)
        LD	(IX+MENU_OFF_FIRST_ENTRY+2), HIGH(SETTINGSENTRY1);

        LD	(IX+MENU_OFF_FIRST_ENTRY+3), 0 ; Flags
        LD	(IX+MENU_OFF_FIRST_ENTRY+4), LOW(SETTINGSENTRY2)
        LD	(IX+MENU_OFF_FIRST_ENTRY+5), HIGH(SETTINGSENTRY2);
        
        LD	(IX+MENU_OFF_FIRST_ENTRY+6), 0 ; Flags
        LD	(IX+MENU_OFF_FIRST_ENTRY+7), LOW(SETTINGSENTRY3)
        LD	(IX+MENU_OFF_FIRST_ENTRY+8), HIGH(SETTINGSENTRY3)
        
        LD	(IX+MENU_OFF_FIRST_ENTRY+9), 0 ; Flags
        LD	(IX+MENU_OFF_FIRST_ENTRY+10), LOW(SETTINGSENTRY4)
        LD	(IX+MENU_OFF_FIRST_ENTRY+11), HIGH(SETTINGSENTRY4)

        LD	(IX+MENU_OFF_FIRST_ENTRY+12), 0 ; Flags
        LD	(IX+MENU_OFF_FIRST_ENTRY+13), LOW(SETTINGSENTRY5)
        LD	(IX+MENU_OFF_FIRST_ENTRY+14), HIGH(SETTINGSENTRY5)

	LD	(IX+MENU_OFF_CALLBACKPTR), LOW(SETTINGSMENUCALLBACKTABLE)
        LD	(IX+MENU_OFF_CALLBACKPTR+1), HIGH(SETTINGSMENUCALLBACKTABLE)

        LD 	(IX+MENU_OFF_DISPLAY_OFFSET), 0
        LD 	(IX+MENU_OFF_SELECTED_ENTRY), 0
        PUSH	IX
        POP	HL
        RET

SETTINGSMENUCALLBACKTABLE:
	DEFW WIFICONFIG__SHOW
        DEFW 0 ; Bluetooth
        DEFW 0 ; USB
        DEFW VIDEOMODE__SHOW
        DEFW EXITSETTINGS

SETTINGS__SHOW:
	LD	HL, SETTINGS__CLASSDEF
        JP	WIDGET__DISPLAY		; TAILCALL
        
SETTINGS__CLASSDEF:
	DEFW	SETTINGS__INIT
        DEFW	WIDGET__IDLE
        DEFW	SETTINGS__KEYHANDLER
        DEFW	SETTINGS__REDRAW
        DEFW	MENU__GETBOUNDS



SETTINGSMENUTITLE:
	DB 	"Settings", 0
SETTINGSENTRY1: DB	"Wifi...", 0
SETTINGSENTRY2: DB	"Bluetooth...", 0
SETTINGSENTRY3: DB	"USB...", 0
SETTINGSENTRY4: DB	"Video...", 0
SETTINGSENTRY5: DB	"Back", 0
