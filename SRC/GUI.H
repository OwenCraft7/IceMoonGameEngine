#ifndef GUI_H
#define GUI_H

enum gui_command
{
	GUI_IMAGE = 0,
	GUI_TEXT = 1,
	GUI_TEXTINTERACT = 2
};

enum gui_text_type_parameter
{
	GUI_DESTINATION = 0,
	GUI_FUNCTION = 1
};

typedef struct
{
	int value[15];
	char* optionalString;
} gui_parameters;

typedef struct
{
	int command_count;
	gui_parameters* command_parameters;
} gui_screen;

gui_screen* mainGUIScreen;

int screenCount = 1;
int activeGUIScreen = 1;

#endif
