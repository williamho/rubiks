#include <Angel.h>
#include "rubiks.h"

int addRotation(int *packed, int axis, bool isClockwise=true) {
	// Z-axis is weird and reversed
	//if (axis == 2)
		//isClockwise = !isClockwise;

	const int mask = 3;
	int shift = axis * 2;

	// Unpack the bits to get the rotation of the cube (0 to 3 -> 0 to 270)
	int unpacked = (*packed & (mask << shift)) >> shift;

	// Update the info based on `isClockwise` and repack the bits
	unpacked += isClockwise ? 1 : -1;
	unpacked &= mask;
	*packed &= ~(mask << shift);
	*packed |= unpacked << shift;

	return *packed;
}

//DEBUG------------------------------
int comp (const void * elem1, const void * elem2) {
	int f = *((int*)elem1);
	int s = *((int*)elem2);
	if (f > s) return  1;
	if (f < s) return -1;
	return 0;
}
//DEBUG----------------------------

void rotateSlice(int cubes[], int axis, int n, bool isClockwise) {
	if (IS_ROTATING) // Current rotation not done. Do nothing.
		return;

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

	memset(rotationAxes, 0, sizeof(rotationAxes)); // TESTING

	int newPlane[CUBES_PER_PLANE];
	for (int i=0; i<CUBES_PER_PLANE; i++) {
		newPlane[i] = cubes[planes[sliceNum][newIndices[i]]];
		rotationAxes[planes[sliceNum][i]] = (axis+1) * (isClockwise ? 1 : -1); // TESTING
	}
	for (int i=0; i<CUBES_PER_PLANE; i++) {
		cubes[planes[sliceNum][i]] = newPlane[i];
		int *cubeRotation = &rotations[newPlane[i]];
		addRotation(cubeRotation,axis,isClockwise);
	}
		
	rotationStartTime = glutGet(GLUT_ELAPSED_TIME);
	rotationProgress = 0.0f;
}

