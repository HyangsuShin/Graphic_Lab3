#version 430 core

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec2 UVs;
layout(location = 4) in float dist;

uniform mat4 proj_matrix;
uniform vec3 camPos;

out vec2 UV;

void main(void)                                                   
{                                             
	UV = UVs;	
	
	gl_Position = proj_matrix * vec4( VertPos.x * dist - camPos.x, VertPos.y * dist - camPos.y, VertPos.z * dist - camPos.z, 1.0f);
}