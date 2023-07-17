#ifndef INPUT_H
#define INPUT_H

//  Simple key input processing.

enum kbd_KeyCode
{
    ESC = 1,        NUM1 = 2,       NUM2 = 3,       NUM3 = 4,
    NUM4 = 5,       NUM5 = 6,       NUM6 = 7,       NUM7 = 8,
    NUM8 = 9,       NUM9 = 10,      NUM0 = 11,      MINUS = 12,
    PLUS = 13,      BACKSPACE = 14, TAB = 15,       Q = 16,
    W = 17,         E = 18,         R = 19,         T = 20,
    Y = 21,         U = 22,         I = 23,         O = 24,
    P = 25,         L_BRACKET = 26, R_BRACKET = 27, ENTER = 28,
    CTRL = 29,      A = 30,         S = 31,         D = 32,
    F = 33,         G = 34,         H = 35,         J = 36,
    K = 37,         L = 38,         SEMICOLON = 39, APOST = 40,
    GRAVE = 41,     L_SHIFT = 42,   BACKSLASH = 43, Z = 44,
    X = 45,         C = 46,         V = 47,         B = 48,
    N = 49,         M = 50,         COMMA = 51,     PERIOD = 52,
    QUESTION = 53,  R_SHIFT = 54,   PRTSC = 55,     ALT = 56,
    SPACE = 57,     CAPS = 58,      F1 = 59,        F2 = 60,
    F3 = 61,        F4 = 62,        F5 = 63,        F6 = 64,
    F7 = 65,        F8 = 66,        F9 = 67,        F10 = 68,
    NUMLOCK = 69,   SCRLOCK = 70,   HOME = 71,      UP = 72,
    PGUP = 73,      LEFT = 75,      CENTER = 76,    RIGHT = 77,
    END = 79,       DOWN = 80,      PGDOWN = 81,    INS = 82,
    DEL = 83
};

const uint16_t *kbd_updateInput();

static inline int kbd_keyDown(enum kbd_KeyCode key);
static inline void kbd_flush();

#endif
