#ifndef MOUSE_H
#define MOUSE_H

bool mouseDetect;       // Detect if mouse is being used or not.
bool mouseLeftDown, mouseRightDown;
bool mouseLeftPressed, mouseRightPressed;
int mouseX, mouseY, mouseClick;

enum mouse_KeyCode
{
    MOUSE_LEFT = 1, MOUSE_RIGHT = 2, MOUSE_BOTH = 3
};

static inline int mouseInit(void);

#endif
