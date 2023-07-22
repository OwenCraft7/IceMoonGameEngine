# Ice Moon - A 3D MS-DOS Game Engine

This is a 3D game engine I created in MS-DOS.

![Alt text](PREVIEW.PNG?raw=true "Preview of the engine.")

If you want to try this engine yourself, follow this setup. This setup assumes you have DOSBox 0.74-3 installed, but you can test this on real hardware too.

  ------

You must have the following specifications to run this engine:

-	CPU:	Intel Pentium (85 MHz+ recommended)
-	RAM:	8 MB+
-	GPU:	VGA or better
-	OS:		MS-DOS 5.0+
-	Storage: (Updated as of July 22 2023)
	- If you just want to play the game:	At least 569 KB
	- If you want to modify the code too:	At least 11.3 MB

First, rename the extracted IceMoonGameEngine-main folder to ICESRC, then place it in your DOS C: drive. Next, in DOS, go to the ICESRC directory and type "play". You can also go to the BIN directory and type "icemoon" to do the same thing.

  ------

For Windows users using DOSBox, a shortcut is provided for your pleasure, along with these instructions.

First, open "ICEMOON.CONF", scroll to the bottom, and change the file path after "mount c" to your DOSBox C: drive location. Next, right click "ICEMOON.CONF" and click "Copy As Path", then open the shortcut's properties. Select everything after "-conf" in "Target:" and paste your text.

If you don't have DOSBox version 0.74-3, change "DOSBox-0.74-3" in both "Target:" and "Start in:" to your version. (You'll want to change your DOSBox installation path too if it doesn't match the one in the properties.) Finally, open the shortcut.

If you just want to play the engine, you're pretty much done here. However, if you want to modify the source code, follow these steps:

  ------

Install Open Watcom 2.0 from this link:

https://github.com/open-watcom/open-watcom-v2/releases

Find the current build and click the Assets drop-down arrow, then click on the file that ends in "-c-dos.exe". If the above link is down, download it from here instead:

https://web.archive.org/web/20230716115926/https://github.com/open-watcom/open-watcom-v2/releases

The Watcom installer is over 100 MB in size, which might be too big for a real DOS machine. Plus, it would be very tedious to modify the code in DOS alone, so if you haven't already, install DOSBox for this; it's much easier to modify the code on your own OS instead of DOS.

Place the Watcom installer inside the DOS C: drive, then run the installer in DOS and set the installation directory to "C:\WATCOM".

I recommend performing a selective installation to save as much storage space as possible. It's required if you're gonna test this engine on real hardware.

-	Disable everything in the 16-Bit, Other, and Help Files tabs.
-	Enable everything in the 32-Bit tab.
-	Enable only DOS in the Target and Host tabs.

Delete the installer once finished.

Next, in DOS, go to the ICESRC directory, then type "make". The game will compile and show you if there are any coding errors. "SRC\MAIN.C" will be compiled into "BIN\ICEMOON.EXE".

  ------

Feel free to share your feedback, suggestions, or changes to the engine. You can look in the DOC folder to see documentation of some of the game's assets.

I also want to mention that I'm not an experienced C programmer nor experienced GitHub user, so please tell me how I can optimize my code, fix errors, and make it more user-friendly. Thank you!