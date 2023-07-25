#ifndef SOUND_H
#define SOUND_H

short sb_base;
char sb_irq, sb_dma;

int sb_detected;

#define SB_RESET			0x06
#define SB_READ_DATA		0x0A
#define SB_READ_DATA_STATUS	0x0E
#define SB_WRITE_DATA		0x0C

typedef struct {
	bool sixteenBit;	// If false, set to 8-bit sound
	bool stereo;		// If true, the left channel's data comes immediately before the right channel.
	int length;			// Length in bytes, not samples
	int samplesPerSec;	// Example: An 8-bit 22050hz mono WAV file's speed is 22050 bytes per second.
	char* byte;
} sfx;	// This can be a voice line, a musical instrument, a GUI button, a weapon firing, and much more.

#endif
