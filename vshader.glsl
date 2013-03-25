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
uniform int colors[6];

/** Based on which sub-cube this is, determine color of this vertex */
vec4 getColor() {
	const vec4 faceColors[6] = {
		vec4(1.0, 0.5, 0.0, 1.0), // Left   (-x) Orange
		vec4(1.0, 0.0, 0.0, 1.0), // Right  (+x) Red
		vec4(0.0, 1.0, 0.0, 1.0), // Bottom (-y) Green
		vec4(1.0, 1.0, 0.0, 1.0), // Top    (+y) Yellow
		vec4(0.0, 0.0, 1.0, 1.0), // Back   (-z) Blue
		vec4(1.0, 1.0, 1.0, 1.0), // Front  (+z) White
	};

	int colorIndex = colors[gl_VertexID/4];
	if (colorIndex >= 0)
		return faceColors[colorIndex];
	else
		return vec4(0.0, 0.0, 0.0, 1.0);
}

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
	if (rotationAxis != 0)
		rotationAngle *= -1;

	float radiansCurr = mix(0, radians(rotationAngle), rotationProgress);

	switch(rotationAxis) {
	case 0: return rotateX(radiansCurr);
	case 1: return rotateY(radiansCurr);
	case 2: return rotateZ(radiansCurr);
	}
}

void main() {
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

