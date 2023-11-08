# GUI documentation
Ice Moon uses text files to form unique GUI screens. Each GUI screen's value depends on where it is referenced in the GUISCR.CFG file in a mod folder. GUI screen 0 stays completely blank, so the first line in GUISCR.CFG would reference screen #1.

In each GUI text file, the first word in each line are commands. Each command has their own parameters and are separated with spaces.

Here are the different commands:

-	`IMAGE <x y img_number>`
	
	This command draws images. The x and y parameters are the image's coordinates on the screen, and img_number is the image to display in the in-game's image list.

-	`TEXT <x y font_number line_special>`
	
	This command draws text. The x and y parameters are the text's coordinates on the screen, and font_number is the text's numerical font.

	If line_special is 0, then line below the command will be placeholder text, not another command. Therefore, the command will look like this:
```
TEXT 0 0 0 0
Hello, world! This is placeholder text.
```
	If line_special is not 0, then the line below the command will be another command, not placeholder text. this is what the line_special will be:
	
	-	1 - 20:		Chat Lines 0 - 19
	-	21 - 30:	Debug Info Lines 0 - 9
	-	31:			Chat Typing Line
	-	32:			Current HP
	-	33:			Maximum HP
	-	34:			Loaded Ammo
	-	35:			Unloaded Ammo
	
	So if line_special were set to 31, shown below:

`TEXT 0 200 0 31`

	The line would show whatever the player is typing in chat.

-	`TEXTINTERACT <x y font_idle font_hover font_enable set_id id_up id_left id_right id_down text_type number>`
	
	This command draws interactive text. The line below the command will always be placeholder text no matter what.
	
	The x and y parameters are the text's coordinates and the font parameters are the font numbers when the text is idle, selected, and activated.
	
	The set_id parameter is the interactive text's ID in the GUI screen.
	
	The next four id parameters are here because of keyboard interaction. Say, if the player presses the up arrow on the keyboard, the cursor shall move to the interactive text ID defined in id_up. If the "Load Game" text (ID #1) was below "New Game" (ID #0), then the latter's TEXTINTERACT id_down value would be 1, and the other's id_up value would be 0. If you want a certain key, say the left key, to not take you to another text, then set id_left to -1.
	
	The text_type parameter can be either two words: DESTINATION or FUNCTION.
	
	Setting text_type to DESTINATION will send the player to a different GUI screen based on the number parameter. For example, if a text would take you to GUI screen #6 when it is activated, this is how it would work:
```
TEXTINTERACT 0 0 3 4 5 0 -1 -1 -1 1 DESTINATION 6
This text will take you to GUI screen #6.
```	
	Setting text_type to FUNCTION will make the game do unique functions. Here are the following functions the game will execute depending on the number parameter:
	
	-	0:		Nothing
	-	1:		
	
	For example:
```
TEXTINTERACT 0 0 3 4 5 0 -1 -1 -1 1 FUNCTION 3
Click this text to execute function #3.
```