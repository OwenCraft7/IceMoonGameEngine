# Ice Moon Graphics File Documentation, by SampleImage

**This is a planned document! Everything below here is not part of the actual game engine yet.**

Ice Moon uses the .IGF extension to load graphics of any kind, such as images, texture, text, and GUI.

The file starts with a 4 byte "IGF0" string. // "IGF0" Graphics File Version 0

The next byte's least significant nybble contains many different graphics modes to draw the image in a unique way.

  ------

Here are the list of modes:

0:	256 color image, no compression
	Unrestricted width and height
	8 bits per pixel

1:	16 color image, no compression
	Width fixed in 2px multiples
	Unrestricted height
	4 bits per pixel

2:	4 color image, no compression
	Width fixed in 4px multiples
	Unrestricted height
	2 bits per pixel

3:	256 color image, run-length encoding
	Unrestricted width and height
	8 bits per pixel

4:	16 color image, run-length encoding
	Width fixed in 2px multiples
	Unrestricted height
	4 bits per pixel

5:	4 color image, run-length encoding
	Width fixed in 4px multiples
	Unrestricted height
	2 bits per pixel

6:	16 color image, quadtree encoding, width and height fixed to powers of 2
	Width and height fixed in powers of 2
	4 bits per pixel

7:	4 color image, quadtree encoding, width and height fixed to powers of 2
	Width and height fixed in powers of 2
	2 bits per pixel

8:	256 color image, blue channel has a 2:1 pixel ratio
	Width fixed in 2px multiples
	Unrestricted height
	8 bits per pixel

9:	256 color image, blue and red channels have a 2:1 pixel ratio
	Width fixed in 2px multiples
	Unrestricted height
	8 bits per pixel

A:	256 color image, red channel 2:1 pixel ratio, blue channel 2:2 pixel ratio
	Width and height fixed in 2px multiples
	8 bits per pixel

B:	16 palettes with 16 colors each, each 8x8 pixel tile has a specific palette
	Width and height fixed in 8px multiples
	4 bits per pixel

C:	64 palettes with 4 colors each, each 8x8 pixel tile has a specific palette
	Width and height fixed in 8px multiples	// 3 bytes of palette information per 4 tiles
	2 bits per pixel

D:	8x8 tiles with varying bits per pixel
	16 palettes for 4bpp tiles
	64 palettes for 2bpp tiles
	2 color choice for 1bpp tiles
	1 color for 0bpp tiles
	Width and height fixed in 8px multiples

E:	There are ON/OFF 8x8 pixel tiles
	256 colors inside an OFF tile (in 8-8-4 RGB format, 8 bits per pixel)
	64 colors inside an ON tile (in 4-4-4 RGB format, 64 bits per pixel)
	Width and height fixed in 8px multiples

F:	undefined