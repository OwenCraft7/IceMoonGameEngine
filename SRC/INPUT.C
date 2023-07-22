#include "INPUT.H"
static uint16_t keysDown[0x81];
static uint16_t keysPressed[0x81];

typedef void (__interrupt __far *kbdIntFuncPtr)();

static kbdIntFuncPtr oldKbdInterrupt; // original keyboard interrupt handler

// internal: interrupt handler for the keyboard - update key state tables
static void __interrupt __far keyboardHandler()
{
    (void)kbd_updateInput();
}

void kbd_start()    // initiate keyboard interrupt handler (irq 09h)
{
    union REGS r;
    struct SREGS s;
    _disable();
    segread(&s);

    // save old interrupt vector (irq 09h)
    r.h.al = 0x09;
    r.h.ah = 0x35;
    int386x(0x21, &r, &r, &s);
    oldKbdInterrupt = (kbdIntFuncPtr)MK_FP(s.es, r.x.ebx);

    // install new interrupt handler
    r.h.al = 0x09;
    r.h.ah = 0x25;
    s.ds    = FP_SEG(keyboardHandler);
    r.x.edx = FP_OFF(keyboardHandler);
    int386x(0x21, &r, &r, &s);

    _enable();
}

void kbd_finish()   // restore the original keyboard irq handler - required if kbd_start() was called
{
    union REGS r;
    struct SREGS s;

    if(!oldKbdInterrupt) return;

    _disable();
    segread(&s);
 
    // restore original interrupt handler
    r.h.al = 0x09;
    r.h.ah = 0x25;
    s.ds    = FP_SEG(oldKbdInterrupt);
    r.x.edx = FP_OFF(oldKbdInterrupt);
    int386x(0x21, &r, &r, &s);

    _enable();
}

// updates state of pressed/released keys - call this directly if you don't use custom irq handler
// returns an integer array with each entry representing a keypress, mapped to KeyCode enum
const uint16_t *kbd_updateInput()   
{
    uint8_t key;
    // get last key and flush keyboard buffer
    _asm {
        in al, 60h
        mov key, al
        in al, 61h
        mov ah, al
        or al, 80h
        out 61h, al
        xchg ah, al
        out 61h, al
        mov al, 20h
        out 20h, al
        mov ah, 0Ch
        mov al, 0h
        int 21h
    }

    if (key < 0x80)
    {
        keysDown[key] = 1;
        keyDown = key;
    }
    else
    {
        key -= 0x80;
        keysDown[key] = 0;
        keysPressed[key] = 0;
        keyDown = 0;
    }

    return &keysDown[0];
}

int kbd_keyPressed(enum kbd_KeyCode key)    // check if key was pressed - will return 0 on second check
{
    if(keysDown[key] && !keysPressed[key])
    {
        keysPressed[key] = 1;
        return 1;
    }
    return 0;
}

static int kbd_keyDown(enum kbd_KeyCode key)    // check if key is held down - will return 0 only if key is not being pressed
{
    return keysDown[key];
}

static void kbd_flush() // clear pressed key states - not needed if using custom irq handler
{
    memset(keysDown, 0, sizeof(uint16_t) * 0x81);
    memset(keysPressed, 0, sizeof(uint16_t) * 0x81);
}

void detectKeysChat()
{
    char keyCodePressed = kbd_keyPressed(keyDown);
    char characterToType = keycodeToChar[keyDown + shiftKeyDown * 81];

    if (keyCodePressed)
    {
        if (characterToType != '\0')
        {
            typeXPosition_Text++;
            if (typeXPosition_Text <= CHAT_LINE_LENGTH)
            {
                typeLineLength++;
                if (typeLineLength <= CHAT_LINE_LENGTH)
                {
                    memmove(type_line + typeXPosition_Text, type_line + typeXPosition_Text - 1, typeLineLength - typeXPosition_Text);
                    type_line[typeXPosition_Text - 1] = characterToType;
                }
                else
                {
                    typeXPosition_Text--;
                    typeLineLength = CHAT_LINE_LENGTH;
                }
            }
            else
                typeXPosition_Text = CHAT_LINE_LENGTH;
        }
        else
        {
            if (keyDown == BACKSPACE)
            {
                typeXPosition_Text--;
                if (typeXPosition_Text >= playerNameLengthPlusTwo)
                {
                    memmove(type_line + typeXPosition_Text, type_line + typeXPosition_Text + 1, typeLineLength - typeXPosition_Text);
                    typeLineLength--;
                }
                else
                    typeXPosition_Text = playerNameLengthPlusTwo;
            }
            else if (keyDown == ESC)   // If escape pressed,
                chat_mode = false;  // Hide chat
            else if (keyDown == ENTER) // Send a message with the enter key
            {
                scrollChatUp();
                snprintf(chat_line[19], CHAT_LINE_WITHNULL, "%s", type_line);
                chat_timer[19] = 6.0f;
                chat_mode = false;
            }
            else
            {
                typeXPosition_Text += (keyDown == RIGHT) - (keyDown == LEFT);

                if (typeXPosition_Text < playerNameLengthPlusTwo)
                    typeXPosition_Text = playerNameLengthPlusTwo;
                else if (typeXPosition_Text > typeLineLength)
                    typeXPosition_Text = typeLineLength;
                else if (typeXPosition_Text > CHAT_LINE_LENGTH)
                    typeXPosition_Text = CHAT_LINE_LENGTH;
            }
        }
    }
}

