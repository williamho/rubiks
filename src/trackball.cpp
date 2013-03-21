#include <Angel.h>
#include <cmath>
#include "rubiks.h"

vec3 lastPos = vec3(0.0f, 0.0f, 0.0f);
float trackballAngle = 45.0f;

bool trackingMouse = false;

vec3 getTrackballPos(int x, int y, int width, int height) {
	float d;
	vec3 v;

	// Project (x,y) onto a hemisphere centered within (width,height) 
	v[0] = (2.0f*x - width) / width;
	v[1] = (height - 2.0f*y) / height;
	d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
	v[2] = (float) cos((M_PI/2.0f) * ((d < 1.0f) ? d : 1.0f));
	v /= (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

	return v;
}

void mouseMotion(int x, int y) {
	vec3 curPos = getTrackballPos(x, y, winWidth, winHeight);
	//printf("v (%f,%f,%f)\n",curPos[0],curPos[1],curPos[2]);
	vec3 d;
	if(trackingMouse) {
		d = curPos - lastPos;
		if (d[0] || d[1] || d[2]) {
			trackballAngle = 90.0f * sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
			theta += 100*cross(lastPos,curPos);
			lastPos = curPos;
		}
	} 
}

