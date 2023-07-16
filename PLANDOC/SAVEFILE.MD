# Ice Moon Save File Documentation
Ice Moon uses the .SGF extension to load saved games. By default, they are in the game's main directory, but its destination path can be set by the user.

  ------

It uses the following format in pseudocode:

uint32 header; // "SGF0"
uint8 mod_count;
uint8 sfx_count;
uint16 entity_count;
char map_name[8];	// Determines the map's location: "\MAPS\map_name.POS"
uint32 playerDebugSettings;	// Contains info about debug status such as noclip
uint8 item[5], item_value[5], item_maxValue[5];
uint8 selectedItemSlot, playerState, playerStateKeyframe;
float player_X, player_Y, player_Z, camRotation_X, camRotation_y;
float velocity_X, velocity_Y, velocity_Z;

char save_mod_number[mod_count][8];
char save_sfx_number[sfx_count][8];
entity save_entity_number[entity_count];

entity{
	float x, y, z, x_rotation, y_rotation;
	float entityVelocity_X, entityVelocity_Y, entityVelocity_Z;
	uint16 type;
	uint32 beginning_settings; // 4 bytes of settings for every entity defined in the .POS file
	uint32 save_settings;
	uint8 entityState, entityStateKeyframe;
}