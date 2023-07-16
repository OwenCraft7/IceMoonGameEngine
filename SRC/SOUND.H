#ifndef SOUND_H
#define SOUND_H

short sb_base;
char sb_irq, sb_dma;

int sb_detected;

#define SB_RESET			0x06
#define SB_READ_DATA		0x0A
#define SB_READ_DATA_STATUS	0x0E
#define SB_WRITE_DATA		0x0C

#endif
