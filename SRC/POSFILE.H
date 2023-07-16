#ifndef POSFILE_H
#define POSFILE_H

typedef struct
{
    float u, v, lx, ly;
} uv;
typedef struct
{
    int texture;
    uint16_t v[3], uv[3];
} tri;
typedef struct
{
    float a, b, c, d;
    int front, back;
    int f_leaf, b_leaf;
} node;
typedef struct
{
    float x, y, z, x_rotation;
    int settings[2];
    int type;
} entity;

int vert_count = 0, uv_count = 0, tri_count = 0, node_count = 0, leaf_count = 0, texture_count = 0, entity_count = 0;
vec3* map_vert;
uv* map_uv;
tri* map_tri;
node* map_node;
int* map_leaf;
image* map_texture;
entity* map_entity;

#endif