# Ice Moon colormap documentation
Blending and multiplying colors using calculations alone for each pixel would be pretty expensive in the Ice Moon game engine. To optimize, the game uses a colormap, two actually, for drawing translucent pixels over others and lighting the surrounding environment.

  ------

Color Blend Map: (For translucent images and textures)
	Top Side: 33% blend
	Left Side: 66% blend
	Result = 1/3 Top + 2/3 Left;
	
  ------

Color Multiply Map: (For lighting)
	Top Side: Texture Pixel
	Left Side: Light Pixel
	Result = Light x Texture;