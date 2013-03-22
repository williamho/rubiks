#include <Angel.h>
#include "rubiks.h"

int addRotation(int packed, int axis, bool clockwise=true) {
	// Z-axis is weird and reversed
	if (axis == 2)
		clockwise = !clockwise;

	// TODO: clean this up
	int mask = 3 << (axis*2);
	if (clockwise) {
		if (packed & mask)
			packed -= 1 << (axis*2);
		else
			packed |= mask;
	}
	else {
		if (packed & mask == mask)
			packed &= ~mask;
		else
			packed += 1 << (axis*2);
	}
	return packed;
}

void rotateFace(int cubes[], int faceNum, bool clockwise) { 
	// Which cubes are on which face. See `cube_data.cpp` for face numbers.
	const int faces[6][9] = { 
		{  0,  3,  6,  9, 12, 15, 18, 21, 24},
		{  2,  5,  8, 11, 14, 17, 20, 23, 26},
		{  0,  1,  2,  9, 10, 11, 18, 19, 20},
		{  6,  7,  8, 15, 16, 17, 24, 25, 26},
		{  0,  1,  2,  3,  4,  5,  6,  7,  8},
		{ 18, 19, 20, 21, 22, 23, 24, 25, 26},
	};

	/*              Rotating a face:
	     0 1 2    6 3 0        0 1 2    2 5 8
	     3 4 5 -> 7 4 1        3 4 5 -> 1 4 7
	     6 7 8    8 5 2        6 7 8    0 3 6
		    Clockwise         Counterclockwise
	*/
	const int newIndicesCW[CUBES_PER_FACE] = {6,3,0,7,4,1,8,5,2};
	const int newIndicesCCW[CUBES_PER_FACE] = {2,5,8,1,4,7,0,3,6};
	const int *newIndices = clockwise ? newIndicesCW : newIndicesCCW;

	int newFace[CUBES_PER_FACE];
	memcpy(rotationsPrev,rotations,sizeof(rotations));
	for (int i=0; i<CUBES_PER_FACE; i++) {
		newFace[i] = cubes[faces[faceNum][newIndices[i]]];

		int *cubeRotation = &rotations[faces[faceNum][i]];
		*cubeRotation = addRotation(*cubeRotation,faceNum/2,faceNum%2==0);
	}
	for (int i=0; i<CUBES_PER_FACE; i++)
		cubes[faces[faceNum][i]] = newFace[i];
}

