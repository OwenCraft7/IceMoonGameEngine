/*-------------------------------*/
/*        Ice Moon  gui.c        */
/*                               */
/* Graphical User Interface Code */
/*-------------------------------*/

#include "GUI.H"

void loadGUIScreens()	// Loads every GUI screen referenced in GUISCR.CFG
{
    char cfgpath[20], screenfile[9], screenpath[34], cmdstring[16];
    int i, cMin1, sMin1;
    FILE *guiscrcfg, *guiscreenfile;
    gui_parameters* GUIp;

    // Allocate memory for GUI Screen #0, the blank one.
    mainGUIScreen = calloc(1, sizeof(gui_screen));
    // Obtain GUISRC.CFG file path
    sprintf(cfgpath, "%s/GUISCR.CFG", mod_folder);

    // Open GUISRC.CFG
    if (!endGameLoop && (guiscrcfg = fopen(cfgpath, "r")) != NULL)
    {
        // Repeat line by line until end of the CFG file.
        while (fscanf(guiscrcfg, "%s", screenfile) != EOF)
        {
            // Obtain the listed GUI Screen TXT file path.
            sprintf(screenpath, "%s/GUI/SCREENS/%s.TXT", mod_folder, screenfile);
            // screenCount increases by one.
            screenCount++;
            sMin1 = screenCount - 1;
            // Open the GUI Screen file.
            if (!endGameLoop && (guiscreenfile = fopen(screenpath, "r")) != NULL)
            {
                // Allocate memory for screenCount GUI screens.
                mainGUIScreen = realloc(mainGUIScreen, sizeof(gui_screen) * screenCount);
                // Repeat line by line until end of the TXT file.
                while (fscanf(guiscreenfile, "%s", cmdstring) != EOF)
                {
                    // Allocate memory for more command parameters.
                    mainGUIScreen[sMin1].command_count++;
                    mainGUIScreen[sMin1].command_parameters = realloc(mainGUIScreen[sMin1].command_parameters, sizeof(gui_parameters) * mainGUIScreen[sMin1].command_count);
                    GUIp = mainGUIScreen[sMin1].command_parameters;
                    cMin1 = mainGUIScreen[sMin1].command_count - 1;
                    if (strcmp(cmdstring, "IMAGE") == 0)
                    {
                        GUIp[cMin1].value[0] = GUI_IMAGE;
                        for (i = 1; i <= 3; i++)
                            fscanf(guiscreenfile, "%d", &GUIp[cMin1].value[i]);
                    }
                    else if (strcmp(cmdstring, "TEXT") == 0)
                    {
                        GUIp[cMin1].value[0] = GUI_TEXT;
                        for (i = 1; i <= 4; i++)
                            fscanf(guiscreenfile, "%d", &GUIp[cMin1].value[i]);
                        if (GUIp[cMin1].value[4] == 0)
                            fscanf(guiscreenfile, "%s", GUIp[cMin1].optionalString);
                    }
                    else if (strcmp(cmdstring, "TEXTINTERACT") == 0)
                    {
                        GUIp[cMin1].value[0] = GUI_TEXTINTERACT;
                        for (i = 1; i <= 12; i++)
                            fscanf(guiscreenfile, "%d", &GUIp[cMin1].value[i]);
                        fscanf(guiscreenfile, "%s", cmdstring);
                        if (strcmp(cmdstring, "DESTINATION") == 0)
                            GUIp[cMin1].value[11] = GUI_DESTINATION;
                        else if (strcmp(cmdstring, "FUNCTION") == 0)
                            GUIp[cMin1].value[11] = GUI_FUNCTION;
                        fscanf(guiscreenfile, "%d", &GUIp[cMin1].value[12]);
                        fscanf(guiscreenfile, "%s", GUIp[cMin1].optionalString);
                    }
                }
            }
            fclose(guiscreenfile);
        }
    }
    else
    {
        endGameLoop = true;
        errormessage = 6;
    }
    fclose(guiscrcfg);
}

void showGUIScreen(screen)	// Displays a GUI screen based on the screen parameter.
{
    int i;
    int* GUIVal;
    char* GUIStr;
    for (i = 0; i < mainGUIScreen[screen].command_count; i++)
    {
        GUIVal = mainGUIScreen[screen].command_parameters[i].value;
        GUIStr = mainGUIScreen[screen].command_parameters[i].optionalString;
        if (GUIVal[0] == GUI_IMAGE)
            drawimage(imgnumber[GUIVal[3]], GUIVal[1], GUIVal[2]);
        else if (GUIVal[0] == GUI_TEXT)
        {
            if (GUIVal[4] == 0)
                printtext(GUIStr, GUIVal[3], GUIVal[1], GUIVal[2]);
            else if (GUIVal[4] >= 1 && GUIVal[4] <= 20)
            {
                if (chat_mode || chat_timer[GUIVal[4] - 1] > 0.0f)
                    printtext(chat_line[GUIVal[4] - 1], GUIVal[3], GUIVal[1], GUIVal[2]);
            }
            else if (GUIVal[4] >= 21 && GUIVal[4] <= 30)
                printtext(debug_line[GUIVal[4] - 21], GUIVal[3], GUIVal[1], GUIVal[2]);
            else if (GUIVal[4] == 31 && chat_mode)
                printtext(type_line, GUIVal[3], GUIVal[1], GUIVal[2]);
            else if (GUIVal[4] == 32)
                printtext(HPCurrentString, GUIVal[3], GUIVal[1], GUIVal[2]);
            else if (GUIVal[4] == 33)
                printtext(HPMaxString, GUIVal[3], GUIVal[1], GUIVal[2]);
            else if (GUIVal[4] == 34)
                printtext(AmmoLoadedString, GUIVal[3], GUIVal[1], GUIVal[2]);
            else if (GUIVal[4] == 35)
                printtext(AmmoUnloadedString, GUIVal[3], GUIVal[1], GUIVal[2]);
        }
        else if (GUIVal[0] == GUI_TEXTINTERACT)
        {
            printtext(GUIStr, GUIVal[3], GUIVal[1], GUIVal[2]);
        }
    }
}

void free_gui()
{
    int i;
    for (i = 0; i < screenCount; i++)
        free(mainGUIScreen[i].command_parameters);
    free(mainGUIScreen);
}
