#ifndef MOUSE_H
#define MOUSE_H

bool mouseDetect;       // Detect if mouse is being used or not.
bool mouseLeftDown, mouseRightDown;
bool mouseLeftPressed, mouseRightPressed;
int mouseX, mouseY, mouseClick;

static inline int mouseInit(void);

#endif
