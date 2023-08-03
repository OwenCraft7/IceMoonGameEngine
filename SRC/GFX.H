#ifndef GFX_H
#define GFX_H

#define DISPLAY_ENABLE	0x01
#define MAP_MASK		0x02
#define MEMORY_MODE		0x04
#define V_TOTAL         0x06
#define VRETRACE        0x08
#define HIGH_ADDRESS	0x0C
#define LOW_ADDRESS		0x0D
#define V_RETRACE_START 0x10
#define V_RETRACE_END   0x11
#define V_DISPLAY_END   0x12
#define UNDERLINE_LOC	0x14
#define V_BLANK_START   0x15
#define V_BLANK_END     0x16
#define MODE_CONTROL	0x17
// VGA sequence controller
#define SC_INDEX		0x03c4
#define SC_DATA			0x03c5
#define INPUT_STATUS	0x03da
// VGA CRT controller
#define CRTC_INDEX		0x03d4
#define CRTC_DATA		0x03d5

// Game Engine Specifics
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define HALF_WIDTH      160
#define HALF_HEIGHT     120

typedef struct
{
    unsigned char c;
} color;
typedef struct
{
    unsigned short width, height;
    int transparent;    // Set it to -1 to make no color transparent.
    color* pixel;
} image;

unsigned char* VGA = (unsigned char*)0xA0000;

int img_count = 0;
image* imgnumber;
color* tempimgdata;
int tempimgTransparent;
int nonVisible;

color* blend_map;
color* multiply_map;
float dist_buffer[240][320];
unsigned char level_buffer[240][320], col_buffer[240][320];

static inline void videoMode(uint8_t mode);
static inline void pixel(const int, const int, const char);
static inline char blendColor(const char, const char);
static inline char multiplyColor(const char, const char);

#endif
