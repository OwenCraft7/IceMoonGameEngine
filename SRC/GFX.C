/*------------------------*/
/*     Ice Moon gfx.c     */
/*                        */
/*   Core graphics code   */
/*------------------------*/
/*
	Some code is copyrighted by David Brackeen, licensed under a Creative Commons Attribution Share Alike 2.5 Generic License.
	The code comes from his 256 color VGA tutorial, specifically "unchain.c" and "modes.c".
	I have modified his code, and distributed my contributions under the same license.
	http://www.brackeen.com/vga/unchain.html
	https://www.creativecommons.org/licenses/by-sa/2.5/legalcode
*/

#include "GFX.H"

static void videoMode(uint8_t mode)
{
    _asm
    {
        mov ah, 0x00
        mov al, mode
        int 10h
    }
}

void modeX()
{
    videoMode(0x13);

    // Turn off chain-4 mode
    outp(SC_INDEX, MEMORY_MODE);
    outp(SC_DATA, 0x06);
    // Turn off long mode
    outp(CRTC_INDEX, UNDERLINE_LOC);
    outp(CRTC_DATA, 0x00);
    // Turn on byte mode
    outp(CRTC_INDEX, MODE_CONTROL);
    outp(CRTC_DATA, 0xe3);
    // Clear VGA memory
    outp(SC_INDEX, MAP_MASK);
    outp(SC_DATA, 0xFF);

    // Turn off write protect
    outpw(CRTC_INDEX, 0x2c00 + V_RETRACE_END);
    outpw(CRTC_INDEX, 0x0d00 + V_TOTAL);
    outpw(CRTC_INDEX, 0x3e07);
    outpw(CRTC_INDEX, 0xea00 + V_RETRACE_START);
    outpw(CRTC_INDEX, 0xac00 + V_RETRACE_END);
    outpw(CRTC_INDEX, 0xdf00 + V_DISPLAY_END);
    outpw(CRTC_INDEX, 0xe700 + V_BLANK_START);
    outpw(CRTC_INDEX, 0x0600 + V_BLANK_END);
}

void flip()
{
    while (inp(INPUT_STATUS) & VRETRACE);
    outpw(CRTC_INDEX, HIGH_ADDRESS | (nonVisible & 0xff00));
    outpw(CRTC_INDEX, LOW_ADDRESS | ((nonVisible << 8) & 0xffff));
    while (!(inp(INPUT_STATUS) & VRETRACE));

    nonVisible ^= 19200;
}

void palette(const int r_multi, const int g_multi, const int b_multi)
{
    int r, g;
    int r_limit = r_multi << 3;
    int g_limit = g_multi << 3;

    outp(0x03c8, 0);
    for (r = 0; r < r_limit; r += r_multi)
        for (g = 0; g < g_limit; g += g_multi)
        {
            outp(0x03c9, r); outp(0x03c9, g); outp(0x03c9, 0);
            outp(0x03c9, r); outp(0x03c9, g); outp(0x03c9, b_multi);
            outp(0x03c9, r); outp(0x03c9, g); outp(0x03c9, b_multi << 1);
            outp(0x03c9, r); outp(0x03c9, g); outp(0x03c9, b_multi * 3);
        }
}

static void pixel(const int x, const int y, const char c)
{
    col_buffer[y][x] = c;
}

static char blendColor(const char oneThirdBlend, const char twoThirdBlend)
{
    return blend_map[oneThirdBlend + (twoThirdBlend << 8)].c;
}

static char multiplyColor(const char textureColor, const char lightColor)
{
    return multiply_map[textureColor + (lightColor << 8)].c;
}

