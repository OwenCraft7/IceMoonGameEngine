/*----------------------*/
/*   Ice Moon mouse.c   */
/*                      */
/*    Mouse function    */
/*----------------------*/

#include "MOUSE.H"

static int mouseInit(void)	// Initialize the mouse
{
	in.w.ax = 0;
	int386(0x33, &in, &out);
	return out.w.ax;
}

void mouseStatus(int *xp, int *yp, int *clk)	// Set variables to Mouse X, Y, and button clicked.
{
	in.w.ax = 3;
	int386(0x33, &in, &out);
	*clk = out.w.bx;
	*xp = out.w.cx >> 1;
	*yp = out.w.dx;

	mouseLeftPressed = (!mouseLeftDown && *clk & 1) ? true : false;
	mouseRightPressed = (!mouseRightDown && *clk & 2) ? true : false;

	mouseLeftDown = (*clk & 1) ? true : false;
	mouseRightDown = (*clk & 2) ? true : false;
}

void mousePos(int xp, int yp)	// Set mouse position
{
	in.w.ax = 4;
	in.w.cx = xp << 1;
	in.w.dx = yp;
	int386(0x33, &in, &out);
	mouseX = xp; mouseY = yp;
}

void mouseLimits(int xmin, int xmax, int ymin, int ymax)	// Limit the mouse to certain coordinates
{
	in.w.ax = 7;
	in.w.cx = xmin << 1;
	in.w.dx = xmax << 1;
	int386(0x33, &in, &out);
	in.w.ax = 8;
	in.w.cx = ymin;
	in.w.dx = ymax;
	int386(0x33, &in, &out);
}
