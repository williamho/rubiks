#include <Angel.h>
#include "rubiks.h"

GLuint vao, vbo, ibo;
GLuint uRotation, uScale;
vec2 mousePosPrev;
GLfloat scale = INITIAL_SCALE;
mat4 rotation;
bool rightMousePressed;

int winWidth = 640, winHeight = 640;

void init() {
	createVBO();

	// Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load shaders
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// Set position and color for single cube
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindVertexArray(0);

	// Handle depth info
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	uRotation = glGetUniformLocation(program, "rotation");
	uScale = glGetUniformLocation(program, "scale");
}

void reshape (int w, int h) {
	glViewport(0, 0, w, h);
	winWidth = w;
	winHeight = h;
}

void display() {
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	// Set uniform variables and draw 27 cubes based on initial cube
	glUniformMatrix4fv(uRotation, 1, 0, rotation);
	glUniform1f(uScale,scale);
	glDrawElementsInstanced(GL_TRIANGLES, VERT_PER_CUBE, GL_UNSIGNED_SHORT, 0, NUM_CUBES);

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

void mouseButton(int button, int state, int x, int y) {
	// Rotate scene with right mouse 
	if(button == GLUT_RIGHT_BUTTON) {
		switch(state) {
		case GLUT_DOWN:
			mousePosPrev = vec2(x,y);
			rightMousePressed = true;
			break;
		case GLUT_UP:
			rightMousePressed = false;
			break;
		}
	}
	// Zoom with scroll wheel
	else if (button == 3 || button == 4) { 
		if (state == GLUT_UP) // Disregard redundant GLUT_UP events
			return;
		scale += (button == 3 ? 1 : -1) * SCALE_FACTOR; // UP to zoom in
		if (scale < MIN_SCALE) scale = MIN_SCALE;
		if (scale > MAX_SCALE) scale = MAX_SCALE;
	}
}

void mouseMotion(int x, int y) {
	vec2 mousePos = vec2(x,y);
	// If right mouse pressed, changes in mouse position will rotate scene
	if(rightMousePressed) {
		vec2 d = mousePos - mousePosPrev;
		d *= ROTATION_FACTOR;
		rotation *= RotateY(d[0]);
		rotation *= RotateX(d[1]);
		mousePosPrev = mousePos;
	} 
}

void idle() {
	// DEBUG: Display framerate in window title
	static char windowTitle[20]; 
	sprintf(windowTitle, "%.1f", calculateFPS());
	glutSetWindowTitle(windowTitle);

	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	// Initialize window 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Rubik's Cube");
	
	glewInit();
	init();

	// Register callback functions
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutIdleFunc(idle);

	// Set up rotation matrix for entire scene
	rotation = mat4();
	vec3 r(INITIAL_ROTATION);
	rotation *= RotateX(r[0]);
	rotation *= RotateY(r[1]);
	rotation *= RotateZ(-r[2]);

	glutMainLoop();
	return 0;
}

