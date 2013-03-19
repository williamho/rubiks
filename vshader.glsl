#version 150

in vec4 vPosition;
in vec4 vColor;
uniform vec4 uOffset;
uniform uint uCubeNum;
out vec4 color;

vec4 getColor() {
	switch(gl_VertexID) {
	case 0:
		return vec4(0,0,0,1);
		break;
	case 1:
		return vec4(0,0,1,1);
		break;
	case 2:
		return vec4(0,1,0,1);
		break;
	case 3:
		return vec4(0,1,1,1);
		break;
	case 4:
		return vec4(1,0,0,1);
		break;
	case 5:
		return vec4(1,0,1,1);
		break;
	case 6:
		return vec4(1,1,0,1);
		break;
	case 7:
		return vec4(1,1,1,1);
		break;
	}
}

void main() {
	vPosition += uOffset*2.1;
	vPosition.xyz *= 0.25;

	vec3 theta = vec3(45,45,45);
	vec3 angles = radians(theta);
	vec3 c = cos(angles);
	vec3 s = sin(angles);

	// Remeber: these matrices are column-major
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

