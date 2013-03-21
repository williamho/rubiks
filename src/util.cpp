/* Some useful utility functions such as determining FPS */
#include <Angel.h>

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

