OpenGL Rubik's Cube
===================

#Requirements

* Cygwin (Makefile written with Cygwin in mind, but the code itself should 
	theoretically work on any OS)
* OpenGL 3.2

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#Build

The program can be compiled by running `make`.

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#Run

The program can be executed with one optional argument, which specifies the 
number of rotations that should be applied at the beginning of the program.

For example, if run with `./rubiks 1`, a slice of the Rubik's cube will be 
randomly chosen and rotated either clockwise or counterclockwise once.

If no argument is provided, no rotations will be applied.

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#Controls

##Keyboard
* Q/Esc: Quit
* W: Write (Save current state to file "savestate.rubiks")
* E: Edit  (Load from saved file "savestate.rubiks")
* R: Reset (Reset camera angle to default)
* H/L: Rotate entire Rubik's cube about the X-axis
* J/K: Rotate entire Rubik's cube about the Z-axis
* 1-9: Rotate slices of the Rubik's cube clockwise 
	* 1-3 correspond to the X-axis
	* 4-6 correspond to the Y-axis
	* 7-9 correspond to the Z-axis
	* Pressing `shift` with 1-9 keys will rotate counterclockwise

##Mouse
* Left click: If clicking on the center cube of a face, rotate face clockwise
* Right click: Same as left click but counterclockwise
* Middle click: Hold down middle click and move the mouse to rotate the entire 
	Rubik's cube about the X and Y axes.
* Scroll wheel: Zoom in/out of the Rubik's cube

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

