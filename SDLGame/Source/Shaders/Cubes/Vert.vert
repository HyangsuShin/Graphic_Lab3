#version 430 core

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec2 UVs;
layout(location = 2) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec2 UV;
out vec3 normalVec;
out vec3 lightVec;
//JD 1: make sure the geometry shader ins match the vert outs

vec3 light_pos = vec3(10.0, 10.0, 10.0);

void main(void)                                                   
{                                             
	UV = UVs;	
	
    // Calculate normal in view-space
    normalVec = mat3(mv_matrix) * normal;
	
	float cubeX = VertPos.x * 2;
	float cubeY = VertPos.y * 2;	
	float cubeZ = VertPos.z * -2;

	vec4 P = proj_matrix * mv_matrix * vec4( cubeX, cubeY, cubeZ, 1.0f);
	
    // Calculate light vector
    lightVec = light_pos;
	
	gl_Position = P;
}         
