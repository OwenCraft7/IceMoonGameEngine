/*--------------------------*/
/*    Ice Moon texture.c    */
/*                          */
/* Draws textured triangles */
/*        and stuff.        */
/*--------------------------*/

#include "TEXTURE.H"

void load_temptexture(image sourceImage)    // Texture MUST be a power of two. Otherwise, the image will be screwed up.
{
    if (!endGameLoop)
    {
        if ((tempimgdata = sourceImage.pixel) != NULL)
        {
            tempimgTransparent = sourceImage.transparent;
            bit_imgwidth = roundpowtwo(sourceImage.width);
            bit_imgsize = bit_imgwidth * roundpowtwo(sourceImage.height);
            f_width = bit_imgwidth;  f_size = bit_imgsize;
            bit_imgsize -= bit_imgwidth;
            bit_imgwidth -= 1;
            if (tempimgTransparent == -1)
                alpha_toggle = 0.0f;
            else
                alpha_toggle = 999.0f;
        }
        else
        {
            tempimgTransparent = -1;
            tempimgdata = imgnumber[0].pixel;
            bit_imgwidth = 7; bit_imgsize = 56;
            f_width = 8; f_size = 64;
            alpha_toggle = 0.0f;
        }
    }
}

void texture_hline(const int y, int x1, int x2, float z1, float z2, float u1, float u2, float v1, float v2) // Only draw a line given that Z, U, and V are inversed and interpolated.
{
    float slope[3], i;
    int color;
    float *dist_pointer = dist_buffer[y];
    char *level_pointer = level_buffer[y];
    
    if (x1 < 320 && x2 > -1)
    {
        i = 1.0f / (x2 - x1);
        slope[0] = (z2 - z1) * i; slope[1] = (u2 - u1) * i; slope[2] = (v2 - v1) * i;

        if (x1 < 0)
        {
            z1 -= slope[0] * x1; u1 -= slope[1] * x1; v1 -= slope[2] * x1;
            x1 = 0;
        }
        if (x2 > 319) x2 = 319;

        while (x1 <= x2)
        {
            i = 1.0f / z1;
            if (dist_pointer[x1] >= i || skipDistBuffer)
            {
                color = tempimgdata[((int)(u1 * i) & bit_imgwidth) + ((int)(v1 * i) & bit_imgsize)].c;
                if (color != tempimgTransparent)
                {
                    level_pointer[x1] = multiplyColor(color, luminance);  // Draw pixel in the level buffer
                    dist_pointer[x1] = i;       // Set distance in a specific pixel in the distance buffer
                }
            }
            z1 += slope[0]; u1 += slope[1]; v1 += slope[2];
            x1++;
        }
    }
}

void bresenham(int adx, int ady, int sdx, int *x, float *side)  // This function calculates the left and right edges for a 2D triangle
{
    *x += adx;
    while (*x >= ady)
    {
        *x -= ady;
        *side += sdx;
    }
}

void drawHalfTri(int startEdge, int y0, int *y1, int triHalf, int x1minx0)    // This function draws either the top or bottom half of a 2D triangle
{
    int i, j;
    adx[1] = abs(x1minx0); sdx[1] = sgn(x1minx0); x[1] = 0;
    reversed = (slope[0][1] < slope[0][0]) ? triHalf ^ 1 : triHalf;
    edge[0][1] = startEdge;
    if (y0 < 0)
    {
        for (i = 0; i < 8; i++)
            edge[0][i] -= slope[0][i] * y0;
        y0 = 0;
    }
    if (*y1 > 240) *y1 = 240;
    for (i = y0; i < *y1; i++)
    {
        texture_hline(i, edge[0][reversed], edge[0][reversed ^ 1], edge[1][reversed], edge[1][reversed ^ 1], edge[2][reversed], edge[2][reversed ^ 1], edge[3][reversed], edge[3][reversed ^ 1]);
        bresenham(adx[0], ady[0], sdx[0], &x[0], &edge[0][0]);
        bresenham(adx[1], ady[1], sdx[1], &x[1], &edge[0][1]);
        for (j = 0; j < 6; j++)
            edge[1][j] += slope[1][j];
    }
}

