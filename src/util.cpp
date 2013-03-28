/* Some useful utility functions such as determining FPS */
#include <Angel.h>
#include <string>
#include <fstream>
#include <iostream>
#include "rubiks.h"

/** Calculate the frames per second over multiple frames */
float calculateFPS() {
	static int frameCount, currentTime, previousTime;
	static float fps;

	frameCount++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;

	if(timeInterval > 1000) {
		fps = 1000.0*frameCount/timeInterval;
		previousTime = currentTime;
		frameCount = 0;
	}
	return fps;
}

/** Save the current state of the Rubik's cube as a binary file */
void saveState(char *filename) {
	if (IS_ROTATING) {
		std::cout << "Error: Wait until rotation has finished before saving." << std::endl;
		return;
	}

	std::fstream file(filename, std::ios::out | std::ios::binary);
	file.write((char *)positions, sizeof(positions));
	file.write((char *)colors, sizeof(colors));
	file.close();
}

/** Load a saved Rubik's cube state from a binary file */
void loadState(char *filename) {
	if (IS_ROTATING) {
		std::cout << "Error: Wait until rotation has finished before loading." << std::endl;
		return;
	}

	std::fstream file(filename, std::ios::in | std::ios::binary);
	file.seekg(0);
	file.read((char *)positions, sizeof(positions));
	file.read((char *)colors, sizeof(colors));
	file.close();
}

/** For each subcube on the specified face, determine if all relevant 
	subcube faces are the same color */
bool faceIsSolved(int faceNum) {
	int planeNum;
	switch(faceNum) {
	case 0: planeNum = 0; break;
	case 1: planeNum = 2; break;
	case 2: planeNum = 3; break;
	case 3: planeNum = 5; break;
	case 4: planeNum = 6; break;
	case 5: planeNum = 8; break;
	}

	for (int i = 0; i < CUBES_PER_PLANE-1; i++) {
		if (colors[planes[planeNum][i]][faceNum] != 
		    colors[planes[planeNum][i+1]][faceNum])
			return false;
	}
	return true;
}

/** Determine if the entire Rubik's cube is solved */
bool isSolved() {
	for (int f=0; f<FACES_PER_CUBE; f++) {
		if (!faceIsSolved(f))
			return false;
	}
	return true;	
}

