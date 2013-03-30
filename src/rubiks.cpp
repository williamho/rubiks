#include <Angel.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "rubiks.h"

GLuint vao, vbo, ibo;
GLuint uRotationMat, uScale, uRotationAxes, 
	uRotationProgress, uPositions, uRotatingSlice, uCubeId, uColors;
mat4 rotationMat;
GLfloat scale = INITIAL_SCALE;
GLint colors[NUM_CUBES][FACES_PER_CUBE];
GLint lineColors[FACES_PER_CUBE];
GLint rotationAxes[NUM_CUBES];
GLint cubeId;
GLint positions[NUM_CUBES];
GLfloat rotationProgress = 1.0;
bool finishedRotating=true;
int rotationStartTime;

vec2 mousePosPrev;
bool leftMousePressed;
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
	glDepthRange(0.0, 1.0);

	// Set up uniform variables
	uRotationMat = glGetUniformLocation(program, "rotationMat");
	uScale = glGetUniformLocation(program, "scale");
	uPositions = glGetUniformLocation(program, "positions");
	uCubeId = glGetUniformLocation(program, "cubeId");
	uColors = glGetUniformLocation(program, "colors");
	uRotationAxes = glGetUniformLocation(program, "rotationAxes");
	uRotationProgress = glGetUniformLocation(program, "rotationProgress");

	glutPostRedisplay();
	glutSwapBuffers();
}

// TODO: keep aspect ratio of cube if window size is not square
void reshape (int w, int h) {
	glViewport(0, 0, w, h);
	winWidth = w;
	winHeight = h;
}

/** Check if the cube is still rotating. If it just finished, update the cubes. */
void updateRotationProgress() {
	if (IS_ROTATING) 
		rotationProgress = ((float)glutGet(GLUT_ELAPSED_TIME)-rotationStartTime)/ROTATION_DURATION;
	if (rotationProgress >= 1.0 && !finishedRotating) {
		rotationProgress = 1.0;
		updateCubes();
	}
}

void display() {
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindVertexArray(vao);

	// Pass info about the rotation/scale of the entire Rubik's cube
	glUniformMatrix4fv(uRotationMat, 1, 0, rotationMat);
	glUniform1f(uScale,scale);

	updateRotationProgress();

	// Let the vertex shader handle all the angle calculations
	glUniform1f(uRotationProgress,rotationProgress);

	// Draw 27 cubes based on initial cube
	glClearStencil(0);
	glStencilMask(0xFF);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	for (int i=0; i<NUM_CUBES; i++) {
		// Stencil buffer:
		// http://en.wikibooks.org/wiki/OpenGL_Programming/Object_selection
		glStencilFunc(GL_ALWAYS, i+1, 0xFF); // i+1 because 0 is used for background

		glUniform1i(uCubeId,i);
		glUniform1i(uPositions,positions[i]);
		glUniform1iv(uColors,FACES_PER_CUBE,colors[i]);
		glUniform1i(uRotationAxes,rotationAxes[i]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, VERT_PER_CUBE, GL_UNSIGNED_SHORT, 0); 
	}

	glBindVertexArray(0);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 033: // Esc
	case 'q': 
		exit(EXIT_SUCCESS);
		break;
	case 'w': // Write
		saveState("savestate.rubiks");
		break;
	case 'e': // Edit
		loadState("savestate.rubiks");
		break;
	case 'r': // Reset
		rotationMat = mat4();
		break;

	// Scene rotation with arrow keys
	case 'k':
		rotationMat *= RotateX(ROTATION_FACTOR_KEYBOARD);
		break;
	case 'j':
		rotationMat *= RotateX(-ROTATION_FACTOR_KEYBOARD);
		break;
	case 'l':
		rotationMat *= RotateZ(ROTATION_FACTOR_KEYBOARD);
		break;
	case 'h':
		rotationMat *= RotateZ(-ROTATION_FACTOR_KEYBOARD);
		break;
	
	// Shift + number keys
	case '!': key = 1; break;
	case '@': key = 2; break;
	case '#': key = 3; break;
	case '$': key = 4; break;
	case '%': key = 5; break;
	case '^': key = 6; break;
	case '&': key = 7; break;
	case '*': key = 8; break;
	case '(': key = 9; break;
	}
	if (key <= 9) {
		key--;
		rotateSlice(positions,key/3,key%3,false);
	}
	if (key > '0' && key <='9') {
		int p = key - '1';
		rotateSlice(positions,p/3,p%3,true);
	}
}

void mouseButton(int button, int state, int x, int y) {
	int index;
	// Rotate slices of cube with left button
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		glReadPixels(x, winHeight-y-1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
		index--;
		rotateFaceFromCube(index);
	}
	// Rotate scene with right mouse 
	else if(button == GLUT_RIGHT_BUTTON) {
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
	GLuint index;

	// If right mouse pressed, changes in mouse position will rotate scene
	if (rightMousePressed) {
		vec2 d = mousePos - mousePosPrev;
		d *= -ROTATION_FACTOR_MOUSE;
		rotationMat *= RotateY(d[0]);
		rotationMat *= RotateX(d[1]);
		mousePosPrev = mousePos;
	} 
}

void idle() {
	// DEBUG: Display framerate in window title
	//static char windowTitle[20]; 
	//sprintf(windowTitle, "%.1f", calculateFPS());
	//glutSetWindowTitle(windowTitle);

	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
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
	initColors();

	// Initialize window 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Rubik's Cube");
	
	glewExperimental = GL_TRUE;
	glewInit();
	init();

	// Register callback functions
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutIdleFunc(idle);

	srand(time(0));
	long numRotations;
	if (argc > 1) {
		numRotations = strtol(argv[1],0,10);
		if (errno == ERANGE) 
			std::cerr << "Invalid argument" << std::endl;
		else 
			randomRotations(numRotations);
	}

	glutMainLoop();
	return 0;
}

