#version 150

in vec4 vPosition;
out vec4 color;
uniform mat4 rotationMat; // Rotate the entire Rubik's cube
uniform float scale;      // Scale the entire Rubik's cube
uniform float rotationProgress;   // In the middle of a rotation, 0.0 to 1.0

// Arrays of bit-packed ints with individual sub-cube rotation info
uniform int rotations[27];     
uniform mat4 rotationsPrev[27];

/** Based on which sub-cube this is, determine color of this vertex */
vec4 getColor(vec4 cubeOffset) {
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
		if (cubeOffset[i/2] == i%2*2-1 && gl_VertexID/4 == i)
			return faceColors[i];
	}

	// No matches found: this vertex is black
	return vec4(0.0, 0.0, 0.0, 1.0);
}

/** Based on which sub-cube this is, and the uniform rotations/rotationsPrev 
    arrays, determine the angle of rotation of this vertex */
mat4 getRotation() {
	if (rotations[gl_InstanceID] == 0)
		return rotationsPrev[gl_InstanceID];

	// Interpolate rotation based on rotationProgress
	float radiansCurr = mix(
		0, 
		radians(90 * (rotations[gl_InstanceID] > 0 ? -1 : 1)), 
		rotationProgress
	);

	// Calculate rotation matrices
	float c = cos(radiansCurr);
	float s = sin(radiansCurr);
	mat4 r;
	switch(rotations[gl_InstanceID]) {
	case -1: case 1:
		r = mat4( 
			1.0,  0.0,  0.0,  0.0,
			0.0,    c,    s,  0.0,
			0.0,   -s,    c,  0.0,
			0.0,  0.0,  0.0,  1.0 
		);
		break;
	case -2: case 2:
		r = mat4( 
			  c,  0.0,   -s,  0.0,
			0.0,  1.0,  0.0,  0.0,
			  s,  0.0,    c,  0.0,
			0.0,  0.0,  0.0,  1.0 
		);
		break;
	case -3: case 3:
		r = mat4( 
			  c,   -s,  0.0,  0.0,
			  s,    c,  0.0,  0.0,
			0.0,  0.0,  1.0,  0.0,
			0.0,  0.0,  0.0,  1.0 
		);
		break;
	}
	return r*rotationsPrev[gl_InstanceID];
}

void main() {
	// Position the cube based on gl_InstanceID
	int x = gl_InstanceID%3 - 1;
	int y = (gl_InstanceID%9)/3 - 1;
	int z = gl_InstanceID/9 - 1;
	vec4 cubeOffset = vec4(x, y, z, 1.0);

	vPosition += cubeOffset*1.05; // Leave some space between sub-cubes
	vPosition.xyz *= scale;

	color = getColor(cubeOffset);
	gl_Position = rotationMat * getRotation() * vPosition;
} 

