#ifndef TIME_H
#define TIME_H

clock_t clockCycles, lastClockUpdate;

int frames, framesPerSecond, framesUpdate = 0;
float seconds, deltaTime, secondsUpdate = 1.0f;
float inverseClockSpeed;

static inline void updateDeltaTime();
static inline void updateFPS();

#endif
