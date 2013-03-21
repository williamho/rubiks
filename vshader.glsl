#version 150

in vec4 vPosition;
out vec4 color;
uniform vec3 theta;

vec4 getColor(vec4 cubeOffset) {
	/*   5-------6
	    /|      /|      y
	   1-------2 |      |/
	   | |     | |    --+--x
	   | 4-----|-7     /|
	   |/      |/     z
	   0-------3             */

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
	return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {
	// Position the cube based on gl_InstanceID
	int x = gl_InstanceID%3 - 1;
	int y = (gl_InstanceID%9)/3 - 1;
	int z = gl_InstanceID/9 - 1;
	vec4 cubeOffset = vec4(x, y, z, 1.0);

	vPosition += cubeOffset*1.1;

	// Temporary scaling by constant factor
	// TODO: replace with a uniform
	vPosition.xyz /= 2;

	vec3 angles = radians(theta);
	vec3 c = cos(angles);
	vec3 s = sin(angles);

	// Remember: these matrices are column-major
	mat4 rx = mat4( 
		1.0,  0.0,  0.0, 0.0,
		0.0,  c.x,  s.x, 0.0,
		0.0, -s.x,  c.x, 0.0,
		0.0,  0.0,  0.0, 1.0 
	);

	mat4 ry = mat4( 
		c.y, 0.0, -s.y, 0.0,
		0.0, 1.0,  0.0, 0.0,
		s.y, 0.0,  c.y, 0.0,
		0.0, 0.0,  0.0, 1.0 
	);

	// Workaround for bug in ATI driver
	ry[1][0] = 0.0;
	ry[1][1] = 1.0;

	mat4 rz = mat4( 
		c.z, -s.z, 0.0, 0.0,
		s.z,  c.z, 0.0, 0.0,
		0.0,  0.0, 1.0, 0.0,
		0.0,  0.0, 0.0, 1.0 
	);

	// Workaround for bug in ATI driver
	rz[2][2] = 1.0;

	color = getColor(cubeOffset);
	gl_Position = rz * ry * rx * vPosition;
} 

