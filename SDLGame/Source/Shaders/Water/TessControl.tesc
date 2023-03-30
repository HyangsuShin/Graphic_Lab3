#version 430 core

layout (vertices = 3) out;

in vec2 UV[];
in vec3 normalVec[];
in vec3 lightVec[];
in vec4 vposition[];

out vec2 patch_UV[];
out vec3 patch_normalVec[];
out vec3 patch_lightVec[];
out vec4 patch_vposition[];

void main(void)
{
	gl_TessLevelInner[0] = 10.0;
	gl_TessLevelOuter[0] = 10.0;
	gl_TessLevelOuter[1] = 10.0;
	gl_TessLevelOuter[2] = 10.0;

	patch_normalVec[gl_InvocationID] = normalVec[gl_InvocationID];
	patch_lightVec[gl_InvocationID] = lightVec[gl_InvocationID];
	patch_UV[gl_InvocationID] = UV[gl_InvocationID];
	patch_vposition[gl_InvocationID] = vposition[gl_InvocationID];

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}