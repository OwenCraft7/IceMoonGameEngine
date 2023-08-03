/*   Ice Moon time.c   */
/*                     */
/*   Calculates time   */

#include "TIME.H"

static void updateDeltaTime()   // Updates the delta time every frame
{
    clockCycles = clock() - lastClockUpdate;
    lastClockUpdate += clockCycles;
    deltaTime = (float)clockCycles / (float)CLOCKS_PER_SEC;
}

static void updateFPS()    // Update the FPS every second.
{
    secondsUpdate += 1.0f;
    framesPerSecond = frames - framesUpdate;
    framesUpdate = frames;
    typeCursorVisible = !typeCursorVisible; // The little vertical flashing line in the text toggles visibility every FPS update.
}
