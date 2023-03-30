#include "ShaderManager.h"
#include <cstdio>
#include <iostream>
#include "Utils.h"
#include "vmath.h"
#include "CameraManager.h"
#include "EventHandler.h"
#include "Environment/WorldManager.h"

using namespace std;

ShaderManager* ShaderManager::instance = nullptr;

#define LOG_COMPILE_SHADERS 1

ShaderManager::ShaderManager()
{
	dist[SKY] = 10.0;
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Init()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glewExperimental = GL_TRUE;
}

// function to call post Init
void ShaderManager::Startup()
{
	for (int i = 0; i < NUM_SHADERS; i++)
	{
		programObj[i] = glCreateProgram();
	}
}

void ShaderManager::LoadVertices(ShaderTypes ID, static const GLfloat CubeVB[], int numVerts)
{
	glBindBuffer(GL_ARRAY_BUFFER, verticesHandle[ID]);
	glBufferData(GL_ARRAY_BUFFER, numVerts, CubeVB, GL_STATIC_DRAW);
}

void ShaderManager::LoadUVs(ShaderTypes ID, static const GLfloat uvVB[], int numUVs)
{
	glBindBuffer(GL_ARRAY_BUFFER, UVHandle[ID]);
	glBufferData(GL_ARRAY_BUFFER, numUVs, uvVB, GL_STATIC_DRAW);
}

void ShaderManager::LoadNormals(ShaderTypes ID, static const GLfloat NormalsVB[], int numNormals)
{
	glBindBuffer(GL_ARRAY_BUFFER, NormalsHandle[ID]);
	glBufferData(GL_ARRAY_BUFFER, numNormals, NormalsVB, GL_STATIC_DRAW);
}

void ShaderManager::FindUniformHandles(ShaderTypes ID)
{
	TextureUniformHandle[ID] = glGetUniformLocation(programObj[ID], "texture0");// Get the initial matrices references from the program.

	ModelViewUniformHandle[ID] = glGetUniformLocation(programObj[ID], "mv_matrix");

	ProjectionUniformHandle[ID] = glGetUniformLocation(programObj[ID], "proj_matrix");

	AmbientUniformHandle[ID] = glGetUniformLocation(programObj[ID], "ambientLight");

	CamposUniformHandle[ID] = glGetUniformLocation(programObj[ID], "camPos"); // Get the camPos

	BlockTypeUniformHandle[ID] = glGetUniformLocation(programObj[ID], "blockType"); // determine the blockType

	HeightUniformHandle[ID] = glGetUniformLocation(programObj[ID], "height"); // determine the height

	WaterUniformHandle[ID] = glGetUniformLocation(programObj[ID], "water"); // determine the water

	TransparencyUniformHandle[ID] = glGetUniformLocation(programObj[ID], "transparency"); // control water transparency
}

void ShaderManager::LoadShaderFromFile(ShaderTypes ID, const char * shaderfile, GLenum type)
{
	GLuint shaderObj = 0;
	FILE * filePtr;
	
#ifdef LOG_COMPILE_SHADERS
	string newStr = "Source\\Shaders\\glslangValidator.exe ";
	newStr.append(shaderfile);
	cout << newStr.c_str();
	system(newStr.c_str());
#endif

	if (fopen_s(&filePtr, shaderfile, "rb"))
	{
		cout << "error opening shader file: " << shaderfile << endl;
		return;
	}

	fseek(filePtr, 0, SEEK_END);
	int size = ftell(filePtr);
	fseek(filePtr, 0, SEEK_SET);

	char *data = new char[size + 1];

	if (!data)
	{
		cout << "No Data inside shader file: " << shaderfile << endl;
		return;
	}

	fread(data, 1, size, filePtr);
	data[size] = 0;
	fclose(filePtr);

	ProcessShaderFile(ID, shaderObj, data, type);

	delete[] data;
}

void ShaderManager::ProcessShaderFile(ShaderTypes ID, GLuint shader, char *shaderData, GLenum type)
{

	shader = glCreateShader(type);

	if (!shader)
	{
		cout << "error creating shader: " << shader << endl;
		return;
	}

	glShaderSource(shader, 1, &shaderData, NULL);
	glCompileShader(shader);
	glAttachShader(programObj[ID], shader);
	glDeleteShader(shader);
}

