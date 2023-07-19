/*   Ice Moon time.c   */
/*                     */
/*   Calculates time   */

#include "TIME.H"

void updateDeltaTime() // Updates the delta time every frame
{
    ftime(&delt2);
    deltaTime = (float)(delt2.time - delt1.time) + ((delt2.millitm - delt1.millitm) * 0.001f);
    delt1 = delt2;
}

void updateFPS()    // Updates the FPS every quarter second.
{
    seconds_quarterSecondUpdate += 0.25f;
    framesPerSecond = (frames - frames_quarterSecondUpdate) * 4;
    frames_quarterSecondUpdate = frames;
    typeCursorVisible = !typeCursorVisible; // The little vertical flashing line in the text toggles visibility every FPS update.
}
