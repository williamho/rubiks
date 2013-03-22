#include <Angel.h>
#include "rubiks.h"

int addRotation(int packed, int axis, bool clockwise=true) {
	int mask = 3 << (axis*2);
	if (clockwise) {
		if (packed & mask == mask)
			packed &= ~mask;
		else
			packed += 1 << (axis*2);
	}
	else {
		if (packed & mask)
			packed -= 1 << (axis*2);
		else
			packed |= mask;
	}
	return packed;
}

void rotateFace(int cubes[], int faceNum, bool clockwise) { // Clockwise
	const int faces[6][9] = { // Which cubes are on which face
		{  0,  3,  6,  9, 12, 15, 18, 21, 24},
		{  2,  5,  8, 11, 14, 17, 20, 23, 26},
		{  0,  1,  2,  9, 10, 11, 18, 19, 20},
		{  6,  7,  8, 15, 16, 17, 24, 25, 26},
		{  0,  1,  2,  3,  4,  5,  6,  7,  8},
		{ 18, 19, 20, 21, 22, 23, 24, 25, 26},
	};

	/*  Rotating a face:
	     0 1 2    6 3 0
	     3 4 5 -> 7 4 1 
	     6 7 8    8 5 2  */
	const int newIndicesCW[CUBES_PER_FACE] = {6,3,0,7,4,1,8,5,2};
	const int newIndicesCCW[CUBES_PER_FACE] = {3,6,9,2,5,6,1,4,7};
	const int *newIndices = clockwise ? newIndicesCW : newIndicesCCW;

	int newFace[CUBES_PER_FACE];
	memcpy(rotationsPrev,rotations,sizeof(rotations));
	for (int i=0; i<CUBES_PER_FACE; i++) {
		newFace[i] = cubes[faces[faceNum][newIndices[i]]];

		int *cubeRotation = &rotations[faces[faceNum][i]];
		switch(faceNum) {
		case 0: *cubeRotation = addRotation(*cubeRotation,0,true); break;
		case 1: *cubeRotation = addRotation(*cubeRotation,0,false); break;
		case 2: *cubeRotation = addRotation(*cubeRotation,1,true); break;
		case 3: *cubeRotation = addRotation(*cubeRotation,1,false); break;
		case 4: *cubeRotation = addRotation(*cubeRotation,2,false); break;
		case 5: *cubeRotation = addRotation(*cubeRotation,2,true); break;
		}
	}
	for (int i=0; i<CUBES_PER_FACE; i++)
		cubes[faces[faceNum][i]] = newFace[i];
}

