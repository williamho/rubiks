#include <Angel.h>
#include "rubiks.h"

// Use a bunch of macros to define a single cube
/*    5-------6
     /|      /|      y
    1-------2 |      |/
    | |     | |    --+--x
    | 4-----|-7     /|
    |/      |/     z
    0-------3             */
#define CUBE_P0  -0.5, -0.5,  0.5,  1.0
#define CUBE_P1  -0.5,  0.5,  0.5,  1.0
#define CUBE_P2   0.5,  0.5,  0.5,  1.0
#define CUBE_P3   0.5, -0.5,  0.5,  1.0
#define CUBE_P4  -0.5, -0.5, -0.5,  1.0
#define CUBE_P5  -0.5,  0.5, -0.5,  1.0
#define CUBE_P6   0.5,  0.5, -0.5,  1.0
#define CUBE_P7   0.5, -0.5, -0.5,  1.0

// Define the vertex order required to create two triangles to form a quad
#define QUAD(a,b,c,d) a,b,c, a,c,d

// Define a numbered cube face based on squares (quads, above) and indices
#define CUBE_FACE(n) QUAD(4*n, 4*n+1, 4*n+2, 4*n+3)

extern GLuint vbo, ibo;
void createVBO() {
	// Vertices of the cube centered around the origin, one face per line
	const GLfloat vertexData[] = {
		CUBE_P4, CUBE_P0, CUBE_P1, CUBE_P5,    
		CUBE_P7, CUBE_P6, CUBE_P2, CUBE_P3,
		CUBE_P4, CUBE_P7, CUBE_P3, CUBE_P0,
		CUBE_P5, CUBE_P1, CUBE_P2, CUBE_P6,    
		CUBE_P4, CUBE_P5, CUBE_P6, CUBE_P7,    
		CUBE_P1, CUBE_P0, CUBE_P3, CUBE_P2,
	};

	const GLshort indexData[] = {
		CUBE_FACE(0),  // Left   (-x)
		CUBE_FACE(1),  // Right  (+x)
		CUBE_FACE(2),  // Bottom (-y)
		CUBE_FACE(3),  // Top    (+y)
		CUBE_FACE(4),  // Back   (-z)
		CUBE_FACE(5),  // Front  (+z)
	};

	// Vertex Buffer Object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// Index Buffer Object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
}

int getColor(int posNum, int faceId) {
	vec3 cubePosition = vec3(
		posNum%3 - 1, 
		(posNum%9)/3 - 1,
		posNum/9 - 1
	);

	// Determine which face of the Rubik's cube this sub-cube is part of
	for (int i=0; i<6; i++) {
		if (cubePosition[i/2] == i%2*2-1 && faceId == i)
			return i;
	}

	// No matches found: this vertex is black
	return -1;
}

void initColors() {
	for (int c=0; c<NUM_CUBES; c++) {
		for (int f=0; f<FACES_PER_CUBE; f++) {
			colors[c][f] = getColor(c,f);
		}
	}
}

