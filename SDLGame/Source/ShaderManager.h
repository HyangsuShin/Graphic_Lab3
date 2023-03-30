#pragma once
#include "SDL.h"

// new OpenGL/GLEW headers:
#define GL3_PROTOTYPES 1
#include "glew.h"
#include "Math.h"
#include "Objects\ObjectData.h"


class ShaderManager
{
public:
	static ShaderManager* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new ShaderManager();
		}
		return instance;
	}

	void Init();

	void Startup();

	void LoadShader(ShaderTypes ID, bool recompiled = false);

	void Render(ShaderTypes shaderID, TextureTypes texID, BlockTypes blockType);

	void Update();

	void Shutdown();

	void LoadShaderFromFile(ShaderTypes ID, const char * shaderfile, GLenum type);
	void LoadVertices(ShaderTypes ID, static const GLfloat CubeVB[], int numVerts);
	void LoadUVs(ShaderTypes ID, static const GLfloat CubeVB[], int numUVs);
	void LoadNormals(ShaderTypes ID, static const GLfloat NormalsVB[], int numNormals);
	void LoadTexture();

private:

	ShaderManager();
	~ShaderManager();

	void ProcessShaderFile(ShaderTypes ID, GLuint shader, char *shaderData, GLenum type);
	void FindUniformHandles(ShaderTypes ID);

	GLuint programObj[NUM_SHADERS];
	GLuint vao[NUM_SHADERS];

	GLint AmbientUniformHandle[NUM_SHADERS];
	GLint TextureUniformHandle[NUM_SHADERS];
	GLint ProjectionUniformHandle[NUM_SHADERS];
	GLint ModelViewUniformHandle[NUM_SHADERS];

	GLuint verticesHandle[NUM_SHADERS];
	GLuint UVHandle[NUM_SHADERS];
	GLuint NormalsHandle[NUM_SHADERS];

	GLuint Textures[NUM_TEXTURES];
	// get camera position
	GLuint CamposUniformHandle[NUM_SHADERS];

	// poisition the skybox
	GLfloat dist[NUM_SHADERS];

	// check if it is grass block
	GLuint BlockTypeUniformHandle[NUM_SHADERS];

	// height of grass
	GLuint HeightUniformHandle[NUM_SHADERS];
	float Height = 0.8;
	float add = 0.001;
	
	// control water transparency
	GLuint TransparencyUniformHandle[NUM_SHADERS];
	float transparency = 0.5;

	// water effect
	GLuint WaterUniformHandle[NUM_SHADERS];
	float water = 1.0;

	static ShaderManager* instance;
};

