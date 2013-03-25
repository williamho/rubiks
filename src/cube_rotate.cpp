#include <Angel.h>
#include "rubiks.h"

int lastRotationAxis, lastRotationWasClockwise;
int lastRotationCubesBefore[CUBES_PER_PLANE],  // indices
    lastRotationCubesAfter[CUBES_PER_PLANE];   // cube IDs
int newCubeColors[CUBES_PER_PLANE][FACES_PER_CUBE];

void addRotation(int cubeNum, int axis, bool isClockwise=true) {
	const int cwX[] = {0,1,4,5,3,2};
	const int ccwX[] = {0,1,5,4,2,3};
	const int cwY[] = {4,5,2,3,1,0};
	const int ccwY[] = {5,4,2,3,0,1};
	const int cwZ[] = {2,3,1,0,4,5};
	const int ccwZ[] = {3,2,0,1,4,5};
	const int *rotate;

	switch(axis) {
	case 0: rotate = isClockwise ? cwX : ccwX; break;
	case 1: rotate = isClockwise ? cwY : ccwY; break;
	case 2: rotate = isClockwise ? cwZ : ccwZ; break;
	}
	int newColors[FACES_PER_CUBE];
	for (int i = 0; i < FACES_PER_CUBE; i++)
		newColors[rotate[i]] = colors[cubeNum][i];
	for (int i = 0; i < FACES_PER_CUBE; i++) 
		colors[cubeNum][i] = newColors[i];

}

void rotateSlice(int cubes[], int axis, int n, bool isClockwise) {
	if (!finishedRotating) // Current rotation not done. Do nothing.
		return;
	finishedRotating = false;

	int sliceNum = axis*3 + n;
	const int planes[9][9] = { // Positions on the cube that correspond to planes
		// Planes x=0 to x=2
		{  0,  3,  6,  9, 12, 15, 18, 21, 24},
		{  1,  4,  7, 10, 13, 16, 19, 22, 25},
		{  2,  5,  8, 11, 14, 17, 20, 23, 26},
		// Planes y=0 to y=2
		{  0,  1,  2,  9, 10, 11, 18, 19, 20},
		{  3,  4,  5, 12, 13, 14, 21, 22, 23},
		{  6,  7,  8, 15, 16, 17, 24, 25, 26},
		// Planes z=0 to z=2
		{  0,  1,  2,  3,  4,  5,  6,  7,  8}, 
		{  9, 10, 11, 12, 13, 14, 15, 16, 17},
		{ 18, 19, 20, 21, 22, 23, 24, 25, 26},
	};

	/*              Rotating a face:
	     0 1 2    6 3 0        0 1 2    2 5 8
	     3 4 5 -> 7 4 1        3 4 5 -> 1 4 7
	     6 7 8    8 5 2        6 7 8    0 3 6
		    Clockwise         Counterclockwise  */
	const int newIndicesCW[CUBES_PER_PLANE] = {6,3,0,7,4,1,8,5,2};
	const int newIndicesCCW[CUBES_PER_PLANE] = {2,5,8,1,4,7,0,3,6};
	const int *newIndices = isClockwise ? newIndicesCW : newIndicesCCW;

	memset(rotationAxes, 0, sizeof(rotationAxes)); 

	for (int i=0; i<CUBES_PER_PLANE; i++) {
		lastRotationCubesBefore[i] = planes[sliceNum][i];
		lastRotationCubesAfter[i] = cubes[planes[sliceNum][newIndices[i]]];
		rotationAxes[planes[sliceNum][i]] = (axis+1) * (isClockwise ? 1 : -1); 
		memcpy(newCubeColors[i],colors[planes[sliceNum][newIndices[i]]],sizeof(colors[0]));
	}

	rotationStartTime = glutGet(GLUT_ELAPSED_TIME);
	rotationProgress = 0.0f;
	lastRotationAxis = axis;
	lastRotationWasClockwise = isClockwise;
}

void updateCubes() {
	for (int i=0; i<CUBES_PER_PLANE; i++)
		memcpy(colors[lastRotationCubesBefore[i]],newCubeColors[i],sizeof(colors[0]));
	for (int i=0; i<CUBES_PER_PLANE; i++) {
		addRotation(lastRotationCubesBefore[i],lastRotationAxis,lastRotationWasClockwise);
		positions[lastRotationCubesBefore[i]] = lastRotationCubesAfter[i];
	}
	finishedRotating = true;
}

