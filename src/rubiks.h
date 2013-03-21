#ifndef RUBIKS_H
#define RUBIKS_H
#include <Angel.h>

#define VERT_PER_CUBE 36
#define NUM_CUBES 27
#define ROTATION_FACTOR 0.5

#define INITIAL_ROTATION 45.0, 45.0, 45.0

void createVBO();
float calculateFPS();

extern int winWidth, winHeight;

#endif

