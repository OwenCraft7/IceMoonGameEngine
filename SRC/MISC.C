/*---------------------*/
/*   Ice Moon misc.c   */
/*                     */
/*    Miscellaneous    */
/*---------------------*/

#include "MISC.H"

int roundpowtwo(int value) // This function doesn't expect "value" to go higher than 65535.
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value++;
    return value >> 1;
}

static void swapfloat(float *var1, float *var2)
{
    float temp = *var1;
    *var1 = *var2;
    *var2 = temp;
}

static void swapthreefloats(float* var1, float* var2, float* var3)
{
    float temp = *var1;
    *var1 = *var2;
    *var2 = *var3;
    *var3 = temp;
}

static void swapint(int* var1, int* var2)
{
    int temp = *var1;
    *var1 = *var2;
    *var2 = temp;
}

static void fgets_noNewline(char* buffer, int maxCount, FILE* stream)
{
    fgets(buffer, maxCount, stream);
    buffer[strcspn(buffer, "\n")] = 0;
}

float invsqrt(float var)
{
    float f = var;
    long i = *(long*)&f;
    i = 0x5f1ffff9 - (i >> 1);
    f = *(float*)&i;
    return 0.703952253f * f * (2.38924456f - var * f * f);
}

float distance(vert a, vert b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

static float dot(vert a, vert b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static void cross(float ax, float ay, float az, float bx, float by, float bz, vert *c)
{
    c->x = ay * bz - az * by;
    c->y = az * bx - ax * bz;
    c->z = ax * by - ay * bx;
}

void normalize(vert* Vector)
{
    float inverseLength = invsqrt(Vector->x * Vector->x + Vector->y * Vector->y + Vector->z * Vector->z);
    Vector->x *= inverseLength;
    Vector->y *= inverseLength;
    Vector->z *= inverseLength;
}

void loadCfg()
{
    FILE* tempcfg;

    if (!endGameLoop && (tempcfg = fopen("CONFIG.CFG", "r")) != NULL)
    {
        fgets_noNewline(mod_folder, 9, tempcfg);
        fgets(player_name, 21, tempcfg);
        playerNameLengthPlusTwo = strlen(player_name) + 2;
    }
    else
    {
        endGameLoop = true;
        errormessage = 2;
    }
    fclose(tempcfg);
}