void ShaderManager::LoadShader(ShaderTypes ID, bool recompiled)
{
	// load in the shaders for the specific object type.
	string vertShadername = "Source\\Shaders\\";
	vertShadername.append(gShaderNames[ID]); // JD Create individual shader folders and different shaders for each object type.
	vertShadername.append("\\Vert.vert");

	// load in the shaders for the specific object type.
	string tessControlShadername = "Source\\Shaders\\";
	tessControlShadername.append(gShaderNames[ID]); // JD Create individual shader folders and different shaders for each object type.
	tessControlShadername.append("\\TessControl.tesc");

	// load in the shaders for the specific object type.
	string tessEvalShadername = "Source\\Shaders\\";
	tessEvalShadername.append(gShaderNames[ID]); // JD Create individual shader folders and different shaders for each object type.
	tessEvalShadername.append("\\TessEval.tese");

	//JD 1: Add geometry filename processing here... you will need to create a geometry shader yourself inside of the Cubes shader folder. 
	// check the geometry shader slides for an example of a pass through geometry shader.
	string geomShadername = "Source\\Shaders\\";
	geomShadername.append(gShaderNames[ID]); // JD Create individual shader folders and different shaders for each object type.
	geomShadername.append("\\Geom.geom");

	string fragShadername = "Source\\Shaders\\";
	fragShadername.append(gShaderNames[ID]); // JD Create individual shader folders and different shaders for each object type.
	fragShadername.append("\\Frag.frag");

	LoadShaderFromFile(ID, vertShadername.c_str(), GL_VERTEX_SHADER);

	if (ID == 2)
	{
		LoadShaderFromFile(ID, tessControlShadername.c_str(), GL_TESS_CONTROL_SHADER);
		LoadShaderFromFile(ID, tessEvalShadername.c_str(), GL_TESS_EVALUATION_SHADER);
	}

	//JD 1: load the geometry shader
	if (ID == 0)	//load only at cube
	{
		LoadShaderFromFile(ID, geomShadername.c_str(), GL_GEOMETRY_SHADER);
	}
	
	LoadShaderFromFile(ID, fragShadername.c_str(), GL_FRAGMENT_SHADER);

	glLinkProgram(programObj[ID]);

	if (!recompiled)
	{
		//NUM_SHADERS -> 1: This fix the issue of memory overwriting
		glGenVertexArrays(1, &vao[ID]);
		glBindVertexArray(vao[ID]);
		glGenBuffers(1, &verticesHandle[ID]);
		glGenBuffers(1, &UVHandle[ID]);
		glGenBuffers(1, &NormalsHandle[ID]);
	}
	FindUniformHandles(ID);
	
}

