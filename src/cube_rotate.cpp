#include <Angel.h>
#include <iostream>
#include <cstdlib>
#include "rubiks.h"

int lastRotationAxis, lastRotationWasClockwise;
int lastRotationCubesBefore[CUBES_PER_PLANE],  // indices
    lastRotationCubesAfter[CUBES_PER_PLANE];   // cube IDs
int newCubeColors[CUBES_PER_PLANE][FACES_PER_CUBE];

/** Rotate the colors on the faces of each subcube */
void addRotation(int cubeNum, int axis, bool isClockwise=true) {
	const int cwX[]  = {0,1,4,5,3,2};
	const int ccwX[] = {0,1,5,4,2,3};
	const int cwY[]  = {4,5,2,3,1,0};
	const int ccwY[] = {5,4,2,3,0,1};
	const int cwZ[]  = {2,3,1,0,4,5};
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

/** Rotate a 'slice' (plane) of the Rubik's cube */
void rotateSlice(int cubes[], int axis, int n, bool isClockwise) {
	if (!finishedRotating) // Current rotation not done. Do nothing.
		return;
	finishedRotating = false;

	int sliceNum = axis*3 + n;

	/*              Rotating a face:
	     0 1 2    6 3 0        0 1 2    2 5 8
	     3 4 5 -> 7 4 1        3 4 5 -> 1 4 7
	     6 7 8    8 5 2        6 7 8    0 3 6
		    Clockwise         Counterclockwise  */
	const int newIndicesCW[CUBES_PER_PLANE] = {6,3,0,7,4,1,8,5,2};
	const int newIndicesCCW[CUBES_PER_PLANE] = {2,5,8,1,4,7,0,3,6};
	const int *newIndices = isClockwise ? newIndicesCW : newIndicesCCW;

	// Unset previously-set rotation
	memset(rotationAxes, 0, sizeof(rotationAxes)); 

	// Store info about each subcube's rotation and color data
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

/** After the slice is done rotating, replace the original cubes with the new 
	cubes by changing the colors */
void updateCubes() {
	for (int i=0; i<CUBES_PER_PLANE; i++)
		memcpy(colors[lastRotationCubesBefore[i]],newCubeColors[i],sizeof(colors[0]));
	for (int i=0; i<CUBES_PER_PLANE; i++) {
		addRotation(lastRotationCubesBefore[i],lastRotationAxis,lastRotationWasClockwise);
		positions[lastRotationCubesBefore[i]] = lastRotationCubesAfter[i];
	}
	finishedRotating = true;

	if (isSolved())
		std::cout << "Rubik's cube solved!" << std::endl;
}

/** Rotate random slices of the Rubik's cube */
void randomRotations(int numRotations) {
	for (int i=0; i<numRotations; i++) {
		int r = rand() % NUM_PLANES;
		rotateSlice(positions,r/3,r%3,rand()%2);
		rotationProgress = 1.0f;
		updateRotationProgress();
	} 
}

/** If a cube in the center of a face is clicked, rotate that face */
void rotateFaceFromCube(int cubeNum) {
	switch (cubeNum) {
	case 4: rotateSlice(positions,2,0,true); break;
	case 10: rotateSlice(positions,1,0,false); break;
	case 12: rotateSlice(positions,0,0,true); break;
	case 14: rotateSlice(positions,0,2,false); break;
	case 16: rotateSlice(positions,1,2,true); break;
	case 22: rotateSlice(positions,2,2,false); break;
	}
}

