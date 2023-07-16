#ifndef TEXTURE_H
#define TEXTURE_H

const float NEAR_CLIP = 0.01f;
const float INVS_NEAR_CLIP = 100.0f;

int luminance;
float alpha_toggle;  // If texture has transparency or translucency, disable backface culling by setting alpha_toggle to 999.0f.
int bit_imgwidth, bit_imgsize;
float f_width, f_size;

#endif
