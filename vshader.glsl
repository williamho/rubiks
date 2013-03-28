#version 150

in vec4 vPosition;
out vec4 color;
uniform mat4 rotationMat; // Rotate the entire Rubik's cube
uniform float scale;      // Scale the entire Rubik's cube
uniform float rotationProgress;   // In the middle of a rotation, 0.0 to 1.0

uniform int cubeId;
uniform int positions;     

uniform int rotationAxes;
uniform int colors[6];

/** Based on which sub-cube this is, determine color of this vertex */
vec4 getColor() {
	const vec4 faceColors[6] = vec4[] (
		vec4(1.0, 0.5, 0.0, 1.0), // Left   (-x) Orange
		vec4(1.0, 0.0, 0.0, 1.0), // Right  (+x) Red
		vec4(0.0, 1.0, 0.0, 1.0), // Bottom (-y) Green
		vec4(1.0, 1.0, 0.0, 1.0), // Top    (+y) Yellow
		vec4(0.0, 0.0, 1.0, 1.0), // Back   (-z) Blue
		vec4(1.0, 1.0, 1.0, 1.0)  // Front  (+z) White
	);

	// There are 24 vertices per cube, and each group of 4 vertices defines a face
	// Thus, gl_VertexID/4 (int division) will give the face the vertex belongs to
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

/** Based on the rotationProgress, interpolate between 0 and 90 degrees
	of rotation for the specified 'slice' (plane) of the Rubik's cube */
mat4 getSliceRotation() {
	int r = rotationAxes;
	if (rotationProgress >= 1.0f || r == 0) // This subcube not rotating
		return mat4(1);
	int rotationAxis = (r < 0 ? -r : r) - 1;
	int rotationAngle = (r < 0 ? -90 : 90);

	// For some reason this is necessary (not entirely sure why)
	if (rotationAxis != 0)
		rotationAngle *= -1;

	float radiansCurr = mix(0, radians(float(rotationAngle)), rotationProgress);

	switch(rotationAxis) {
	case 0: return rotateX(radiansCurr);
	case 1: return rotateY(radiansCurr);
	case 2: return rotateZ(radiansCurr);
	}
}

void main() {
	vec4 p = vPosition;

	// Position each subcube based on its cubeId, leave some space between
	p += 1.05 * vec4(	
		cubeId%3 - 1, 
		(cubeId%9)/3 - 1,
		cubeId/9 - 1,
		1.0
	);

	p.xyz *= scale;

	// If a slice of the Rubik's cube is rotating, get the rotation matrix
	if (rotationProgress < 1.0f)
		p = getSliceRotation() * p;
	color = getColor();

	// Rotate the entire Rubik's cube
	gl_Position = rotationMat * p;
	gl_Position.z *= -1;
} 

