#ifndef MISC_H
#define MISC_H

#define sgn(x) ((x>0) - (x<0))

#define BACKGROUND_COLOR 0

union REGS in, out;

const double PI =           3.1415926535897932384626433832795;
const double HALF_PI =      1.5707963267948966192313216916398;
const double PI180 =        0.01745329251994329576923690768489; // Pi DIVIDED BY 180, not multiplied!
const double INVS_PI180 =   57.295779513082320876798154814105;  // 1 / (Pi / 180)

enum kbd_KeyCode
{
    ESC = 1,        NUM1 = 2,       NUM2 = 3,       NUM3 = 4,
    NUM4 = 5,       NUM5 = 6,       NUM6 = 7,       NUM7 = 8,
    NUM8 = 9,       NUM9 = 10,      NUM0 = 11,      MINUS = 12,
    EQUALS = 13,    BACKSPACE = 14, TAB = 15,       Q = 16,
    W = 17,         E = 18,         R = 19,         T = 20,
    Y = 21,         U = 22,         I = 23,         O = 24,
    P = 25,         L_BRACKET = 26, R_BRACKET = 27, ENTER = 28,
    CTRL = 29,      A = 30,         S = 31,         D = 32,
    F = 33,         G = 34,         H = 35,         J = 36,
    K = 37,         L = 38,         SEMICOLON = 39, APOST = 40,
    GRAVE = 41,     L_SHIFT = 42,   BACKSLASH = 43, Z = 44,
    X = 45,         C = 46,         V = 47,         B = 48,
    N = 49,         M = 50,         COMMA = 51,     PERIOD = 52,
    SLASH = 53,     R_SHIFT = 54,   PRTSC = 55,     ALT = 56,
    SPACE = 57,     CAPS = 58,      F1 = 59,        F2 = 60,
    F3 = 61,        F4 = 62,        F5 = 63,        F6 = 64,
    F7 = 65,        F8 = 66,        F9 = 67,        F10 = 68,
    NUMLOCK = 69,   SCRLOCK = 70,   HOME = 71,      UP = 72,
    PGUP = 73,      LEFT = 75,      CENTER = 76,    RIGHT = 77,
    END = 79,       DOWN = 80,      PGDOWN = 81,    INS = 82,
    DEL = 83
};

char keycodeToChar[162] =
{
    '\0', '\0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', '0', '-',
    '=', '\0', ' ', 'q',
    'w', 'e', 'r', 't',
    'y', 'u', 'i', 'o',
    'p', '[', ']', '\0',
    '\0', 'a', 's', 'd',
    'f', 'g', 'h', 'j',
    'k', 'l', ';', '\'',
    '`', '\0', '\\', 'z',
    'x', 'c', 'v', 'b',
    'n', 'm', ',', '.',
    '/', '\0', '\0', '\0',
    ' ', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '!', '@', '#',
    '$', '%', '^', '&',
    '*', '(', ')', '_',
    '+', '\0', ' ', 'Q',
    'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O',
    'P', '{', '}', '\0',
    '\0', 'A', 'S', 'D',
    'F', 'G', 'H', 'J',
    'K', 'L', ':', '"',
    '~', '\0', '|', 'Z',
    'X', 'C', 'V', 'B',
    'N', 'M', '<', '>',
    '?', '\0', '\0', '\0',
    ' ', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0',
};

typedef struct
{
	float x, y, z;
} vert;

// Player Properties
vert playerPos = { 1.0f, 0.0f, 0.0f };
vert prevPlayerPos;
vert dummyTri[3] = { 0, 0, 2,
					-2, 0, -2,
					2, 0, -2 };
float camRotX, camRotY, camPosY, headPosY;

// Miscellaneous Properties
bool endGameLoop, noclip, paused, chat_mode, mapCycled, typeCursorVisible;
int mapNumber, errormessage;

int playerHPCurrent = 100, playerAmmoLoaded = 24;
int playerHPMax = 150, playerAmmoUnloaded = 100;
char HPCurrentString[4], AmmoUnloadedString[4];
char HPMaxString[4], AmmoLoadedString[4];

char mod_folder[9], player_name[21];
int playerNameLengthPlusTwo;

static inline void swapfloat(float*, float*);
static inline void swapthreefloats(float*, float*, float*);
static inline void swapint(int*, int*);
static inline void fgets_noNewline(char*, int, FILE*);
static inline float dot(vert, vert);
static inline void cross(float, float, float, float, float, float, vert*);

#endif