void texture_twodimtri(int x0, int x1, int x2, int y0, int y1, int y2, float z0, float z1, float z2, float u0, float u1, float u2, float v0, float v1, float v2)    // This function sets up the process to draw a 2D triangle on screen
{
    int i;
    float k;
    // Sort each vertex by y value
    if (y0 > y1)
    {
        swapint(&x0, &x1); swapint(&y0, &y1);
        swapfloat(&z0, &z1); swapfloat(&u0, &u1); swapfloat(&v0, &v1);
    }
    if (y0 > y2)
    {
        swapint(&x0, &x2); swapint(&y0, &y2);
        swapfloat(&z0, &z2); swapfloat(&u0, &u2); swapfloat(&v0, &v2);
    }
    if (y1 > y2)
    {
        swapint(&x1, &x2); swapint(&y1, &y2);
        swapfloat(&z1, &z2); swapfloat(&u1, &u2); swapfloat(&v1, &v2);
    }
    if (y2 > -1 && y0 < 240)    // If triangle is inside screen
    {
        i = x2 - x0; adx[0] = abs(i); sdx[0] = sgn(i);
        ady[0] = y2 - y0; x[0] = 0; k = 1.0f / ady[0];
        slope[0][0] = (x2 - x0) * k; slope[1][0] = (z2 - z0) * k;
        slope[2][0] = (u2 - u0) * k; slope[3][0] = (v2 - v0) * k;
        edge[0][0] = x0; edge[1][0] = z0; edge[2][0] = u0; edge[3][0] = v0;
        if (y0 != y1)
        {
            ady[1] = y1 - y0;
            edge[1][1] = z0; edge[2][1] = u0; edge[3][1] = v0;
            if (y1 > -1)
            {
                k = 1.0f / ady[1];
                slope[0][1] = (x1 - x0) * k; slope[1][1] = (z1 - z0) * k;
                slope[2][1] = (u1 - u0) * k; slope[3][1] = (v1 - v0) * k;
                drawHalfTri(x0, y0, &y1, TRI_TOP_HALF, x1 - x0);
            }
            else
                for (i = 0; i < 8; i++)
                    edge[0][i] += slope[0][i] * ady[1];
        }
        if (y1 < 240 && y1 != y2)
        {
            ady[1] = y2 - y1;
            edge[1][1] = z1; edge[2][1] = u1; edge[3][1] = v1;
            k = 1.0f / ady[1];
            slope[0][1] = (x2 - x1) * k; slope[1][1] = (z2 - z1) * k;
            slope[2][1] = (u2 - u1) * k; slope[3][1] = (v2 - v1) * k;
            drawHalfTri(x1, y1, &y2, TRI_BOTTOM_HALF, x2 - x1);
        }
    }
}

