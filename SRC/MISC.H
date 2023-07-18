#ifndef MISC_H
#define MISC_H

#define sgn(x) ((x>0) - (x<0))

union REGS in, out;

bool endGameLoop;   // Detect if game has ended.
int errormessage;   // In case the game screws up, send an error message.
int mapNumber = 0;		// The map the player is in.
bool mapCycled = false;

const double PI = 3.1415926535897932384626433832795;
const double HALF_PI = 1.5707963267948966192313216916398;
const double PI180 = 0.01745329251994329576923690768489;		// Pi DIVIDED BY 180, not multiplied!
const double INVS_PI180 = 57.295779513082320876798154814105;	// 1 / (Pi / 180)

typedef struct
{
	float x, y, z;
} vec3;

vec3 playerPos = { 1.0f, 0.0f, 0.0f };
vec3 prevPlayerPos;
vec3 dummyTri[3] = { 0, 0, 2,
					-2, 0, -2,
					2, 0, -2 };
float camRotX, camRotY, camPosY, headPosY;

bool noclip = false, paused = false, chat_mode = false;
char mod_folder[9];

static inline void swapfloat(float* var1, float* var2);
static inline void swapthreefloats(float* var1, float* var2, float* var3);
static inline void swapint(int* var1, int* var2);
static inline float dot(vec3 a, vec3 b);
static inline void cross(float ax, float ay, float az, float bx, float by, float bz, float* cx, float* cy, float* cz);

#endif
