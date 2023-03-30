#version 430 core

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec2 UVs;
layout(location = 2) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec2 UV;
out vec3 normalVec;
out vec3 lightVec;
out vec4 vposition;

vec3 light_pos = vec3(10.0, 10.0, 10.0);

void main(void)                                                   
{                                             
	UV = UVs;	
	
    // Calculate normal in view-space
    normalVec = mat3(mv_matrix) * normal;

	// new position using the projection and modelview matrices in addition to the position of each vertices
	// we  pass in 300 cubes, each cube is assigned a new gl_InstanceID, based off that we need to determine it's XY & Z
	float cubeX = VertPos.x*2 + (mod(gl_InstanceID , 30));	// use mod 10 then /10 to find offset as we have a 10x10 area
	float cubeY = VertPos.y;			
	float cubeZ = VertPos.z*2 - ((gl_InstanceID/30));      //(mod(gl_InstanceID , 10) /2);			// use mod 10 to determine the depth as we only have 10 depth max

	vec4 P = proj_matrix * mv_matrix * vec4( cubeX, cubeY, cubeZ, 1.0f);
	
	vposition = vec4(cubeX, cubeY, cubeZ, 1.0);

    // Calculate light vector
    lightVec = light_pos;
	
	gl_Position = P;
}         
