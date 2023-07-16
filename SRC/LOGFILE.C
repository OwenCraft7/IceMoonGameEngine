/*-------------------------*/
/*   Ice Moon logfile.c    */
/*                         */
/*   Debug log generator   */
/*-------------------------*/

#include "LOGFILE.H"

void logfile()
{
    int i;
    FILE* log;
    log = fopen("DEBUGLOG.TXT", "w+");
    estimate_ram = 0;
    
    fprintf(log, "Vertices:    %d\n", vert_count);
    fprintf(log, "UVs:         %d\n", uv_count);
    fprintf(log, "Triangles:   %d\n", tri_count);
    fprintf(log, "Node Planes: %d\n", node_count);
    fprintf(log, "Leaves:      %d\n", leaf_count);
    fprintf(log, "Textures:    %d\n", texture_count);
    fprintf(log, "Entities:    %d\n\n", entity_count);

    fprintf(log, "Images:      %d\n", img_count);
    fprintf(log, "Fonts:       %d", font_count);

    estimate_ram = (vert_count * sizeof(vec3)) + (uv_count * sizeof(uv)) + (tri_count * sizeof(tri)) +
        (node_count * sizeof(node)) + (leaf_count * sizeof(int)) + (texture_count * sizeof(image)) +
        (entity_count * sizeof(entity)) + (img_count * sizeof(image)) + (font_count * sizeof(image));

    fprintf(log, "\n\nVertex List:");
    fprintf(log, "\n    X,  Y,  Z");
    for (i = 0; i < vert_count; i++)
        fprintf(log, "\n    %.2f, %.2f, %.2f", map_vert[i].x, map_vert[i].y, map_vert[i].z);

    fprintf(log, "\n\nUV List:");
    fprintf(log, "\n    U,  V,  LX, LY");
    for (i = 0; i < uv_count; i++)
        fprintf(log, "\n    %.2f, %.2f, %.2f, %.2f", map_uv[i].u, map_uv[i].v, map_uv[i].lx, map_uv[i].ly);

    fprintf(log, "\n\nTriangle List:");
    fprintf(log, "\n    V0, V1, V2, U0, U1, U2, Texture");
    for (i = 0; i < tri_count; i++)
        fprintf(log, "\n    %d, %d, %d,    %d, %d, %d,    %d", map_tri[i].v[0], map_tri[i].v[1], map_tri[i].v[2], map_tri[i].uv[0], map_tri[i].uv[1], map_tri[i].uv[2], map_tri[i].texture);

    fprintf(log, "\n\nNode Plane List:");
    fprintf(log, "\n    A,  B,  C,  D,  Front is leaf?, Back is leaf?, FNum, BNum");
    for (i = 0; i < node_count; i++)
        fprintf(log, "\n    %.2f, %.2f, %.2f, %.2f,    %d, %d,    %d, %d", map_node[i].a, map_node[i].b, map_node[i].c, map_node[i].d, map_node[i].f_leaf, map_node[i].b_leaf, map_node[i].front, map_node[i].back);

    fprintf(log, "\n\nLeaf's Starting Triangle List:");
    for (i = 0; i < leaf_count; i++)
        fprintf(log, "\n    %d", map_leaf[i]);

    fprintf(log, "\n\nTexture List:");
    fprintf(log, "\n    Width,  Height");
    for (i = 0; i < texture_count; i++)
    {
        fprintf(log, "\n    %d, %d", map_texture[i].width, map_texture[i].height);
        estimate_ram += (map_texture[i].width * map_texture[i].height * sizeof(color));
    }

    fprintf(log, "\n\nEntity List:");
    fprintf(log, "\n    X,  Y,  Z,  X-Rotation, S0, S1, Type");
    for (i = 0; i < entity_count; i++)
        fprintf(log, "\n    %.2f, %.2f, %.2f, %.2f,    %d, %d,    %d", map_entity[i].x, map_entity[i].y, map_entity[i].z, map_entity[i].x_rotation, map_entity[i].settings[0], map_entity[i].settings[1], map_entity[i].type);

    fprintf(log, "\n\nImage List:");
    fprintf(log, "\n    Width,  Height");
    for (i = 0; i < img_count; i++)
    {
        fprintf(log, "\n    %d, %d", imgnumber[i].width, imgnumber[i].height);
        estimate_ram += (imgnumber[i].width * imgnumber[i].height * sizeof(color));
    }

    fprintf(log, "\n\nFont List:");
    fprintf(log, "\n    Character Width, Character Height");
    for (i = 0; i < font_count; i++)
    {
        fprintf(log, "\n    %d, %d", fontnumber[i].width / 16, fontnumber[i].height / 16);
        estimate_ram += (fontnumber[i].width * fontnumber[i].height * sizeof(color)) + sizeof(vwf_settings);
    }

    fprintf(log, "\n\nEstimated RAM size of all of the listed objects: %d Bytes", estimate_ram);
    fclose(log);
}
