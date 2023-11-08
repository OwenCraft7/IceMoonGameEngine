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
            f_width = bit_imgwidth; f_size = bit_imgsize;
            bit_imgsize -= bit_imgwidth;
            bit_imgwidth -= 1;
            alpha_toggle = (tempimgTransparent == -1) ? 0.0f : 999.0f;
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

void texture_hline(const int y, int x1, int x2, float z, float u, float v) // Only draw a line given that Z, U, and V are inversed and interpolated.
{
    float i;
    int color;
    float *dist_pointer = dist_buffer[y];
    char *level_pointer = level_buffer[y];

    if (x1 < 0)
    {
        z -= lineSlope[0] * x1; u -= lineSlope[1] * x1; v -= lineSlope[2] * x1;
        x1 = 0;
    }
    if (x2 >= SCREEN_WIDTH) x2 = SCREEN_WIDTH - 1;
    while (x1 <= x2)
    {
        if (dist_pointer[x1] <= z)
        {
            i = 1.0f / z;
            color = tempimgdata[((int)(u * i) & bit_imgwidth) + ((int)(v * i) & bit_imgsize)].c;

            // Uncomment everything below in this if-statement if you want rendered triangles to support transparency and shading.

            // if (color != tempimgTransparent)
            // {
            // level_pointer[x1] = multiplyColor(color, luminance);
            level_pointer[x1] = color;
            dist_pointer[x1] = z;
            // }
        }
        z += lineSlope[0]; u += lineSlope[1]; v += lineSlope[2];
        x1++;
    }
}