void itemKey()
{

}

void interactKey()
{

}

void inputKeyboard()
{
    int i;
    mouseStatus(&mouseX, &mouseY, &mouseClick); // Get mouse X & Y movement as well as click status.

    maxSpeed = 3.0f * (kbd_keyDown(CTRL) + 1);      // Calculate maximum speed
    acceleration = 10.0f * (kbd_keyDown(CTRL) + 1); // Calculate acceleration

    shiftKeyDown = kbd_keyDown(L_SHIFT);

    if (!chat_mode) //  If chat isn't open,
    {
        if (kbd_keyPressed(M)) // If M pressed,
        {
            mouseDetect = !mouseDetect; //  Toggle mouse and arrow keys for camera rotation and interaction.
            mousePos(160, 120); // Center the mouse.
        }
        if (kbd_keyPressed(V))  // If V pressed,
            noclip = !noclip;   // Toggle noclip

        if (mouseDetect == true)    // If mouse is plugged in,
        {
            //  Modify camera rotation based on mouse movement.
            camRotX += (mouseX - 160) * rotationSpeed * deltaTime;
            camRotY -= (mouseY - 120) * rotationSpeed * deltaTime;
            if (mouseX - 160 != 0 || mouseY - 120 != 0)
                playerMovement = true;
            mousePos(160, 120); // Center the mouse

            if (mouseLeftDown) itemKey();       //  Left-click function for weapons and misc. items.
            if (mouseRightDown) interactKey();  //  Right-click function for talking to NPCs, activating switches, and more.
        }
        else    // Otherwise,
        {
            // Use arrow keys to rotate camera.
            camRotX += (kbd_keyDown(RIGHT) - kbd_keyDown(LEFT)) * keyRotationSpeed * deltaTime;
            camRotY -= (kbd_keyDown(DOWN) - kbd_keyDown(UP)) * keyRotationSpeed * deltaTime;
            if ((kbd_keyDown(RIGHT) - kbd_keyDown(LEFT)) != 0 || (kbd_keyDown(DOWN) - kbd_keyDown(UP)) != 0)
                playerMovement = true;
            //  Q to use item and E to interact.
            if (kbd_keyDown(Q)) itemKey();
            if (kbd_keyDown(E)) interactKey();
        }

        axis_keyDown[0] = (kbd_keyDown(W) - kbd_keyDown(S)) * maxSpeed;            //  Whether the player is going forward or backward
        axis_keyDown[1] = (kbd_keyDown(D) - kbd_keyDown(A)) * maxSpeed;            //  Left or right
        axis_keyDown[2] = (kbd_keyDown(SPACE) - kbd_keyDown(L_SHIFT)) * maxSpeed;  //  Up or down

        if (kbd_keyPressed(R)) // Cycle map left
        {
            mapNumber--;
            if (mapNumber < 0)
                mapNumber = 2;
            mapCycled = true;
        }
        if (kbd_keyPressed(F)) // Cycle map right
        {
            mapNumber++;
            if (mapNumber > 2)
                mapNumber = 0;
            mapCycled = true;
            
        }
        if (mapCycled)
        {
            mapCycled = false;
            playerMovement = true;
            switch (mapNumber)
            {
                case 0:
                    load_pos("MAPS/TEST.POS");
                    break;
                case 1:
                    load_pos("MAPS/TEST2.POS");
                    break;
                case 2:
                    load_pos("MAPS/TEST3.POS");
            }
        }
        if (kbd_keyPressed(T))  // If T pressed,
        {
            chat_mode = true; // Show chat
            memset(type_line, 0x00, CHAT_LINE_WITHNULL);  // Clear text the user will type.
            snprintf(type_line, CHAT_LINE_LENGTH, "%s: ", player_name);
            typeXPosition_Text = typeLineLength = playerNameLengthPlusTwo;
        }
    }
    else   // If chat is open,
    {
        axis_keyDown[0] = axis_keyDown[1] = axis_keyDown[2] = 0.0f; // Make sure the player can't move or look.
        detectKeysChat();
    }

    //  Limit player X rotation between -180 and 180 degrees, and make it wrap around.
    if (camRotX > PI)               camRotX = -PI + 0.001f;
    else if (camRotX < -PI)         camRotX = PI - 0.001f;
    // Limit player Y rotation between -90 and 90 degrees.
    if (camRotY > HALF_PI)          camRotY = HALF_PI;
    else if (camRotY < -HALF_PI)    camRotY = -HALF_PI;
    
    for (i = 0; i < 2 + noclip; i++)    // For each axis of movement,
    {
        if (axis_speed[i] < axis_keyDown[i])
        {
            axis_speed[i] += acceleration * deltaTime;    // Increase speed if its below its maximum limit.
            if (axis_speed[i] > axis_keyDown[i])
                axis_speed[i] = axis_keyDown[i];
        }
        else if (axis_speed[i] > axis_keyDown[i])
        {
            axis_speed[i] -= acceleration * deltaTime;    // Increase speed if its above its negative maximum limit.
            if (axis_speed[i] < axis_keyDown[i])
                axis_speed[i] = axis_keyDown[i];
        }
    }
}
