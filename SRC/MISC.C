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

float invsqrt(float var)
{
    long i; float f = var;
    i = *(long*)&f;
    i = 0x5f1ffff9 - (i >> 1);
    f = *(float*)&i;
    return 0.703952253f * f * (2.38924456f - var * f * f);
}

static float dot(vec3 a, vec3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}


static void cross(float ax, float ay, float az, float bx, float by, float bz, float *cx, float *cy, float *cz)
{
    *cx = ay * bz - az * by;
    *cy = az * bx - ax * bz;
    *cz = ax * by - ay * bx;
}

void loadCfg()
{
    FILE* tempcfg;

    if (!endGameLoop && (tempcfg = fopen("CONFIG.CFG", "r")) != NULL)
    {
        fgets(mod_folder, 9, tempcfg);
    }
    else
    {
        endGameLoop = true;
        errormessage = 2;
    }
    fclose(tempcfg);
}
