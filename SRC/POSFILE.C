/*--------------------------*/
/*    Ice Moon posfile.c    */
/*                          */
/* .pos file format loading */
/*--------------------------*/
// Loads post-compiled map files.

#include "POSFILE.H"

void free_mapdata()
{
    int i;
    free(map_vert);
    free(map_uv);
    free(map_tri);
    free(map_node);
    free(map_leaf);
    for (i = texture_count - 1; i >= 0; i--)
        free(map_texture[i].pixel);
    free(map_texture);
    free(map_entity);
}

void load_pos(const char* file)
{
    FILE *map;
    int i, j;
    char texture[13], tx_string[31], filepath[23];

    sprintf(filepath, "%s/%s", mod_folder, file);
    
    if (!endGameLoop && (map = fopen(filepath, "rb")) != NULL)
    {
        free_mapdata();
        // If file header is invalid, end the game.
        if (fgetc(map) != 'P' || fgetc(map) != 'O' || fgetc(map) != 'S' || fgetc(map) != '0')
        {
            endGameLoop = true;
            errormessage = 4;
            goto CANCEL;
        }
        fread(&vert_count, 2, 1, map);
        fread(&uv_count, 2, 1, map);
        fread(&tri_count, 2, 1, map);
        fread(&node_count, 2, 1, map);
        fread(&leaf_count, 2, 1, map);
        fread(&texture_count, 2, 1, map);
        fread(&entity_count, 2, 1, map);
        
        map_vert = calloc(vert_count, sizeof(vert));
        map_uv = calloc(uv_count, sizeof(uv));
        map_tri = calloc(tri_count, sizeof(tri));
        map_node = calloc(node_count, sizeof(node));
        map_leaf = calloc(leaf_count, sizeof(int));
        map_texture = calloc(texture_count, sizeof(image));
        map_entity = calloc(entity_count, sizeof(entity));

        for (i = 0; i < vert_count; i++)
        {
            fread(&map_vert[i].x, 4, 1, map);
            fread(&map_vert[i].y, 4, 1, map);
            fread(&map_vert[i].z, 4, 1, map);
        }
        for (i = 0; i < uv_count; i++)
        {
            fread(&map_uv[i].u, 4, 1, map);
            fread(&map_uv[i].v, 4, 1, map);
            fread(&map_uv[i].lx, 4, 1, map);
            fread(&map_uv[i].ly, 4, 1, map);
        }
        for (i = 0; i < tri_count; i++)
        {
            fread(&map_tri[i].v[0], 2, 1, map); fread(&map_tri[i].v[1], 2, 1, map); fread(&map_tri[i].v[2], 2, 1, map);
            fread(&map_tri[i].uv[0], 2, 1, map); fread(&map_tri[i].uv[1], 2, 1, map); fread(&map_tri[i].uv[2], 2, 1, map);
            fread(&map_tri[i].texture, 2, 1, map);
        }
        for (i = 0; i < node_count; i++)
        {
            fread(&map_node[i].a, 4, 1, map);
            fread(&map_node[i].b, 4, 1, map);
            fread(&map_node[i].c, 4, 1, map);
            fread(&map_node[i].d, 4, 1, map);
            fread(&map_node[i].f_leaf, 1, 1, map);
            fread(&map_node[i].b_leaf, 1, 1, map);
            fread(&map_node[i].front, 2, 1, map);
            fread(&map_node[i].back, 2, 1, map);
        }
        for (i = 0; i < leaf_count; i++)
        {
            fread(&map_leaf[i], 2, 1, map);
        }
        for (i = 0; i < texture_count; i++)
        {
            fread(texture, 1, 12, map);
            texture[12] = '\0';
            sprintf(tx_string, "TEXTURES/%s", texture);
            loadimage(tx_string, &map_texture[i]);
        }
        for (i = 0; i < entity_count; i++)
        {
            fread(&map_entity[i].type, 2, 1, map);
            fread(&map_entity[i].position.x, 4, 1, map);
            fread(&map_entity[i].position.y, 4, 1, map);
            fread(&map_entity[i].position.z, 4, 1, map);
            fread(&map_entity[i].x_rotation, 4, 1, map);
            fread(&map_entity[i].y_rotation, 4, 1, map);
            for (j = 0; j < 8; j++)
                fread(&map_entity[i].misc[j], 4, 1, map);
        }
    }
    CANCEL: // If the map loading process screws up, go to this label.
    fclose(map);
}