void ShaderManager::Render(ShaderTypes shaderID, TextureTypes texID, BlockTypes blockType)
{
	glUseProgram(programObj[shaderID]);
	glBindVertexArray(vao[shaderID]);

	//pass in the vertices for our cube
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, verticesHandle[shaderID]);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVHandle[shaderID]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// send projection/view matrix into shaders
	vmath::mat4 proj_matrix = CameraManager::GetInstance()->getModifiedProjectionMatrix();
	glUniformMatrix4fv(ProjectionUniformHandle[shaderID], 1, GL_FALSE, proj_matrix);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NormalsHandle[shaderID]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// send lighting
	float LightVals[3] = { 0.2f, 0.2f, 0.2f };
	if (AmbientUniformHandle[shaderID] != -1)
	{
		glUniform3fv(AmbientUniformHandle[shaderID], 1, &LightVals[0]);
	}

	// set texture
	glActiveTexture(GL_TEXTURE1 + texID);
	glBindTexture(GL_TEXTURE_2D, Textures[texID]);
	// pass our data to openGL to store for the shaders use.
	glUniform1i(TextureUniformHandle[shaderID], Textures[(int)texID]);
	
	// send camposition
	Vec3 camPos = CameraManager::GetInstance()->GetCameraPosition();
	float campositions[3] = { camPos.x, camPos.y, camPos.z };
	glUniform3fv(CamposUniformHandle[shaderID], 1, &campositions[0]);

	// send blockType
	float BlockType = blockType;
	glUniform1f(BlockTypeUniformHandle[shaderID], BlockType);

	// send height
	glUniform1f(HeightUniformHandle[shaderID], Height);
	Height += add;
	if (Height > 2 || Height < 0.8)
	{
		add = -add;
	}

	// water plane rendering
	if (shaderID == WATER)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		vmath::mat4 mv_matrix = vmath::translate(-5.0f, -3.0f, -6.0f);
		mv_matrix *= vmath::scale(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(ModelViewUniformHandle[shaderID], 1, GL_FALSE, mv_matrix);

		// water effect
		glUniform1f(WaterUniformHandle[shaderID], water);
		water += 0.01;

		// control water transparency
		glUniform1f(TransparencyUniformHandle[shaderID], transparency);
		if (EventHandler::events[ControlsEvents::RIGHT_PRESSED])
		{
			if (transparency < 1)
			{
				transparency += 0.001;
			}
		}
		else if (EventHandler::events[ControlsEvents::LEFT_PRESSED])
		{
			if (transparency > 0)
			{
				transparency -= 0.001;
			}
		}

		// draw the water planes as an instance the size of the world dimensions
		//glDrawArraysInstanced(GL_TRIANGLES, 0, 4 * 3, WorldManager::WORLD_WIDTH * WorldManager::WORLD_DEPTH);
		glDrawArraysInstanced(GL_PATCHES, 0, 4 * 3, WorldManager::WORLD_WIDTH * WorldManager::WORLD_DEPTH);
		glDisable(GL_BLEND);
	}
	else if (shaderID == SKY) // sky box rendering
	{
		//control the position of skybox
		glVertexAttrib1fv(4, &dist[shaderID]);

		if (EventHandler::events[ControlsEvents::UP_PRESSED])
		{
			dist[shaderID] += 0.05;
		}
		else if (EventHandler::events[ControlsEvents::DOWN_PRESSED])
		{
			dist[shaderID] -= 0.05;
		}

		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		glEnable(GL_DEPTH_TEST);
	}
	else // cube blocks
	{
		// check how many of that cube type exists in the worldmanager then draw each cube.
		int NumInstances = WorldManager::GetInstance()->getNumBlocksByType(blockType);
		for (int i = 0; i < NumInstances; i++)
		{
			if (blockType != NUM_BLOCK_TYPES)
			{
				Vec3 pos = WorldManager::GetInstance()->getCube(blockType, i)->GetPosition();
				vmath::mat4 mv_matrix = vmath::translate(pos.x-5, pos.y-6, pos.z-35);

				mv_matrix *= vmath::scale(1.0f, 1.0f, 1.0f);
				glUniformMatrix4fv(ModelViewUniformHandle[shaderID], 1, GL_FALSE, mv_matrix);
				glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
			}
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void ShaderManager::LoadTexture()
{

	// create a handle for the texture so openGL has an area allocated to manage all our binary image data.
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		glGenTextures(1, &Textures[i]);

		// now tell OpenGL this is the texture we are currently using for all subsequent texture calls.

		glBindTexture(GL_TEXTURE_2D, Textures[i]);

		unsigned int width, height;
		unsigned char* imgData = Utils::loadBMP(gTextureStrings[i].c_str(), width, height);

		// load our texture data up here
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imgData);

		// configure mipmapping levels
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
}


void ShaderManager::Update()
{
	if (EventHandler::events[ControlsEvents::R_PRESSED])
	{
		for (int i = 0; i < NUM_SHADERS; i++)
		{
			glDeleteProgram(programObj[i]);
			programObj[i] = glCreateProgram();
			LoadShader((ShaderTypes)i, true);
		}
	}
}

void ShaderManager::Shutdown()
{
	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glDeleteProgram(programObj[i]);
		glDeleteVertexArrays(1, &vao[i]);
	}
}