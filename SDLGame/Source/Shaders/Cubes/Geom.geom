#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 UV[];
in vec3 normalVec[];
in vec3 lightVec[];

out vec2 gUV;
out vec3 gnormalVec;
out vec3 glightVec;

uniform float blockType;
uniform float height;

void main(void)                                                   
{

	if(blockType == 0)
	{	
		for (int i =0; i < 3; i++)
		{
			gl_Position = gl_in[i].gl_Position;
			gUV = UV[i];
			gnormalVec = normalVec[i];
			glightVec = lightVec[i];
			EmitVertex();
		}

		gl_Position = gl_in[0].gl_Position + vec4(0, height, 0, 0);
		EmitVertex();
	}
	else
	{
		for (int i =0; i < 3; i++)
		{
			gl_Position = gl_in[i].gl_Position;
			gUV = UV[i];
			gnormalVec = normalVec[i];
			glightVec = lightVec[i];
			EmitVertex();
		}
	}


	EndPrimitive();
}