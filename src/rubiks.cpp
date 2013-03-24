#include <Angel.h>
#include "rubiks.h"

GLuint vao, vbo, ibo;
GLuint uRotationMat, uScale, uRotations, uRotationsPrev, uRotationProgress, uPositions;
mat4 rotationMat;
GLfloat scale = INITIAL_SCALE;
GLint rotations[NUM_CUBES];
GLint rotationsPrev[NUM_CUBES];
int positions[NUM_CUBES];
GLfloat rotationProgress = 1.0f;
int rotationStartTime;

vec2 mousePosPrev;
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

	uRotationMat = glGetUniformLocation(program, "rotationMat");
	uScale = glGetUniformLocation(program, "scale");
	uPositions = glGetUniformLocation(program, "positions");
	uRotations = glGetUniformLocation(program, "rotations");
	uRotationsPrev = glGetUniformLocation(program, "rotationsPrev");
	uRotationProgress = glGetUniformLocation(program, "rotationProgress");
}

// TODO: keep aspect ratio of cube if window size is not square
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

	// Pass info about the rotation/scale of the entire Rubik's cube
	glUniformMatrix4fv(uRotationMat, 1, 0, rotationMat);
	glUniform1f(uScale,scale);

	if (IS_ROTATING) 
		rotationProgress = ((float)glutGet(GLUT_ELAPSED_TIME)-rotationStartTime)/ROTATION_DURATION;
	if (rotationProgress > 1.0f)
		rotationProgress = 1.0f;

	// Let the vertex shader handle all the angle calculations
	glUniform1f(uRotationProgress,rotationProgress);
	glUniform1iv(uPositions,NUM_CUBES,positions);
	glUniform1iv(uRotations,NUM_CUBES,rotations);
	glUniform1iv(uRotationsPrev,NUM_CUBES,rotationsPrev);

	// Draw 27 instanced cubes based on initial cube
	glDrawElementsInstanced(
		GL_TRIANGLES, 
		VERT_PER_CUBE, 
		GL_UNSIGNED_SHORT, 
		0, 
		NUM_CUBES
	);

	glBindVertexArray(0);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 033: // Esc
	case 'q': 
		exit( EXIT_SUCCESS );
		break;
	}
	if (key > '0' && key <='9') {
		int p = key - '1';
		rotatePlane(positions,p/3,p%3,true);
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
		rotationMat *= RotateY(d[0]);
		rotationMat *= RotateX(d[1]);
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
	glewExperimental = GL_TRUE;

	// Initialize window 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(winWidth, winHeight);
	glutInitContextVersion(3, 1);
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
	rotationMat = mat4();
	vec3 r(INITIAL_ROTATION);
	rotationMat *= RotateX(r[0]);
	rotationMat *= RotateY(r[1]);
	rotationMat *= RotateZ(-r[2]);
	
	// Initialize cube positions array to default positions
	// if positions[3] == 6, this means cube instance #6 is at position #3.
	for (int i=0; i<NUM_CUBES; i++) 
		positions[i] = i;

	glutMainLoop();
	return 0;
}

