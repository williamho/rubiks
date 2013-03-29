#ifndef RUBIKS_H
#define RUBIKS_H
#include <Angel.h>

#define VERT_PER_CUBE 36
#define UNIQUE_VERT_PER_CUBE 24
#define FACES_PER_CUBE 6
#define CUBES_PER_PLANE 9
#define NUM_CUBES 27
#define NUM_PLANES 9
#define ROTATION_DURATION 200 // Milliseconds
#define MAX_SELECTED_CUBES 3

#define INITIAL_ROTATION 45.0, 45.0, -45.0
#define ROTATION_FACTOR_MOUSE  0.5
#define ROTATION_FACTOR_KEYBOARD 4.0

#define INITIAL_SCALE    0.5
#define MIN_SCALE        0.05
#define MAX_SCALE        0.75
#define SCALE_FACTOR     0.05 // Amount to zoom in/out for each scroll wheel tick

// Function prototypes (could split these into multiple header files)
void display();
void init();
void reshape (int w, int h);
void display();
void keyboard(unsigned char key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void updateRotationProgress();

void createVBO();
float calculateFPS();
int findPlane(int cubes[MAX_SELECTED_CUBES]);
void rotateSlice(int cubes[], int axis, int n, bool isClockwise=true);
void randomRotations(int numRotations);
void initColors();
void updateCubes();

void saveState(char *filename);
void loadState(char *filename);
bool isSolved();
bool cubeIsSelected(int cubeNum);

// Globals
extern int winWidth, winHeight;
extern GLint positions[NUM_CUBES];
extern GLint rotationAxes[NUM_CUBES];
extern GLfloat rotationProgress;
extern GLint lineColors[FACES_PER_CUBE];
extern GLint colors[NUM_CUBES][FACES_PER_CUBE];
extern int rotationStartTime;
extern bool finishedRotating;
extern int selectedCubesIndex;
extern GLint selectedCubes[MAX_SELECTED_CUBES];

// Positions on the cube that correspond to planes
const int planes[9][9] = { 
	// Planes x=-1 to x=+1
	{  0,  3,  6,  9, 12, 15, 18, 21, 24},
	{  1,  4,  7, 10, 13, 16, 19, 22, 25},
	{  2,  5,  8, 11, 14, 17, 20, 23, 26},
	// Planes y=-1 to y=+1
	{  0,  1,  2,  9, 10, 11, 18, 19, 20},
	{  3,  4,  5, 12, 13, 14, 21, 22, 23},
	{  6,  7,  8, 15, 16, 17, 24, 25, 26},
	// Planes z=-1 to z=+1
	{  0,  1,  2,  3,  4,  5,  6,  7,  8}, 
	{  9, 10, 11, 12, 13, 14, 15, 16, 17},
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26},
};

#define IS_ROTATING (rotationProgress < 1.0f)

#endif

