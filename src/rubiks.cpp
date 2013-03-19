#include <Angel.h>
#include "rubiks.h"

void createVBO();
GLuint vao, vbo, ibo;
GLuint uCubeNum;

void init() {
	createVBO();

	// Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load shaders
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// Set position and color for single cube
	size_t colorDataOffset = sizeof(GLfloat) * 3 * VERT_PER_CUBE_UNIQUE;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindVertexArray(0);

	// Handle depth info
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

    uCubeNum = glGetUniformLocation(program, "uCubeNum");
}

void reshape (int w, int h) {
}

void display() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	// Draw 27 cubes based on initial cube
	for (int i=0; i<27; i++) {
		glUniform1i(uCubeNum,i);
		glDrawElementsBaseVertex(GL_TRIANGLES, VERT_PER_CUBE, 
			GL_UNSIGNED_SHORT, 0, 0);
	}

	glBindVertexArray(0);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch( key ) {
	case 033: // Esc
	case 'q': 
		exit( EXIT_SUCCESS );
		break;
	}
}

void mouse(int button, int state, int x, int y) {
}

void idle() {
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Rubik's Cube");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

