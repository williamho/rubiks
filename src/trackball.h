#ifndef TRACKBALL_H
#define TRACKBALL_H
#include <Angel.h>

extern int startX, startY;
extern bool trackingMouse;
extern vec3 axis, lastPos;
extern float trackballAngle;

vec3 getTrackballPos(int x, int y, int width, int height);
void mouseMotion(int x, int y);

#endif

