#ifndef INPUT_H
#define INPUT_H

//  Simple key input processing.

bool shiftKeyDown;
uint8_t keyDown;
const uint16_t *kbd_updateInput();

static inline int kbd_keyDown(enum kbd_KeyCode key);
static inline void kbd_flush();

#endif
