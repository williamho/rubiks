#ifndef RUBIKS_H
#define RUBIKS_H
#include <Angel.h>

#define VERT_PER_CUBE 36
#define NUM_CUBES 27

void createVBO();
float calculateFPS();

extern int winWidth, winHeight;
extern vec3 theta;

#endif

