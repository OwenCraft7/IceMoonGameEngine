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
    if (playerLeaf == -1)
        playerLeaf = select;
    for (i = map_leaf[select]; i < end; i++)   // Keep in mind, triangles are sorted by leaf number
        texture_tri(map_tri[i]);
}

void checkfront(const node nod)
{
    if (nod.f_leaf == 0) checknode(map_node[nod.front]);
    else drawleaf(nod.front);
}
void checkback(const node nod)
{
    if (nod.b_leaf == 0) checknode(map_node[nod.back]);
    else drawleaf(nod.back);
}

void checknode(const node nod)
{
    if (nod.a * playerPos.x + nod.b * camPosY + nod.c * playerPos.z - nod.d > 0.0f)   // If player is in front of plane
    {
        checkfront(nod);
        checkback(nod);
    }
    else   // If player is behind plane
    {
        checkback(nod);
        checkfront(nod);
    }
}
