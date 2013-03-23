#include <Angel.h>
#include "rubiks.h"

int addRotation(int packed, int axis, bool clockwise=true) {
	// Z-axis is weird and reversed
	if (axis == 2)
		clockwise = !clockwise;

	// Fetch the two bits corresponding to rotation for this axis
	int mask = 3 << (axis*2); 
	if (clockwise) {
		if (packed & mask) 
			packed -= 1 << (axis*2);
		else  // If bits are 00, set to 11
			packed |= mask;
	}
	else {
		if (packed & mask == mask) // If bits are 11, set to 00
			packed &= ~mask;
		else
			packed += 1 << (axis*2);
	}
	return packed;
}

void rotatePlane(int cubes[], int axis, int n, bool isClockwise) {
	int planeNum = axis*3 + n;
	const int planes[9][9] = {
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

	int newPlane[CUBES_PER_PLANE];
	memcpy(rotationsPrev,rotations,sizeof(rotations));
	for (int i=0; i<CUBES_PER_PLANE; i++) {
		if (isClockwise)
			newPlane[i] = cubes[planes[planeNum][newIndicesCW[i]]];
		else // If counter-clockwise, simply reverse the indices 
			newPlane[newIndicesCW[i]] = cubes[planes[planeNum][i]];
			
		int *cubeRotation = &rotations[planes[planeNum][i]];
		*cubeRotation = addRotation(*cubeRotation,axis,isClockwise);
	}
	for (int i=0; i<CUBES_PER_PLANE; i++)
		cubes[planes[planeNum][i]] = newPlane[i];
}

