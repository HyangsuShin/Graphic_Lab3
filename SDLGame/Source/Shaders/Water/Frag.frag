#version 430 core

out vec4 color;                                                   

in vec2 teUV;
in vec3 tenormalVec;
in vec3 telightVec;
in vec4 tevposition;

uniform sampler2D texture0;
uniform vec3 ambientLight = vec3(0.2,0.2,0.2);
uniform vec3 diffuse_albedo = vec3(1.0, 1.0, 1.0);

uniform vec3 camPos;
uniform float transparency;

void main(void)                                                   
{   
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(tenormalVec);
    vec3 L = normalize(telightVec);

	//the dot product of the normals and the light vector identifies whether the angle to the light is direct or not.
	float Reflection = dot(N, L);

	// Compute the diffuse based off the materials properties and the amount the face is visible to the light
    vec3 diffuse = max(Reflection, 0.0) * diffuse_albedo;

	//color = texture( texture0, teUV )* vec4(diffuse + ambientLight, 1.0f);
	color = texture( texture0, teUV) * vec4(diffuse + ambientLight, 1.0f);

	color.a = transparency;
}                                 