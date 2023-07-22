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

#include "MISC.C"       // Miscellaneous.
#include "TIME.C"       // Calculates the delta time and FPS.
#include "SOUND.C"      // Handles SFX and music.
#include "GFX.C"        // Code for graphics, loading, and drawing images. Some of the code comes from Brackeen's VGA tutorial.
#include "PHYS.C"       // Game Physics.
#include "POSFILE.C"    // Loads maps.
#include "MOUSE.C"      // Input from mouse.
#include "TEXT.C"       // Prints text for chat, debug, HUD, menus, and subtitles.
#include "INPUT.C"      // Input from keyboard. INPUT.C & INPUT.H are from Krzysztof Kondrak's DOS3D program.
#include "TEXTURE.C"    // Draws triangles from the node leaves.
#include "NODES.C"      // Involved with the BSP tree; checks node planes and draws node leaves.
#include "LOGFILE.C"    // Prints a log file after the game ends.

int main(int argc, char **argv[])
{
    int i, j;           // Used with for-statements and other

    // Visual and active page variables used for Mode X page flipping.
    int visual_page = 0;
    int active_page = 19200;

    endGameLoop = false;    // If true, the game will end as soon as it possibly can.

    /*======================//
    //  Real Start of Code  //
    //======================*/

    sb_detected = sb_detect();  // Detect if SoundBlaster is in computer (or DOS emulator)

    mouseDetect = mouseInit() == 0 ? false : true;  // Detect if there's a mouse connected. Will be set true if connected.

    loadCfg();  // Load the configuration file and its settings

    // ARGUMENTS FROM COMMAND LINE:
    for (i = 1; i < argc; i++)
    {
        if (*argv[i] = "nm") // If "-nm" argument detected, disable mouse functions.
            mouseDetect = false;
    }

    load_pos("MAPS/TEST.POS");    // Load the main map

    font_count = 3;
    fontnumber = calloc(font_count, sizeof(image));   // Allocate memory for fonts to be loaded
    fontnumber_vwf = calloc(font_count, sizeof(vwf_settings));
    loadfont("CHAT_0", FONT_CHAT);
    loadfont("DEBUG_0", FONT_DEBUG);
    loadfont("GUI_0", FONT_GUI);

    img_count = 8;
    imgnumber = calloc(img_count, sizeof(image));   // Allocate memory for images to be loaded
    loadimage("NULL.BMP", &imgnumber[0]);
    loadimage("COLMAPS/BLEND.BMP", &imgnumber[1]);      // A colormap used to blend two pixel colors
    loadimage("COLMAPS/MULTIPLY.BMP", &imgnumber[2]);   // A colormap used to multiply two pixel colors
    loadimage("GUI/CROSHAIR.BMP", &imgnumber[3]);
    loadimage("GUI/HEALTH.BMP", &imgnumber[4]);
    loadimage("GUI/AMMO.BMP", &imgnumber[5]);
    loadimage("GUI/CURSOR.BMP", &imgnumber[6]);
    loadimage("GUI/TYPE.BMP", &imgnumber[7]);

    // Shortcuts to the blend and multiply colormaps. Each colormap is both 256x256 pixels in size.
    blend_map = imgnumber[1].pixel;
    multiply_map = imgnumber[2].pixel;
    /* NOTE: Perhaps me or someone could simplify both colormaps each into two channels:
             blue and RG, which would significantly reduce game and RAM size.

             The blue channel in both colormaps would be 4x4 pixels large, and repeated on a 4x4 pixel grid.
             The RG channel would be used for both red and green colors, and would be 8x8 pixels large.
             Green would have the RG channel's size multiplied by 4, and repeated on a 32x32 pixel grid.
             Red would have the RG channel's size multiplied by 32, and encompass the entire colormap. */

    videoMode();        // Switch to VGA Mode X
    palette(9, 9, 21);  // Initialize color palette in the 8-8-4 RGB format, adding up to 256 colors.

    kbd_start();    // Initialize keyboard input
    
    mouseLimits(0, 319, 0, 239);
    mousePos(160, 120);

    ftime(&delt1);
    while (!endGameLoop) // Main game loop.
    {
        updateDeltaTime();  // Update delta time
        if (seconds_quarterSecondUpdate < seconds) updateFPS();  // Update frames per second

        inputKeyboard();        // Get Player Input
        playerPhysicsUpdate();  // Update Player Physics
        
        if (playerMovement == true)
        {
            /* Clear Buffers: */
            memset(level_buffer, BACKGROUND_COLOR, 76800);

            // Sets each pixel in distance buffer to 195.263f. (MAXIMUM VIEW DISTANCE)
            memset(dist_buffer, 0x43, 307200);

            playerLeaf = -1;    // It's set to -1 so the game can check the node leaf the player is in.
            /*  The start of the map rendering process.
            First checks if the camera's in front or behind node plane #0,
            then we go down further from here.  */
            checknode(map_node[0]);
        }
        // Copies level buffer to general color buffer, even if the level hasn't been regenerated.
        memcpy(col_buffer, level_buffer, 76800);
        playerMovement = false;

        /*  Draw GUI  */
        drawimage(imgnumber[4], 0, 208);    // HP
        drawimage(imgnumber[5], 288, 208);  // Ammo
        drawimage(imgnumber[3], 156, 116);  // Crosshair

        snprintf(debug_line[0], CHAT_LINE_WITHNULL, "FPS: %d, Noclip: %d, Map: %d", framesPerSecond, noclip, mapNumber);// , aKeyPressed);
        //snprintf(debug_line[1], CHAT_LINE_WITHNULL, "Leaf: %d, Crosshair Distance: %.2fm", playerLeaf, dist_buffer[120][160]);
        //snprintf(debug_line[2], CHAT_LINE_WITHNULL, "LP: %d, RP: %d, LD: %d, RD: %d", mouseLeftPressed, mouseRightPressed, mouseLeftDown, mouseRightDown);
        snprintf(debug_line[1], CHAT_LINE_WITHNULL, "Coordinates: %.2f, %.2f, %.2f", playerPos.x, playerPos.y, playerPos.z);
        //snprintf(debug_line[2], CHAT_LINE_WITHNULL, "Collide X: %.2f, Y: %.2f, Z: %.2f", playerCollideIntersect.x, playerCollideIntersect.y, playerCollideIntersect.z);
        if (!sb_detected)
            snprintf(debug_line[2], CHAT_LINE_WITHNULL, "SoundBlaster is not connected.");
        else
            snprintf(debug_line[2], CHAT_LINE_WITHNULL, "SoundBlaster is found at A%x I%u D%u.", sb_base, sb_irq, sb_dma);

        displayAllText();

        if (mouseDetect == false)
            drawimage(imgnumber[6], mouseX, mouseY);
        if (kbd_keyPressed(ESC) && !chat_mode)
            endGameLoop = true;

        if (!endGameLoop)
        {
            //  It's time to write to actual VGA memory!
            //  i = Mode X Plane
            for (i = 0; i < 4; i++)
            {
                // The outp() function is expensive, so it's called only four times per frame.
                outp(SC_DATA, 1 << i);

                // Draw 1/4th of the screen:
                for (j = i; j < 76800; j += 4)
                    VGA[nonVisible + (j >> 2)] = col_buffer[0][j]; // Set pixel from color buffer to VGA memory
            }
            // Flip Mode X pages
            flip(&visual_page, &active_page);

            frames++; seconds += deltaTime; // Increase frame count by one and seconds by the delta time.
        }
    }

    kbd_finish();   // End keyboard input
    textMode();     // Return to Text Mode

    logfile();  // Print a text file with certain statistics

    //  Free a bunch of RAM before we exit the game
    free_mapdata();
    free(tempimgdata);
    free_images();
    free_fonts();
    free_textures();

    //  These are error messages:
    switch (errormessage)
    {
        case 0:
            printf("Thanks for playing Ice Moon.");
            break;
        case 1:
            printf("ERROR: Unable to allocate image data");
            break;
        case 2:
            printf("ERROR: Failed to load configuration file");
            break;
        case 3:
            printf("ERROR: Unable to allocate map data");
            break;
        case 4:
            printf("ERROR: Incorrect map header");
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
