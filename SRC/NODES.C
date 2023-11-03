/*----------------------*/
/*   Ice Moon nodes.c   */
/*                      */
/* Draws the leafnodes. */
/*----------------------*/

#include "NODES.H"

void drawleaf(const int select)
{
    int i = select + 1;
    int end = (i != leaf_count) ? map_leaf[i] : tri_count;
    for (i = map_leaf[select]; i < end; i++)   // Keep in mind, triangles are sorted by leaf number
        texture_tri(map_tri[i], map_vert, map_uv, map_texture);
}

static void checkfront(const node nod)
{
    if (nod.f_leaf == 0) checknode(map_node[nod.front]);
    else
    {
        if (leafCheck == -1) leafCheck = nod.front;
        if (renderLevel) drawleaf(nod.front);
    }
}
static void checkback(const node nod)
{
    if (nod.b_leaf == 0) checknode(map_node[nod.back]);
    else
    {
        if (leafCheck == -1) leafCheck = nod.back;
        if (renderLevel) drawleaf(nod.back);
    }
}

void checknode(const node nod)
{
    if (nod.a * leafTestPosition.x + nod.b * leafTestPosition.y + nod.c * leafTestPosition.z - nod.d > 0.0f)   // If player is in front of plane
    {
        checkfront(nod);
        if (renderLevel) checkback(nod);
    }
    else   // If player is behind or inside plane
    {
        checkback(nod);
        if (renderLevel) checkfront(nod);
    }
}

void checkRootNode(float positionX, float positionY, float positionZ, bool drawTriangles, int* leafToSet)
{
    leafCheck = -1;
    leafTestPosition.x = positionX;
    leafTestPosition.y = positionY;
    leafTestPosition.z = positionZ;
    renderLevel = drawTriangles;
    checknode(map_node[0]);
    *leafToSet = leafCheck;
}
