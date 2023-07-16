#ifndef TIME_H
#define TIME_H

struct timeb delt1, delt2;

int frames, framesPerSecond, frames_quarterSecondUpdate = 0;
float seconds, seconds_quarterSecondUpdate = 0.25f;
float deltaTime;

#endif
