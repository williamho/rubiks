#include <Angel.h>
#include "rubiks.h"

void createVBO();
GLuint vao, vbo, ibo;
GLuint uOffset;

void init() {
	createVBO();

	// Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	size_t colorDataOffset = sizeof(GLfloat) * 3 * VERT_PER_CUBE_UNIQUE;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

    uOffset = glGetUniformLocation(program, "uOffset");
}

void reshape (int w, int h) {
}

void display() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	// Draw 27 cubes
	for (int x=-1; x<=1; x++) {
		for (int y=-1; y<=1; y++) {
			for (int z=-1; z<=1; z++) {
				glUniform4f(uOffset, x, y, z, 1.0f);
				glDrawElementsBaseVertex(GL_TRIANGLES, VERT_PER_CUBE, GL_UNSIGNED_SHORT, 0, 0);
			}
		}
	}
	//glUniform4f(uOffset, 0.0f, 0.0f, 0.0f, 1.0f);
	//glDrawElements(GL_TRIANGLES, VERT_PER_CUBE, GL_UNSIGNED_SHORT, 0);

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

