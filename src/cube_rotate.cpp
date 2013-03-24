#include <Angel.h>
#include "rubiks.h"

void rotatePlane(int cubes[], int axis, int n, bool isClockwise) {
	if (IS_ROTATING) // Current rotation not done. Do nothing.
		return;

	int r;
	for (int i=0; i<NUM_CUBES; i++) {
		switch(r = rotations[i]) {
		case -1: case 1:
			rotationsPrev[i] = RotateX(90*r/1) * rotationsPrev[i];
			break;
		case -2: case 2:
			rotationsPrev[i] = RotateY(90*r/2) * rotationsPrev[i];
			break;
		case -3: case 3:
			rotationsPrev[i] = RotateZ(-90*r/3) * rotationsPrev[i];
			break;
		}
		rotations[i] = 0;
	}

	int planeNum = axis*3 + n;
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

	int newPlane[CUBES_PER_PLANE];
	for (int i=0; i<CUBES_PER_PLANE; i++) {
		newPlane[i] = cubes[planes[planeNum][newIndices[i]]];
		rotations[newPlane[i]] = (axis+1) * (isClockwise ? 1 : -1);
	}
	for (int i=0; i<CUBES_PER_PLANE; i++) {
		cubes[planes[planeNum][i]] = newPlane[i];
	}

	// DEBUG--------------------------------
	char c[10];
	for (int i=0; i<NUM_CUBES; i++) {
		if (i%3 == 0)
			fprintf(stderr,"\n");
		if (i%9 == 0)
			fprintf(stderr,"\n");

		//fprintf(stderr,"%2d ",cubes[i]);
		fprintf(stderr,"%2d ",rotations[cubes[i]]);
	}
	fprintf(stderr,"\n----");
	//--------------------------------------
		
	rotationStartTime = glutGet(GLUT_ELAPSED_TIME);
	rotationProgress = 0.0f;
}

