#ifndef __MEMLAYOUT_H__
#define __MEMLAYOUT_H__


#define MEMLAYOUT_ROM0_BASEADDRESS (0x000000)
#define MEMLAYOUT_ROM0_SIZE        (0x002000)
#define MEMLAYOUT_ROM1_BASEADDRESS (0x002000)
#define MEMLAYOUT_ROM1_SIZE        (0x002000)
#define MEMLAYOUT_ROM2_BASEADDRESS (0x004000)
#define MEMLAYOUT_ROM2_SIZE        (0x004000)

#define MEMLAYOUT_RAM_BASEADDRESS(x) (0x010000 + ((x)+0x2000))
#define MEMLAYOUT_RAM_SIZE(x) (0x1FFF)


#define NMI_ROM_BASEADDRESS MEMLAYOUT_ROM0_BASEADDRESS
#define NMI_ROM_SIZE MEMLAYOUT_ROM0_SIZE

#endif
