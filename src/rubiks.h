#ifndef RUBIKS_H
#define RUBIKS_H
#include <Angel.h>

#define VERT_PER_CUBE 36
#define CUBES_PER_PLANE 9
#define NUM_CUBES 27
#define ROTATION_DURATION 200

#define INITIAL_ROTATION 45.0, 45.0, -45.0
#define ROTATION_FACTOR  0.5

#define INITIAL_SCALE    0.5
#define MIN_SCALE        0.05
#define MAX_SCALE        0.75
#define SCALE_FACTOR     0.05

void createVBO();
float calculateFPS();
void rotateSlice(int cubes[], int axis, int n, bool isClockwise=true);

extern int winWidth, winHeight;
extern int positions[NUM_CUBES];
extern GLint rotations[NUM_CUBES];
extern GLint rotationsPrev[NUM_CUBES];
extern GLfloat rotationProgress;
extern int rotationStartTime;

#define IS_ROTATING (rotationProgress < 1.0f)

#endif

