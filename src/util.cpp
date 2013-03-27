/* Some useful utility functions such as determining FPS */
#include <Angel.h>
#include <string>
#include <fstream>
#include <iostream>
#include "rubiks.h"

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

void saveState(char *filename) {
	if (IS_ROTATING)
		std::cout << "Warning: Cube is currently rotating; saved file may be inconsistent" << std::endl;

	std::fstream file(filename, std::ios::out | std::ios::binary);
	file.write((char *)positions, sizeof(positions));
	file.write((char *)colors, sizeof(colors));
	file.close();
}

void loadState(char *filename) {
	std::fstream file(filename, std::ios::in | std::ios::binary);
	file.seekg(0);
	file.read((char *)positions, sizeof(positions));
	file.read((char *)colors, sizeof(colors));
	file.close();
}

