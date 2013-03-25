#version 150

in vec4 vPosition;
out vec4 color;
uniform mat4 rotationMat; // Rotate the entire Rubik's cube
uniform float scale;      // Scale the entire Rubik's cube
uniform float rotationProgress;   // In the middle of a rotation, 0.0 to 1.0

uniform int cubeId;
uniform int positions;     

uniform mat4 rotations;     
uniform int rotationAxes;

/** Based on which sub-cube this is, determine color of this vertex */
vec4 getColor() {
	int posNum = positions;
	//int posNum = cubeId;
	//int posNum = gl_InstanceID;
	vec3 cubePosition = vec3(
		posNum%3 - 1, 
		(posNum%9)/3 - 1,
		posNum/9 - 1
	);

	// DEBUG -------------------------
	//float tmp = (1.0/27)*posNum;
	//return vec4(tmp,tmp,tmp,1.0);
	// -------------------------------

	const vec4 faceColors[6] = {
		vec4(1.0, 0.5, 0.0, 1.0), // Left   (-x) Orange
		vec4(1.0, 0.0, 0.0, 1.0), // Right  (+x) Red
		vec4(0.0, 1.0, 0.0, 1.0), // Bottom (-y) Green
		vec4(1.0, 1.0, 0.0, 1.0), // Top    (+y) Yellow
		vec4(0.0, 0.0, 1.0, 1.0), // Back   (-z) Blue
		vec4(1.0, 1.0, 1.0, 1.0), // Front  (+z) White
	};

	// Determine which face of the Rubik's cube this sub-cube is part of
	for (int i=0; i<6; i++) {
		if (cubePosition[i/2] == i%2*2-1 && gl_VertexID/4 == i)
			return faceColors[i];
	}

	// No matches found: this vertex is black
	return vec4(0.0, 0.0, 0.0, 1.0);
}

/*
vec3 unpack(int packed) {
	const int mask = 3;

	return vec3(
		packed & mask,
		(packed >> 2) & mask,
		(packed >> 4) & mask
	);
}
*/

mat4 rotateX(float angleRadians) {
	float c = cos(angleRadians);
	float s = sin(angleRadians);
	return mat4( 
		1.0,  0.0,  0.0,  0.0,
		0.0,    c,    s,  0.0,
		0.0,   -s,    c,  0.0,
		0.0,  0.0,  0.0,  1.0 
	);
}

mat4 rotateY(float angleRadians) {
	float c = cos(angleRadians);
	float s = sin(angleRadians);

	return mat4( 
		  c,  0.0,   -s,  0.0,
		0.0,  1.0,  0.0,  0.0,
		  s,  0.0,    c,  0.0,
		0.0,  0.0,  0.0,  1.0 
	);
}

mat4 rotateZ(float angleRadians) {
	float c = cos(angleRadians);
	float s = sin(angleRadians);

	return mat4( 
		  c,   -s,  0.0,  0.0,
		  s,    c,  0.0,  0.0,
		0.0,  0.0,  1.0,  0.0,
		0.0,  0.0,  0.0,  1.0 
	);
}

mat4 getSliceRotation() {
	int r = rotationAxes;
	if (rotationProgress >= 1.0f || r == 0)
		return mat4(1);
	int rotationAxis = (r < 0 ? -r : r) - 1;
	int rotationAngle = (r < 0 ? -90 : 90);

	// For some reason this is necessary (why?)
	if (rotationAxis == 0)
		rotationAngle *= -1;

	float radiansCurr = mix(radians(rotationAngle), 0, rotationProgress);

	switch(rotationAxis) {
	case 0: // Rotate about x axis
		return rotateX(radiansCurr);
	case 1: // Rotate about y axis
		return rotateY(radiansCurr);
	case 2: // Rotate about z axis
		return rotateZ(radiansCurr);
	}
}

// TESTING???
mat4 getCubeRotation() {
	return rotations;
	int r = rotationAxes;
	if (rotationProgress < 1.0f || r == 0)
		return mat4(1);
	int rotationAxis = (r < 0 ? -r : r) - 1;
	int rotationAngle = (r < 0 ? -90 : 90);

	float radiansCurr = radians(rotationAngle);

	mat4 rm;	
	switch(rotationAxis) {
	case 0: // Rotate about x axis
		rm = rotateX(radiansCurr); 
		break;
	case 1: // Rotate about y axis
		rm = rotateY(radiansCurr); 
		break;
	case 2: // Rotate about z axis
		rm = rotateZ(radiansCurr); 
		break;
	}

	return rm * rotations;
}

void main() {
	if (rotationAxes != 0)
		vPosition = getCubeRotation() * vPosition;

	// Position the cube based on gl_InstanceID
	vPosition += 1.05 * vec4(
		cubeId%3 - 1, 
		(cubeId%9)/3 - 1,
		cubeId/9 - 1,
		1.0
	);

	vPosition.xyz *= scale;

	if (rotationProgress < 1.0f)
		vPosition = getSliceRotation() * vPosition;

	color = getColor();
	gl_Position = rotationMat * vPosition;
	gl_Position.z *= -1;
} 

