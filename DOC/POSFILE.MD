# Ice Moon Map Documentation
Ice Moon is composed of maps in the .POS file format.

.POS files are post-compiled maps played in-game, but cannot be edited easily except for entities. They must be in the mod folder's MAPS directory.

  ------

It uses the following format in pseudocode:

```
uint32 header; // "POS0"
uint16 vert_count, uv_count, tri_count, node_count, leaf_count, texture_count, entity_count;

vert map_vert[vert_count];			// Vertices have an X, Y, and Z position.
uv map_uv[uv_count];				// UVs determine how the triangles' textures look.
tri map_tri[tri_count];				// Triangles reference three vertices and UVs and are organized by node leaves.
node map_node[node_count];			// Node planes are 3D planes that separate the map into node leaves.
uint16 map_leaf[leaf_count];		// Node leaves are 3D convex volumes defined by the node planes. The numbers in this array refer to its starting triangle since triangles are organized by node leaves.
texture map_texture[texture_count];	// Image textures located in the mod folder's TEXTURES directory.
entity map_entity[entity_count];	// Entities have no real function yet, but they are essentially NPCs, enemies, items, doors, etc.

vert
{
	float x, y, z;
}
uv
{
	float u, v, lx, ly;	// lx and ly are lightmap coordinates, though lightmaps haven't been implemented yet.
}
tri
{
	uint16 v[3], uv[3];
	uint16 texture;
}
node
{
	float a, b, c, d;		// ax+by+cz+d ... Make sure the node plane is normalized!
	uint8 f_leaf, b_leaf;	// These act like bools; Is that thing in front/behind the node plane a node leaf, or is it another node plane?
							// Set to 1 for node leaf, otherwise, 0 for node plane.
	uint16 front, back;		// The numbers could either refer to a node plane or a node leaf depending on the above bools.
}
texture
{
	char[12] texture;	// Format example: "TEXTURE.BMP\0", "A.BMP\0\0\0\0\0\0\0", "FULLNAME.BMP"
}
entity
{
	uint16 type;
	float x, y, z, x_rotation, y_rotation;
	float misc[8]; // 8 custom float variables for every entity
}
```