void drawHalfTri(int startEdge, int y0, int *y1, int triHalf)//, int x1minx0)  // This function draws either the top or bottom half of a 2D triangle
{
    int i;	float j;
    int endL = (triSlope[1] < triSlope[0]) ? triHalf ^ 1 : triHalf;
    int endR = endL ^ 1;
    triEdge[1] = startEdge;
    if (y0 < 0)
    {
        for (i = 0; i < 8; i++)
            triEdge[i] -= triSlope[i] * y0;
        y0 = 0;
    }
    if (*y1 > SCREEN_HEIGHT) *y1 = SCREEN_HEIGHT;
	
	j = 1.0f / (triSlope[endR] - triSlope[endL]);
	lineSlope[0] = (triSlope[endR+2] - triSlope[endL+2]) * j;
	lineSlope[1] = (triSlope[endR+4] - triSlope[endL+4]) * j;
	lineSlope[2] = (triSlope[endR+6] - triSlope[endL+6]) * j;
	
    while (y0 < *y1)
    {
        texture_hline(y0, triEdge[endL], triEdge[endR], triEdge[endL+2], triEdge[endL+4], triEdge[endL+6]);
        for (i = 0; i < 8; i++)
            triEdge[i] += triSlope[i];
        y0++;
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
    if (y2 >= 0 && y0 < SCREEN_HEIGHT)    // If triangle is inside the screen's vertical boundaries
    {
        k = 1.0f / (y2 - y0);
        triSlope[0] = (x2 - x0) * k; triSlope[2] = (z2 - z0) * k;
        triSlope[4] = (u2 - u0) * k; triSlope[6] = (v2 - v0) * k;
        triEdge[0] = x0; triEdge[2] = z0; triEdge[4] = u0; triEdge[6] = v0;
        if (y0 != y1)
        {
            triEdge[3] = z0; triEdge[5] = u0; triEdge[7] = v0;
            if (y1 > -1)
            {
                k = 1.0f / (y1 - y0);
                triSlope[1] = (x1 - x0) * k; triSlope[3] = (z1 - z0) * k;
                triSlope[5] = (u1 - u0) * k; triSlope[7] = (v1 - v0) * k;
                drawHalfTri(x0, y0, &y1, TRI_TOP_HALF);
            }
            else for (i = 0; i < 8; i++)
                    triEdge[i] += triSlope[i] * (y1 - y0);
        }
        if (y1 < SCREEN_HEIGHT && y1 != y2)
        {
            triEdge[3] = z1; triEdge[5] = u1; triEdge[7] = v1;
            k = 1.0f / (y2 - y1);
            triSlope[1] = (x2 - x1) * k; triSlope[3] = (z2 - z1) * k;
            triSlope[5] = (u2 - u1) * k; triSlope[7] = (v2 - v1) * k;
            drawHalfTri(x1, y1, &y2, TRI_BOTTOM_HALF);
        }
    }
}

void texture_tri(const tri triangle, vert* sourceVert, uv* sourceUV, image* sourceImageArray)  // This function takes a 3D triangle and converts the vertices to 2D points on the screen
{
    int i, j, k, cx[4], cy[4];
    float pu[3], pv[3], cz[4], cu[4], cv[4];
    float slope, intercept, inverse, lightDot;
    vert normal, p[3], diff, edge[2], lightDir;

    // Precalculate sine and cosine of player rotation.
    const float cos_camx = cos(camRotX), cos_camy = cos(camRotY),
                sin_camx = sin(camRotX), sin_camy = sin(camRotY);
    
    int behindZ = 0; // "behindZ" tells us the number of vertices behind the near clipping plane.
    load_temptexture(sourceImageArray[triangle.texture]);   // Load triangle's texture

    for (i = 0; i < 3; i++) // For the three vertices:
    {
        // Get UV coordinates and multiply them with texture width and size respectively.
        // (V is multiplied with size to avoid calculating Width * Height = Size.)
        j = triangle.uv[i];
        pu[i] = sourceUV[j].u * f_width - 0.001f;    // Subtract U and V by 0.001 to prevent a UV looping artifact.
        pv[i] = sourceUV[j].v * f_size - 0.001f;
        
        // Subtract the vertex's position by the camera's position so that the camera is at the origin point.
        j = triangle.v[i];
        diff.x = sourceVert[j].x - playerPos.x;
        diff.y = sourceVert[j].y - camPosY;
        diff.z = sourceVert[j].z - playerPos.z;

        // Rotate the camera's yaw to 0. Rotate the vertex along with the camera.
        p[i].x = (cos_camx * diff.x) - (sin_camx * diff.z);
        diff.z = (sin_camx * diff.x) + (cos_camx * diff.z);

        // Rotate the camera's pitch to 0. Rotate the vertex along with the camera.
        p[i].y = (cos_camy * diff.y) - (sin_camy * diff.z);
        p[i].z = (sin_camy * diff.y) + (cos_camy * diff.z);

        if (p[i].z < NEAR_CLIP) // If the vertex is behind the near clipping plane,
            behindZ++;  // Increase behindZ by 1.
    }
    
    if (behindZ < 3)    // If at least one vertex is in front of the camera:
    {
        // Calculate surface normals
        edge[0].x = p[2].x - p[0].x;	edge[0].y = p[2].y - p[0].y;	edge[0].z = p[2].z - p[0].z;
        edge[1].x = p[1].x - p[0].x;	edge[1].y = p[1].y - p[0].y;	edge[1].z = p[1].z - p[0].z;
        cross(edge[0], edge[1], &normal);
        normalize(&normal);

        // If the surface normal is facing towards the camera:
        if (dot(normal, p[0]) < alpha_toggle)
        {
            lightDir.x = 0.0f; lightDir.y = 0.0f; lightDir.z = -1.0f;
            lightDot = -dot(normal, lightDir);

            if (lightDot < 0.25f)
                luminance = 255;
            else if (lightDot < 0.5f)
                luminance = 218;
            else if (lightDot < 0.75f)
                luminance = 182;
            else luminance = 145;

            if (behindZ == 0) // If no vertices are behind the camera, render it normally.
                for (i = 0; i < 3; i++) // For each vertex:
                {
                    inverse = 1.0f / p[i].z;
                    cz[i] = inverse;
                    cu[i] = pu[i] * inverse;
                    cv[i] = pv[i] * inverse;
                    inverse *= HALF_WIDTH;
                    cx[i] = p[i].x * inverse + HALF_WIDTH;  // Set X on screen
                    cy[i] = p[i].y * -inverse + HALF_HEIGHT;  // Set Y on screen
                    // Set its coordinates where it would normally go on the screen (0-319, 0-239).
                }
            else if (behindZ == 1)
            {
                j = k = 0;
                for (i = 0; i < 3; i++) // For each vertex:
                {
                    if (p[i].z >= NEAR_CLIP)    // If vertex is in front of camera...
                    {                           // Calculate its X and Y position as well as UV coordinates.
                        inverse = 1.0f / p[i].z;
                        cz[j] = inverse;
                        cu[j] = pu[i] * inverse;
                        cv[j] = pv[i] * inverse;
                        inverse *= HALF_WIDTH;
                        cx[j] = p[i].x * inverse + HALF_WIDTH;  // Set X on screen
                        cy[j] = p[i].y * -inverse + HALF_HEIGHT;  // Set Y on screen
                        j += 1;
                    }
                    else k = i; // K will equal the single vertex behind the camera.
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

                // Now that vertex #2 is the only one behind the camera, it is then
                // converted into two vertices that reside on any side of the screen.
                for (i = 2; i < 4; i++) // For vertices #2 and #3...
                {
                    j = 3 - i;

                    // Calculate Z difference inverse to avoid more than one division
                    inverse = 1.0f / (p[2].z - p[j].z);

                    // Calculate X position on screen
                    slope = (p[2].x - p[j].x) * inverse;
                    intercept = p[2].x - slope * p[2].z;
                    cx[i] = slope + intercept * HALFW_T_INVS_NC;
                    cx[i] += HALF_WIDTH;

                    // Calculate Y position on screen
                    slope = (p[2].y - p[j].y) * inverse;
                    intercept = p[2].y - slope * p[2].z;
                    cy[i] = slope + intercept * -HALFW_T_INVS_NC;
                    cy[i] += HALF_HEIGHT;

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
                        inverse *= HALF_WIDTH;
                        cx[i] = p[i].x * inverse + HALF_WIDTH;  // Set X on screen
                        cy[i] = p[i].y * -inverse + HALF_HEIGHT;  // Set Y on screen
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
                        cx[i] = slope + intercept * HALFW_T_INVS_NC;
                        cx[i] += HALF_WIDTH;

                        // Calculate Y position on screen
                        slope = (p[j].y - p[i].y) * inverse;
                        intercept = p[j].y - slope * p[j].z;
                        cy[i] = slope + intercept * -HALFW_T_INVS_NC;
                        cy[i] += HALF_HEIGHT;

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
