# Ice Moon Map Documentation
Ice Moon is composed of maps in the .POS file format.

.POS files are post-compiled maps played in-game, but cannot be edited except for entities. They must be in the mod folder's MAPS directory.

  ------

It uses the following format in pseudocode:

uint32 header; // "POS0"
uint16 vert_count, uv_count, tri_count, node_count, leaf_count, texture_count, entity_count;

vert map_vert[vert_count];			//
uv map_uv[uv_count];				// 
tri map_tri[tri_count];				// Triangles refrence three vertices and UV vertices and are organized by node leaves.
node map_node[node_count];			// Node planes are 3D planes that separate the map into node leaves.
uint16 map_leaf[leaf_count];		// Node leaves are 3D convex volumes of area defined by the node planes. The numbers in this array refer to its starting triangle since triangles are organized by node leaves.
texture map_texture[texture_count];	// Image textures located in the mod folder's TEXTURES directory.
entity map_entity[entity_count];	// Entities have no real function yet, but they are essentially NPCs, enemies, items, doors, etc.

vert
{
	float x, y, z;
}
uv
{
	float u, y, lx, ly;
}
tri
{
	uint16 v[3], uv[3];
	uint16 texture;
}
node
{
	float a, b, c, d;		// ax+by+cz+d ... Make sure the node plane is normalized!
	uint8 f_leaf, b_leaf;	// These act like bools; Is that thing in front/behind the plane a leaf node, or is it another node plane?
	uint16 front, back;		// The numbers could either refer to a node plane or a leaf node, depending on the above bools.
}
texture
{
	char[12] texture;	// Format example: "TEXTURE.BMP\0", "A.BMP\0\0\0\0\0\0\0", "FULLNAME.BMP"
}
entity
{
	float x, y, z, x_rotation;
	uint16 type;
	uint32 settings; // 4 bytes of settings for every entity
}