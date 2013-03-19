#include <Angel.h>
#include "rubiks.h"

extern GLuint vbo, ibo;
void createVBO() {
	/*   5-------6
	    /|      /|      y
	   1-------2 |      |/
	   | |     | |    --+--x
	   | 4-----|-7     /|
	   |/      |/     z
	   0-------3             */

	// Vertices of the cube centered around the origin, one face per line
	const GLfloat vertexData[] = {
		-1, -1,  1,
		-1,  1,  1,
		 1,  1,  1,
		 1, -1,  1,
		-1, -1, -1,
		-1,  1, -1,
		 1,  1, -1,
		 1, -1, -1,
	};

	// Indices of points corresponding to each face of the cube
	const int faces[6][4] = {
		{1, 0, 3, 2},
		{2, 3, 7, 6},
		{3, 0, 4, 7},
		{6, 5, 1, 2},
		{4, 5, 6, 7},
		{5, 4, 0, 1},
	};

	// Set up array with index information
	GLshort indexData[VERT_PER_CUBE];

	#define SET_CUBE_DATA(v) indexData[index++] = faces[f][v];
	int index = 0;
	for (int f=0; f<6; f++) {
		SET_CUBE_DATA(0);
		SET_CUBE_DATA(1);
		SET_CUBE_DATA(2);
		SET_CUBE_DATA(0);
		SET_CUBE_DATA(2);
		SET_CUBE_DATA(3);
	}

	// Vertex Buffer Object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// Index Buffer Object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
}

