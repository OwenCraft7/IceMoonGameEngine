# Ice Moon - A 3D MS-DOS Game Engine
This is a 3D game engine I created in MS-DOS. If you want to try this engine yourself, follow this setup.

This setup assumes you have DOSBox 0.74-3 installed. You can test this on real hardware too.

You must have the following specifications to run this engine:

-	CPU:	Intel Pentium 75 MHz or above
-	RAM:	8 MB or above
-	GPU:	VGA or better
-	OS:		MS-DOS 5.0 or above
-	Storage (Updated as of 7/16th/2023):
	-	At least 400 KB for the BIN folder in ICESRC
	-	At least 500 KB for the entire ICESRC folder
	-	At least 27 MB for both the ICESRC and WATCOM folders

I recommend you place the ICESRC folder in your mounted DOSBox C: drive, but you can place it anywhere.

  ------

Next, install Open Watcom 2.0 from this link:
https://github.com/open-watcom/open-watcom-v2/releases

Find the current build and click the Assets drop-down arrow, then click on the file that end in "-c-dos.exe".

If the above link doesn't work, download it from here instead:
https://web.archive.org/web/20230716115926/https://github.com/open-watcom/open-watcom-v2/releases

Place the Watcom installer inside the DOSBox C: drive, then run DOSBox and set the installation directory to "C:\WATCOM".

I recommend performing a selective installation to save as much storage space as possible. It's required if you're gonna test this engine on real hardware.
-	Disable everything in the 16-Bit tab.
-	Enable everything in the 32-Bit tab.
-	Enable only DOS in the Target and Host tabs.
-	Disable everything in the Other tab.
-	Enable only DOS in the Help Files tab.

Delete the installer once finished.

  ------

Next, in DOSBox, go to your ICESRC directory using the cd command, then type "make". The game will compile and show you if there are any coding errors.

"SRC\MAIN.C" will be compiled into "BIN\ICEMOON.EXE".

Alternatively, you can type "play" to quick-launch the game instead if you don't want to compile all the time.

  ------
  Optional:

For Windows users, a shortcut is provided for your pleasure, along with these instructions.

First, open "ICEMOON.CONF", scroll to the bottom, and change "Owen" to your username. Next, right click "ICEMOON.CONF" and click "Copy As Path", then open the shortcut's properties. Select everything after "-conf" in "Target:" and paste your text.

If you don't have DOSBox version 0.74-3, change "DOSBox-0.74-3" in both "Target:" and "Start in:" to your version. (You'll want to change your DOSBox installation path too if it doesn't match the one in the properties.) Finally, open the shortcut.

  ------

Excellent, you are now ready to test the Ice Moon engine! Feel free to share your feedback, suggestions, or changes to the engine. You can look in the DOC folder to see documentation of some of the game's assets.

I also want to mention that I'm not an experienced C programmer or a GitHub user, so for any experts, please tell me how I can optimize my code and make this repository more user-friendly. Thank you!
