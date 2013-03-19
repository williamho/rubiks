#version 150

in vec4 vPosition;
uniform int uCubeNum;
out vec4 color;

vec4 getColor() {
	/*   5-------6
	    /|      /|      y
	   1-------2 |      |/
	   | |     | |    --+--x
	   | 4-----|-7     /|
	   |/      |/     z
	   0-------3             */

	int face[9] = int[9](0,0,0,0,0,0,0,0,0);
	if (uCubeNum%3 == 0) // Left face
		face[0] = 1;
	if (uCubeNum%9 < 3) // Front face
		face[1] = 1;
	

	switch(gl_VertexID) {
	case 0:
		if (face[0] > 0)
			return vec4(0,0,1,1);
		if (face[1] > 0)
			return vec4(1,0,0,1);
		break;
	case 1:
		if (face[0] > 0)
			return vec4(0,0,1,1);
		if (face[1] > 0)
			return vec4(1,0,0,1);
		break;
	case 2:
		if (face[1] > 0)
			return vec4(1,0,0,1);
		break;
	case 3:
		if (face[1] > 0)
			return vec4(1,0,0,1);
		break;
	case 4:
		if (face[0] > 0)
			return vec4(0,0,1,1);
		break;
	case 5:
		if (face[0] > 0)
			return vec4(0,0,1,1);
		break;
	case 6:
		break;
	case 7:
		break;
	}
	return vec4(0,0,0,1);
}

void main() {
	// Position the cube based on uCubeNum
	int x = uCubeNum%3 - 1;
	int y = (uCubeNum%9)/3 - 1;
	int z = uCubeNum/9 - 1;
	vec4 cubeOffset = vec4(x, y, z, 1.0);

	vPosition += cubeOffset*2.1;

	// Temporary scaling by constant factor
	// TODO: replace with a uniform
	vPosition.xyz *= 0.25;
	vec3 theta = vec3(45,45,45);

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

	//color = vColor;
	color = getColor();
	gl_Position = rz * ry * rx * vPosition;
} 

