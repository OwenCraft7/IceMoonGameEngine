/*----------------------*/
/*   Ice Moon main.c    */
/*                      */
/*  Start of compiling  */
/*----------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <sys/timeb.h>
#include <math.h>
#include <i86.h>
#include <dos.h>
#include <memory.h>
#include <time.h>
#include <graph.h>

#include "MISC.C"       // Miscellaneous.
#include "TIME.C"       // Calculates the delta time and FPS.
#include "SOUND.C"      // Handles SFX and music.
#include "GFX.C"        // Code for graphics, loading, and drawing images. Some of the code comes from Brackeen's VGA tutorial.
#include "POSFILE.C"    // Loads maps.
#include "TEXTURE.C"    // Draws triangles from the node leaves.
#include "NODES.C"      // Involved with the BSP tree; checks node planes and draws node leaves.
#include "PHYS.C"       // Game Physics.
#include "TEXT.C"       // Prints text for chat, debug, HUD, menus, and subtitles.
#include "GUI.C"        // Code for the Graphical User Interface.
#include "MOUSE.C"      // Input from mouse.
#include "INPUT.C"      // Input from keyboard. INPUT.C & INPUT.H are from Krzysztof Kondrak's DOS3D program.
#include "LOGFILE.C"    // Prints a log file after the game ends.

int main(int argc, char *argv[])
{
    int i, j;   // Used with for-statements and other

    _clearscreen(_GCLEARSCREEN);
    setbuf(stdout, NULL);
    printf("LOADING ICE MOON\n\nDetecting SoundBlaster...");
    sb_detected = sb_detect();                      // Detect if SoundBlaster is in computer (or DOS emulator)

    printf("\nDetecting Mouse...");
    mouseDetect = mouseInit() == 0 ? false : true;  // Detect if there's a mouse connected. Will be set true if connected.
    printf("\nLoading Config...");
    loadCfg();                                      // Load the configuration file and its settings
    printf("\nLoading GUI Screens...");
    loadGUIScreens();

    // ARGUMENTS FROM COMMAND LINE:
    for (i = 1; i < argc; i++)
    {
        if (argv[i] == "-mouse_cursor")
            mouseDetect = false;
        else if (argv[i] == "-noclip")
            noclip = true;
    }

    printf("\nLoading Map...");
    load_pos("MAPS/TEST2.POS"); // Load map #2 in the EXAMPLE folder

    printf("\nLoading Fonts...");
    font_count = 3;
    fontnumber = calloc(font_count, sizeof(image)); // Allocate memory for fonts to be loaded
    fontnumber_vwf = calloc(font_count, sizeof(vwf_settings));
    loadfont("CHAT_0", FONT_CHAT);
    loadfont("DEBUG_0", FONT_DEBUG);
    loadfont("GUI_0", FONT_GUI);

    printf("\nLoading Images...");
    img_count = 9;
    imgnumber = calloc(img_count, sizeof(image));   // Allocate memory for images to be loaded
    loadimage("NULL.BMP", &imgnumber[0]);
    loadimage("COLMAPS/BLEND.BMP", &imgnumber[1]);      // A colormap used to blend two pixel colors
    loadimage("COLMAPS/MULTIPLY.BMP", &imgnumber[2]);   // A colormap used to multiply two pixel colors
    loadimage("GUI/CROSHAIR.BMP", &imgnumber[3]);       // Crosshair in the screen's middle
    loadimage("GUI/HEALTH.BMP", &imgnumber[4]);         // HP in the screen's bottom-left corner
    loadimage("GUI/AMMO.BMP", &imgnumber[5]);           // Ammo in the screen's bottom-right corner
    loadimage("GUI/CURSOR.BMP", &imgnumber[6]);         // A mouse cursor
    loadimage("GUI/TYPE.BMP", &imgnumber[7]);           // A text cursor
    loadimage("GUI/QUIT.BMP", &imgnumber[8]);  // A placeholder wireframe texture

    // Shortcuts to the blend and multiply colormaps. Each colormap is 256x256 pixels in size.
    blend_map = imgnumber[1].pixel;
    multiply_map = imgnumber[2].pixel;
    /* NOTE: Perhaps someone or I could simplify both colormaps to two channels each:
             blue and RG, which would significantly reduce game storage and RAM size.

             The blue channel in both colormaps would be 4x4 pixels large, and repeated on a 4x4 pixel grid.
             The RG channel would be used for both red and green colors, and would be 8x8 pixels large.
             Multiply the RG channel's size by 4 for green, and repeat it on a 32x32 pixel grid.
             Multiply the RG channel's size by 32 for red, and make it encompass the entire colormap. */

    modeX();            // Switch to VGA Mode X
    palette(9, 9, 21);  // Initialize color palette in the 8-8-4 RGB format, adding up to 256 colors.
    kbd_start();        // Initialize keyboard input
    
    mouseLimits(0, SCREEN_WIDTH - 1, 0, SCREEN_HEIGHT - 1);
    mousePos(HALF_WIDTH, HALF_HEIGHT);

    lastClockUpdate = clock();
    /*                  */
    /*  MAIN GAME LOOP  */
    /*                  */
    while (!endGameLoop)
    {
        updateDeltaTime();                          // Update delta time
        if (!paused)
        {
            if (secondsUpdate < seconds) updateFPS();   // Update frames per second

            inputKeyboard();        // Get Player Input
            playerPhysicsUpdate();  // Update Player Physics

            if (playerMovement == true)
            {
                // Clear level and distance buffers
                memset(level_buffer, BACKGROUND_COLOR, SCREEN_SIZE);
                memset(dist_buffer, 0x3B, SCREEN_SIZE_T4);  // Sets each pixel in distance buffer to 0.00286 (Inverse of 350.0273, which is the maximum view distance.)
                checkRootNode(playerPos.x, camPosY, playerPos.z, true, &cameraLeaf);
            }
            playerMovement = false;
            memcpy(col_buffer, level_buffer, SCREEN_SIZE);    // Copies level buffer to the color buffer, even if the level hasn't been regenerated.
            if (kbd_keyPressed(ESC) && !chat_mode)
            {
                paused = true;
                activeGUIScreen = 2;
                mousePos(HALF_WIDTH, HALF_HEIGHT); // Center the mouse.
            }
            frames++; seconds += deltaTime; // Increase frame count by one and increase seconds by the delta time.
        }
        else
        {
            if (kbd_keyPressed(Y))
                endGameLoop = true;
            else if (kbd_keyPressed(N))
            {
                paused = false;
                activeGUIScreen = 1;
                mousePos(HALF_WIDTH, HALF_HEIGHT); // Center the mouse.
            }
        }

        // Draw GUI
        showGUIScreen(activeGUIScreen);
        setupText();
        //drawMemory(frames);   // This function displays the game's RAM as one byte per pixel. In simple words, this shows clown vomit.
        //if (mouseDetect == false) drawimage(imgnumber[6], mouseX, mouseY);
        
        if (!endGameLoop)
        {
            // It's time to write to VGA memory!
            for (i = 0; i < 4; i++) // i = Mode X Plane
            {
                outp(SC_DATA, 1 << i);                              // The outp() function is expensive, so it's called only four times per frame.
                for (j = i; j < SCREEN_SIZE; j += 4)                // Draw 1/4th of the screen
                    VGA[nonVisible + (j >> 2)] = col_buffer[0][j];  // Set pixel from color buffer to VGA memory
            }
            flip(); // Flip Mode X pages
        }
    }

    kbd_finish();       // End keyboard input
    videoMode(0x03);    // Return to Text Mode
    logfile();          // Create a text file containing technical game statistics

    // Free a bunch of RAM before we exit the game
    free_fonts();
    free_gui();
    free_images();
    free_mapdata();

    // These are error messages:
    switch (errormessage)
    {
        case 0:
            printf("Thanks for playing Ice Moon.");
            break;
        case 1:
            printf("ERROR: Unable to allocate image data.");
            break;
        case 2:
            printf("ERROR: Failed to load configuration file.");
            break;
        case 3:
            printf("ERROR: Unable to allocate map data.");
            break;
        case 4:
            printf("ERROR: Incorrect map header or wrong map file format.");
            break;
        case 5:
            printf("ERROR: Couldn't generate log file; the disk space the game is in could be read-only. Estimated RAM is also set to 0.");
            break;
        case 6:
            printf("ERROR: Failed to load GUI screens.");
            break;
        default:
            printf("ERROR: Unknown, check source code for any problems.");
    }
    printf("\n\nVertex Count: %d\nUV Vertex Count: %d\nTriangle Count: %d\n", vert_count, uv_count, tri_count);
    printf("Node Count: %d\nLeaf Count: %d\nTexture Count: %d\nEntity Count: %d\n\n", node_count, leaf_count, texture_count, entity_count);
    printf("Image Count: %d\nFont Count: %d\n\n", img_count, font_count);
    printf("Estimated RAM for these objects: %d Bytes\n", estimate_ram);
    printf("Mod Folder: %s\nPlayer Name: %s\n", mod_folder, player_name);

    return 0;
}
