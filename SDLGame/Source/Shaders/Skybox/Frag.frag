#version 430 core

out vec4 color;                                                   

in vec2 UV;

uniform sampler2D texture0;

void main(void)                                                   
{     
	color = texture(texture0, UV);
}    