#ifndef TEXTURE_H
#define TEXTURE_H

const float NEAR_CLIP = 0.01f;
const float INVS_NEAR_CLIP = 100.0f;
const float HALFW_T_INVS_NC = 16000.0f;	// Half Screen Width (160px) times inverse near clip

int luminance;
float alpha_toggle;  // If texture has transparency or translucency, disable backface culling by setting alpha_toggle to 999.0f.
int bit_imgwidth, bit_imgsize;
float f_width, f_size;

int reversed, notReversed;
float triSlope[8], triEdge[8], lineSlope[3];

enum triHalves
{
	TRI_TOP_HALF = 0,
	TRI_BOTTOM_HALF = 1
};

#endif