void texture_tri(const tri triangle, vert* sourceVert, uv* sourceUV, image* sourceImageArray, bool ignoreDistanceBuffer)  // This function takes a 3D triangle and converts the vertices to 2D points on the screen
{
    int i, j, k, cx[4], cy[4];
    float pu[3], pv[3], cz[4], cu[4], cv[4];
    float slope, intercept, inverse, n_vector;
    vert normal, p[3], diff;

    // Precalculate sine and cosine of player rotation.
    const float cos_camx = cos(-camRotX);
    const float cos_camy = cos(camRotY);
    const float sin_camx = sin(-camRotX);
    const float sin_camy = sin(camRotY);
    
    int behindZ = 0; // "behindZ" tells us the number of vertices behind the near clipping plane.

    skipDistBuffer = ignoreDistanceBuffer;

    for (i = 0; i < 3; i++) // For the three vertices:
    {
        // Get UV coordinates and multiply them with texture width and size respectively.
        // (V is multiplied with size to avoid calculating Width * Height = Size.)
        j = triangle.uv[i];
        pu[i] = sourceUV[j].u * f_width;
        pv[i] = sourceUV[j].v * f_size;
        
        // Subtract the vertex's position by the camera's position so that the camera is at the origin point.
        j = triangle.v[i];
        diff.x = sourceVert[j].x - playerPos.x;
        diff.y = sourceVert[j].y - camPosY;
        diff.z = sourceVert[j].z - playerPos.z;

        // Rotate the camera's yaw to 0. Rotate the vertex along with the camera.
        p[i].x = (cos_camx * diff.x) + (sin_camx * diff.z);
        diff.z = (-sin_camx * diff.x) + (cos_camx * diff.z);

        // Rotate the camera's pitch to 0. Rotate the vertex along with the camera.
        p[i].y = (cos_camy * diff.y) + (-sin_camy * diff.z);
        p[i].z = (sin_camy * diff.y) + (cos_camy * diff.z);

        if (p[i].z < NEAR_CLIP) // If the vertex is behind the near clipping plane,
            behindZ++;  // Increase behindZ by 1.
    }
    
    if (behindZ < 3)    // If at least one vertex is in front of the camera:
    {
        // Calculate surface normals
        cross(p[2].x - p[0].x, p[2].y - p[0].y, p[2].z - p[0].z, p[1].x - p[0].x, p[1].y - p[0].y, p[1].z - p[0].z, &normal);
        normalize(&normal);

        n_vector = dot(normal, p[0]);

        // Load triangle's texture
        load_temptexture(sourceImageArray[triangle.texture]);

        // If the surface normal is facing towards the camera:
        if (n_vector < alpha_toggle)
        {
            if (normal.z < 0.01f)
                luminance = 255;
            else if (normal.z < 0.1f)
                luminance = 218;
            else if (normal.z < 0.25f)
                luminance = 182;
            else luminance = 145;

            if (behindZ == 0) // If no vertices are behind the camera, render it normally.
                for (i = 0; i < 3; i++) // For each vertex:
                {
                    inverse = 1.0f / p[i].z;
                    cz[i] = inverse;
                    cu[i] = pu[i] * inverse;
                    cv[i] = pv[i] * inverse;
                    inverse *= 160.0f;
                    cx[i] = p[i].x * inverse + 160;  // Set X on screen
                    cy[i] = p[i].y * -inverse + 120;  // Set Y on screen
                    // Set its coordinates where it would normally go on the screen (0-319, 0-239).
                }
            else if (behindZ == 1)
            {
                j = k = 0;
                for (i = 0; i < 3; i++) // For each vertex:
                {
                    if (p[i].z >= NEAR_CLIP)
                    {
                        inverse = 1.0f / p[i].z;
                        cz[j] = inverse;
                        cu[j] = pu[i] * inverse;
                        cv[j] = pv[i] * inverse;
                        inverse *= 160.0f;
                        cx[j] = p[i].x * inverse + 160;  // Set X on screen
                        cy[j] = p[i].y * -inverse + 120;  // Set Y on screen
                        j += 1;
                    }
                    else
                        k = i;
                }
                if (k == 0)
                {
                    swapthreefloats(&p[0].x, &p[1].x, &p[2].x);
                    swapthreefloats(&p[0].y, &p[1].y, &p[2].y);
                    swapthreefloats(&p[0].z, &p[1].z, &p[2].z);
                    swapthreefloats(&pu[0], &pu[1], &pu[2]);
                    swapthreefloats(&pv[0], &pv[1], &pv[2]);
                }
                else if (k == 1)
                {
                    swapfloat(&p[1].x, &p[2].x);
                    swapfloat(&p[1].y, &p[2].y);
                    swapfloat(&p[1].z, &p[2].z);
                    swapfloat(&pu[1], &pu[2]);
                    swapfloat(&pv[1], &pv[2]);
                }

                for (i = 2; i < 4; i++)
                {
                    j = 3 - i;

                    // Calculate Z difference inverse to avoid more than one division
                    inverse = 1.0f / (p[2].z - p[j].z);

                    // Calculate X position on screen
                    slope = (p[2].x - p[j].x) * inverse;
                    intercept = p[2].x - slope * p[2].z;
                    cx[i] = slope + intercept * 16000.0f;
                    cx[i] += 160;

                    // Calculate Y position on screen
                    slope = (p[2].y - p[j].y) * inverse;
                    intercept = p[2].y - slope * p[2].z;
                    cy[i] = slope + intercept * -16000.0f;
                    cy[i] += 120;

                    // Calculate U texture coordinate
                    slope = (pu[2] - pu[j]) * inverse;
                    intercept = pu[2] - slope * p[2].z;
                    cu[i] = slope + intercept * INVS_NEAR_CLIP;

                    // Calculate V texture coordinate
                    slope = (pv[2] - pv[j]) * inverse;
                    intercept = pv[2] - slope * p[2].z;
                    cv[i] = slope + intercept * INVS_NEAR_CLIP;

                    cz[i] = INVS_NEAR_CLIP;

                    j = i - 1;

                    texture_twodimtri(cx[0], cx[j], cx[i], cy[0], cy[j], cy[i], cz[0], cz[j], INVS_NEAR_CLIP, cu[0], cu[j], cu[i], cv[0], cv[j], cv[i]);
                }
            }
            else // If two vertices are behind the camera.
            {
                for (i = 0; i < 3; i++) // For each vertex:
                    if (p[i].z >= NEAR_CLIP) // If the vertex is in front of the near clipping plane, (only one will be in front)
                    {
                        inverse = 1.0f / p[i].z;
                        cz[i] = inverse;
                        cu[i] = pu[i] * inverse;
                        cv[i] = pv[i] * inverse;
                        inverse *= 160.0f;
                        cx[i] = p[i].x * inverse + 160;  // Set X on screen
                        cy[i] = p[i].y * -inverse + 120;  // Set Y on screen
                        j = i; // j = the vertex number that's in front.
                    }
                for (i = 0; i < 3; i++) // For each vertex:
                    if (i != j) // If the vertex is behind the camera,
                    {
                        // Move the vertex to Z position 0, on a line between its original position and the vertex in front of the camera.

                        // Calculate Z difference inverse to avoid more than one division.
                        inverse = 1.0f / (p[j].z - p[i].z);

                        // Calculate X position on screen
                        slope = (p[j].x - p[i].x) * inverse;
                        intercept = p[j].x - slope * p[j].z;
                        cx[i] = slope + intercept * 16000.0f;
                        cx[i] += 160;

                        // Calculate Y position on screen
                        slope = (p[j].y - p[i].y) * inverse;
                        intercept = p[j].y - slope * p[j].z;
                        cy[i] = slope + intercept * -16000.0f;
                        cy[i] += 120;

                        // Calculate U texture coordinate
                        slope = (pu[j] - pu[i]) * inverse;
                        intercept = pu[j] - slope * p[j].z;
                        cu[i] = slope + intercept * INVS_NEAR_CLIP;

                        // Calculate V texture coordinate
                        slope = (pv[j] - pv[i]) * inverse;
                        intercept = pv[j] - slope * p[j].z;
                        cv[i] = slope + intercept * INVS_NEAR_CLIP;

                        cz[i] = INVS_NEAR_CLIP;
                    }
            }

            if (behindZ != 1) // If behindZ doesn't equal 1, render one triangle.
                texture_twodimtri(cx[0], cx[1], cx[2], cy[0], cy[1], cy[2], cz[0], cz[1], cz[2], cu[0], cu[1], cu[2], cv[0], cv[1], cv[2]);
        }
    }
}

void free_textures()
{
    int i = texture_count;
    while (i != 0)
    {
        i--;
        free(map_texture[i].pixel);
    }
    free(map_texture);
}