void loadimage(const char* file, image* pointer)    // Only supports 256-color bitmaps, but will NOT work on bitmaps with exactly 256 colors.
{
    int i, j, width, height;
    int colvar;
    FILE *tempimage;
    color temppal[256];
    char filepath[32];

    sprintf(filepath, "%s/%s", mod_folder, file);
    
    if (!endGameLoop && (tempimage = fopen(filepath, "rb")) != NULL)
    {
        fseek(tempimage, 0, SEEK_SET);  // Go to start of file

        // If the program didn't read "BM", the bitmap's magic number, create an error image.
        if (fgetc(tempimage) != 'B' || fgetc(tempimage) != 'M')
            goto IMPROVISE;

        fseek(tempimage, 18, SEEK_SET);
        fread(&pointer->width, 4, 1, tempimage);  // Get image width
        fread(&pointer->height, 4, 1, tempimage); // Get image height
        fseek(tempimage, 46, SEEK_SET);
        fread(&colvar, 4, 1, tempimage);    // Number of colors in image (forms an image color palette separate from game palette)
        fseek(tempimage, 54, SEEK_SET);

        width = pointer->width; height = pointer->height;
        free(pointer->pixel);
        if ((pointer->pixel = calloc(width * height, sizeof(color))) != NULL)    // Allocate pixel data
        {
            for (i = 0; i < colvar; i++)    // For the entire image color palette:
            {
                temppal[i].c = fgetc(tempimage) >> 6;   // Get blue
                temppal[i].c += (fgetc(tempimage) & 0xE0) >> 3;   // Get green
                temppal[i].c += fgetc(tempimage) & 0xE0;  // Get red
                fgetc(tempimage);   // Get junk variable
            }
            if (temppal[0].c == 0)
                pointer->transparent = 0;   // Set first color in image palette to transparent if its black.
            else pointer->transparent = -1;
            tempimgdata = pointer->pixel; // Reference to ingame image memory
            for (i = (height - 1) * width; i >= 0; i -= width)   // From image bottom to top
            {
                for (j = 0; j < width; j++) // For every pixel
                {
                    fread(&colvar, 1, 1, tempimage);
                    tempimgdata[i + j].c = temppal[colvar].c; // Set color
                }
            }
        }
        else goto IMPROVISE;    // If image fails to allocate pixels to RAM, create an error image.
    }
    else goto IMPROVISE;    // If we can't find our image, then we'll have to make an error image.

    goto SKIP;
    IMPROVISE:
    pointer->width = 8; pointer->height = 8; // It will be 8 * 8 pixels
    free(pointer->pixel);
    if ((pointer->pixel = calloc(64, sizeof(color))) != NULL)    // 8 * 8 pixels = 64
    {
        tempimgdata = pointer->pixel; // Reference to ingame image memory
        pointer->transparent = -1;
        colvar = 3;
        for (i = 0; i < 8; i++)
        {
            for (j = (i << 3); j < (i << 3) + 8; j++)    // The checkerboard pattern is hardcoded into the game.
            {
                tempimgdata[j].c = colvar;  // "colvar" starts out as 3. This is indicated as blue in the color palette.
                colvar ^= 28;  // XOR colvar to toggle between blue and cyan.
            }
            colvar ^= 28;
        }
    }
    else if (!endGameLoop)   // If image failed to allocate pixels to RAM, send error message #1.
    {
        endGameLoop = true;
        errormessage = 1;
    }

    SKIP:
    fclose(tempimage);  // Close image
}

void addimage(const char* file) // Appends an image to RAM.
{
    img_count++; // Add the amount of images in RAM
    imgnumber = realloc(imgnumber, sizeof(image) * img_count);   // Reallocate memory for images to be loaded

    loadimage(file, &imgnumber[img_count - 1]);  // Load the image like normal
    /*  The benefit for using addimage() is to avoid typing a predefined amount of images.
     
        The downside is if you use it repeatedly, you should just predefine the image count instead
        and use loadimage() because it saves CPU time.  */
}

void replaceimage(const char* file, image* pointer) // Replaces images that are no longer in use.
{
    free(pointer->pixel);   // Free both the image's pixels
    free(pointer);          // and the misc. data too

    loadimage(file, pointer);  // Replace the deleted image with the current one.
}

void drawimage(const image img, const int x, const int y)   // Draws an image into the color buffer. Do this AFTER the 3D rendering.
{
    int i, j;
    int img_pos = 0, difference = 0;
    int leftSide, rightSide, bottomSide;
    // leftSide: Left side image, rightSide: Right side image, bottomSide: Bottom side image
    rightSide = img.width + x; bottomSide = img.height + y;
    // Restrict offsets inside the screen
    if (rightSide > 320) {difference = rightSide - 320; rightSide = 320;}
    if (bottomSide > 240) bottomSide = 240;
    if (x < 0) {leftSide = 0; difference -= x; img_pos -= x;}
    else leftSide = x;

    img_pos += -(y < 0) & (img.width * -y);  // Skip part of the image if it's somewhat above screen

    for (i = -(y > -1) & y; i < bottomSide; i++)  // Ensure the image isn't drawn above or below the screen
    {
        for (j = leftSide; j < rightSide; j++)
        {
            if (img.pixel[img_pos].c != img.transparent)
                pixel(j, i, img.pixel[img_pos].c);
            img_pos++;
        }
        img_pos += difference;
    }
}

/*void drawMemory(int page)   // Draws the game's RAM on screen in 1-byte pixels
{
    int x, y;
    int *i;
    i = page << 10;
    for (y = 56; y < 168; y++)
        for (x = 32; x < 288; x++)
        {
            pixel(x, y, (char)*i);
            i++;
        }
}*/

void free_images()
{
    int i;
    for (i = img_count - 1; i >= 0; i--)
        free(imgnumber[i].pixel);
    free(imgnumber);
    free(tempimgdata);
    free(blend_map);
    free(multiply_map);
}
