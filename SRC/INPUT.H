#ifndef INPUT_H
#define INPUT_H

//  Simple key input processing.

bool aKeyPressed, shiftKeyDown;
uint8_t keyDown = 0, lastKeyDown = 0;
const uint16_t *kbd_updateInput();

static inline int kbd_keyDown(enum kbd_KeyCode key);
static inline void kbd_flush();

#endif
