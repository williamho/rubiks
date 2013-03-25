#version 150

in vec4 vPosition;
out vec4 color;
uniform mat4 rotationMat; // Rotate the entire Rubik's cube
uniform float scale;      // Scale the entire Rubik's cube
uniform float rotationProgress;   // In the middle of a rotation, 0.0 to 1.0

uniform int positions[27];     

// Arrays of bit-packed ints with individual sub-cube rotation info
uniform int rotations[27];     
uniform int rotationAxes[27];

/** Based on which sub-cube this is, determine color of this vertex */
vec4 getColor() {
	//int cubePosition = positions[gl_InstanceID];
	//int cubePosition = gl_InstanceID;
	int posNum = positions[gl_InstanceID];
	vec3 cubePosition = vec3(
		posNum%3 - 1, 
		(posNum%9)/3 - 1,
		posNum/9 - 1
	);

	// DEBUG -------------------------
	float tmp = (1.0/27)*posNum;
	return vec4(tmp,tmp,tmp,1.0);
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

vec3 unpack(int packed) {
	const int mask = 3;

	return vec3(
		packed & mask,
		(packed >> 2) & mask,
		(packed >> 4) & mask
	);
}

/** Based on which sub-cube this is, determine the angle of rotation */
mat4 getRotation() {
	int r = rotationAxes[gl_InstanceID];
	if (rotationProgress >= 1.0f || r == 0)
		return mat4(1);
	int rotationAxis = (r < 0 ? -r : r) - 1;
	int rotationAngle = (r < 0 ? -90 : 90);

	// For some reason this is necessary (why?)
	if (rotationAxis == 0)
		rotationAngle *= -1;

	float radiansCurr = mix(radians(rotationAngle), 0, rotationProgress);

	float c = cos(radiansCurr);
	float s = sin(radiansCurr);

	switch(rotationAxis) {
	case 0: // Rotate about x axis
		return mat4( 
			1.0,  0.0,  0.0,  0.0,
			0.0,    c,    s,  0.0,
			0.0,   -s,    c,  0.0,
			0.0,  0.0,  0.0,  1.0 
		);
	case 1: // Rotate about y axis
		return mat4( 
			  c,  0.0,   -s,  0.0,
			0.0,  1.0,  0.0,  0.0,
			  s,  0.0,    c,  0.0,
			0.0,  0.0,  0.0,  1.0 
		);
	case 2: // Rotate about z axis
		return mat4( 
			  c,   -s,  0.0,  0.0,
			  s,    c,  0.0,  0.0,
			0.0,  0.0,  1.0,  0.0,
			0.0,  0.0,  0.0,  1.0 
		);
	}
}

void main() {
	// Position the cube based on gl_InstanceID
	vec4 cubeOffset = vec4(
		gl_InstanceID%3 - 1, 
		(gl_InstanceID%9)/3 - 1,
		gl_InstanceID/9 - 1,
		1.0
	);

	//if (rotationProgress >= 1.0f)
		//vPosition = getRotation() * vPosition;

	vPosition += cubeOffset*1.05; // Leave some space between sub-cubes
	vPosition.xyz *= scale;

	if (rotationProgress < 1.0f)
		vPosition = getRotation() * vPosition;

	color = getColor();
	gl_Position = rotationMat * vPosition;
	gl_Position.z *= -1;
} 

