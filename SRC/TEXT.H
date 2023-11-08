#ifndef TEXT_H
#define TEXT_H

#define CHAT_LINE_LENGTH	127
#define CHAT_LINE_WITHNULL	128

enum font
{
	FONT_CHAT = 0,
	FONT_DEBUG = 1,
	FONT_GUI = 2,
	FONT_MENU = 3,
	FONT_SUB = 4
};

typedef struct
{
	int charcode[256];
} vwf_settings; // VWF = Variable Width Font
// A vwf_settings struct contains 256 ints each defining a unique character's width in pixels.

char chat_line[20][CHAT_LINE_WITHNULL];		// 20 chat lines on screen, each containing 127 characters (#128 is the null terminator)
char debug_line[10][CHAT_LINE_WITHNULL];	// 10 debug lines on screen displaying misc. info
char type_line[CHAT_LINE_WITHNULL];			// A type line displaying what the player is typing to chat
float chat_timer[20];	// Each timer in the array decreases by 1 every second.
						// When the timer reaches 0, its corresponding chat line is invisible...
						// unless chat_mode is true.

int font_count = 0;
int typeXPosition_Text, typeXPosition_Screen, typeLineLength;
image* fontnumber;
vwf_settings* fontnumber_vwf;

#endif
