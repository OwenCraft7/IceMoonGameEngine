/*-----------------------*/
/*    Ice Moon text.c    */
/*                       */
/*   For printing text   */
/*-----------------------*/

#include "TEXT.H"

void loadfont(char* font, int number)   // Loads fonts into the game
{
    FILE *tempvwf;  // VWF = Variable Width Font
    int i, byte;
    char filepath[32];

    sprintf(filepath, "GUI/FONTS/%s.BMP", font);
    loadimage(filepath, &fontnumber[number]);
    sprintf(filepath, "%s/GUI/FONTS/%s.BIN", mod_folder, font);

    if ((tempvwf = fopen(filepath, "rb")) != NULL)
    {
        fseek(tempvwf, 0, SEEK_SET);  // Go to start of file
        for (i = 0; i < 256; i++)
        {
            byte = fgetc(tempvwf);
            fontnumber_vwf[number].charcode[i] = (byte & 240) >> 4; i++;
            fontnumber_vwf[number].charcode[i] = byte & 15;
        }
    }
    else for (i = 0; i < 256; i++)
            fontnumber_vwf[number].charcode[i] = fontnumber[number].width >> 4;

    fclose(tempvwf);
}

void printtext(const char* text, int number, const int x, const int y)  // Prints text with a unique font and screen location
{
    int i, j, grid_width, grid_height, color;
    int shift_x, offset_y, img_pos;
    int char_num, total_length = 0, char_width;
    int* vwf_list = fontnumber_vwf[number].charcode;

    grid_width = fontnumber[number].width >> 4;     // Font image width divided by 16
    grid_height = fontnumber[number].height >> 4;   // Font image height divided by 16 too

    offset_y = y + grid_height;
    if (offset_y > 239) offset_y = 239;

    for (char_num = 0; char_num < strlen(text); char_num++) // For every char in the string,
    {
        shift_x = x + total_length;     // X screen position of top-left of char
        if (shift_x < 321 - grid_width) // If char is far enough from the right of the screen, print the char
        {
            char_width = vwf_list[(int)text[char_num]];
            total_length += char_width;
            img_pos = (text[char_num] >> 4) * fontnumber[number].width * grid_height;
            img_pos += (text[char_num] & 15) * grid_width;

            for (i = y; i < offset_y; i++)
            {
                for (j = shift_x; j < shift_x + char_width; j++)
                {
                    color = fontnumber[number].pixel[img_pos].c;
                    if (color != fontnumber[number].transparent)
                        pixel(j, i, color);
                    img_pos++;
                }
                img_pos += fontnumber[number].width - char_width;
            }
        }
        else char_num = strlen(text);    // Otherwise, cut off the remaining text
    }
}

void displayAllText() // Prints everything text-related on screen
{
    int i;
    for (i = 0; i < 3; i++)
        printtext(debug_line[i], FONT_DEBUG, 0, i * 8);
    for (i = 0; i < 20; i++)
        if (chat_mode || chat_timer[i] > 0.0f)
        {
            printtext(chat_line[i], FONT_CHAT, 0, i * 8 + 24);
            chat_timer[i] -= deltaTime;
        }
    if (chat_mode)
    {
        printtext(type_line, FONT_CHAT, 0, 200);
        if (typeCursorVisible)
        {
            typeXPosition_Screen = 0;
            for (i = 0; i < typeXPosition_Text; i++)  // This focuses on the chat text cursor
                typeXPosition_Screen += fontnumber_vwf[FONT_CHAT].charcode[type_line[i]];
            drawimage(imgnumber[7], typeXPosition_Screen, 200);
        }
    }
    snprintf(HPMaxString, 4, "%03d", playerHPMax);
    snprintf(HPCurrentString, 4, "%03d", playerHPCurrent);
    snprintf(AmmoLoadedString, 4, "%03d", playerAmmoLoaded);
    snprintf(AmmoUnloadedString, 4, "%03d", playerAmmoUnloaded);
    printtext(HPCurrentString, FONT_GUI, 32, 208);
    printtext(HPMaxString, FONT_GUI, 32, 224);
    printtext(AmmoLoadedString, FONT_GUI, 252, 208);
    printtext(AmmoUnloadedString, FONT_GUI, 252, 224);
}

void scrollChatUp()
{
    int i;
    for (i = 1; i < 20; i++)
    {
        strcpy(chat_line[i - 1], chat_line[i]);
        chat_timer[i] = chat_timer[i + 1];
    }
}

void free_fonts()
{
    int i = font_count;
    while (i != 0)
    {
        i--;
        free(fontnumber[i].pixel);
    }
    free(fontnumber);
    free(fontnumber_vwf);
}
