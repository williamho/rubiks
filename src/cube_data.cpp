#include "Angel.h"

GLuint vbo, ibo;
void createVBO() {
	// Vertices of the cube centered around the origin, one face per line
	//const GLfloat vertexData[] = {
		//-1, -1, -1,  1,   -1, -1,  1,  1,   -1,  1,  1,  1,    -1,  1, -1,  1,    
		  //1, -1, -1,  1,    1, -1,  1,  1,    1,  1,  1,  1,     1,  1, -1,  1,    
		//-1, -1, -1,  1,   -1, -1,  1,  1,    1, -1,  1,  1,     1, -1, -1,  1,    
		//-1,  1, -1,  1,   -1,  1,  1,  1,    1,  1,  1,  1,     1,  1, -1,  1,    
		//-1, -1, -1,  1,   -1,  1, -1,  1,    1,  1, -1,  1,     1, -1, -1,  1,    
		//-1, -1,  1,  1,   -1,  1,  1,  1,    1,  1,  1,  1,     1, -1,  1,  1,    
	//};
	const GLfloat vertexData[] = {
		-0.5, -0.5, -0.5,  1.0,   -0.5, -0.5,  0.5,  1.0,   -0.5,  0.5,  0.5,  1.0,    -0.5,  0.5, -0.5,  1.0,    
	 	 0.5, -0.5, -0.5,  1.0,    0.5, -0.5,  0.5,  1.0,    0.5,  0.5,  0.5,  1.0,     0.5,  0.5, -0.5,  1.0,    
		-0.5, -0.5, -0.5,  1.0,   -0.5, -0.5,  0.5,  1.0,    0.5, -0.5,  0.5,  1.0,     0.5, -0.5, -0.5,  1.0,    
		-0.5,  0.5, -0.5,  1.0,   -0.5,  0.5,  0.5,  1.0,    0.5,  0.5,  0.5,  1.0,     0.5,  0.5, -0.5,  1.0,    
		-0.5, -0.5, -0.5,  1.0,   -0.5,  0.5, -0.5,  1.0,    0.5,  0.5, -0.5,  1.0,     0.5, -0.5, -0.5,  1.0,    
		-0.5, -0.5,  0.5,  1.0,   -0.5,  0.5,  0.5,  1.0,    0.5,  0.5,  0.5,  1.0,     0.5, -0.5,  0.5,  1.0,    
	};

	// Colors: Corresponding to the vertices above
	const GLfloat colorData[] = {
		0, 0, 0, 1,   0, 0, 1, 1,   0, 1, 1, 1,   0, 1, 0, 1,
		1, 0, 0, 1,   1, 0, 1, 1,   1, 1, 1, 1,   1, 1, 0, 1,
		0, 0, 0, 1,   0, 0, 1, 1,   1, 0, 1, 1,   1, 0, 0, 1,
		0, 1, 0, 1,   0, 1, 1, 1,   1, 1, 1, 1,   1, 1, 0, 1,
		0, 0, 0, 1,   0, 1, 0, 1,   1, 1, 0, 1,   1, 0, 0, 1,
		0, 0, 1, 1,   0, 1, 1, 1,   1, 1, 1, 1,   1, 0, 1, 1,
	};

	const GLshort indexData[] = {
		 0,  1,  2,  0,  2,  3,
		 4,  5,  6,  4,  6,  7,
		 8,  9, 10,  8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};

	// Vertex Buffer Object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData)+sizeof(colorData), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), vertexData);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexData), sizeof(colorData), colorData);

	// Index Buffer Object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
}

