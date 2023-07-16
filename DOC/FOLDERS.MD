# Ice Moon mod folder and file documentation
Mod folders are folders used in the game's directory to store every file inside it into a full game, mod, or resource pack.

The Ice Moon engine must read the first line in the CFG file to recognize a mod folder.

  ------

A mod folder contains files ranging in different types. Here are the different file types Ice Moon supports:

-	.BMP Bitmap image (8-bit only):
	
	CAUTION! You must use widths with multiples of 4 pixels, otherwise, the image will look funky.
	
	Also, for textures (not GUI and fonts), you must use widths and heights in powers of 2. (4px, 8px, 16px, 32px, 64px, 128px, 256px, and 512px are acceptable for texture widths and heights.)
	
	To comply with the game's 256 color palette, all images are converted to an 8-8-4 RGB format before it's in RAM:
	8 levels for red, 8 levels for green, and 4 levels for blue. (8 x 8 x 4 = 256)

-	.IGF Ice Moon graphics file:

	This is a planned feature! Read more about its documentation in PLANDOC\GFXFILE.MD.

-	.POS Ice Moon post-compiled map file:

	Read more about its documentation in POSFILE.MD.

-	.SGF Ice Moon save file:

	This is a planned feature! Read more about its documentation in PLANDOC\SAVEFILE.MD.