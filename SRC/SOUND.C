/*    Ice Moon sound.c   */
/*                       */
/* Handles SFX and music */

#include "SOUND.H"

int reset_dsp(short port)
{
    outp(port + SB_RESET, 1);
    delay(10);
    outp(port + SB_RESET, 0);
    delay(10);
    if ((inp(port + SB_READ_DATA_STATUS) & 0x80) == 0x80 && inp(port + SB_READ_DATA) == 0xAA)
    {
        sb_base = port;
        return 1;
    }
    return 0;
}

void write_dsp(unsigned char command)
{
    while ((inp(sb_base + SB_WRITE_DATA) & 0x80) == 0x80);
    outp(sb_base + SB_WRITE_DATA, command);
}

int sb_detect()
{
    int temp;
    char* BLASTER;
    sb_base = 0;

    /* possible values: 210, 220, 230, 240, 250, 260, 280 */
    for (temp = 1; temp < 9; temp++)
        if (temp != 7)
            if (reset_dsp(0x200 + (temp << 4)))
                break;
    if (temp == 9)
        return 0;

    BLASTER = getenv("BLASTER");
    sb_dma = 0;
    for (temp = 0; temp < strlen(BLASTER); ++temp)
        if ((BLASTER[temp] | 32) == 'd')
            sb_dma = BLASTER[temp + 1] - '0';

    for (temp = 0; temp < strlen(BLASTER); ++temp)
        if ((BLASTER[temp] | 32) == 'i')
        {
            sb_irq = BLASTER[temp + 1] - '0';
            if (BLASTER[temp + 2] != ' ')
                sb_irq = sb_irq * 10 + BLASTER[temp + 2] - '0';
        }

    return sb_base != 0;
}
