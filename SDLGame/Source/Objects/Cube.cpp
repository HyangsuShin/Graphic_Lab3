#include "Cube.h"
#include "..\ShaderManager.h"
#include "..\Utils.h"
#include "..\EventHandler.h"

Cube::Cube()
{
}


Cube::~Cube()
{
}

void Cube::Init()
{
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	velocity.z = 0.0f;

	transform.position.z = -2;
	transform.position.y = -0.75;
	transform.position.x = -1.5;

	dimensions.x = 5.0;	// width
	dimensions.y = 5.0;	// height
	dimensions.z = 5.0;	// length


	transform.scale.x = 1;
	transform.scale.y = 1;
	transform.scale.z = 1;

	static const GLfloat CubeVB[] =
	{
		-0.25f, -0.25f, -0.25f, //back bottom/left
		-0.25f,  0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f, //back top/right
		0.25f,  0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,// right side back / bottom
		0.25f,  0.25f, -0.25f,
		0.25f, -0.25f,  0.25f,

		0.25f, -0.25f,  0.25f,
		0.25f,  0.25f, -0.25f, // right side front / top
		0.25f,  0.25f,  0.25f,

		0.25f, -0.25f,  0.25f, // front - bottom right
		0.25f,  0.25f,  0.25f,
		-0.25f, -0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		0.25f,  0.25f,  0.25f, // front - left top
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f, // left side - front bottom
		-0.25f,  0.25f,  0.25f,
		-0.25f, -0.25f, -0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f, // left side - back top
		-0.25f,  0.25f, -0.25f,

		0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f, // top back right
		0.25f,  0.25f, 0.25f,

		-0.25f,  0.25f, 0.25f,
		0.25f,  0.25f, 0.25f, // top  front left
		-0.25f,  0.25f, -0.25f,

		-0.25f,  -0.25f, -0.25f, // bottom back right
		0.25f,  -0.25f, -0.25f,
		0.25f,  -0.25f, 0.25f,

		0.25f,  -0.25f, 0.25f, // bottom front left
		-0.25f,  -0.25f, 0.25f,
		-0.25f,  -0.25f, -0.25f,
	};

	static const GLfloat uvVB[] = {
		0.33f, 0.66f, // front - bottom right
		0.33f, 1.00f,
		0.00f, 0.66f,

		0.00f, 0.66f, // front - left top
		0.33f, 1.00f,
		0.00f, 1.00f,

		0.33f, 1.00f,
		0.33f, 0.66f, // right side back / bottom 2
		0.66f, 1.00f,

		0.66f, 1.00f,
		0.33f, 0.66f,
		0.66f, 0.66f, // right side front / top 2

		1.00f, 1.00f, // front - bottom right
		1.00f, 0.66f,
		0.66f, 1.00f,

		0.66f, 1.00f, // front - left top
		1.00f, 0.66f,
		0.66f, 0.66f,

		0.00f, 0.33f, // left side - front bottom
		0.33f, 0.33f,
		0.00f, 0.66f,

		0.00f, 0.66f, // left side - back top
		0.33f, 0.33f,
		0.33f, 0.66f,

		0.66f, 0.66f,
		0.33f, 0.66f, //top
		0.66f, 0.33f,

		0.33f, 0.33f,
		0.66f, 0.33f,
		0.33f, 0.66f,


		0.66f, 0.66f, //bottom
		0.66f, 0.33f,
		1.00f, 0.33f,

		1.00f, 0.33f, //bottom
		1.00f, 0.66f,
		0.66f, 0.66f,
	};

	static const GLfloat normalsVB[] =
	{
		0.0f, 0.0f, -0.25f, //back bottom/left
		0.0f, 0.0f, -0.25f, //back bottom/left
		0.0f, 0.0f, -0.25f, //back bottom/left

		0.0f, 0.0f, -0.25f, //back top/right
		0.0f, 0.0f, -0.25f, //back top/right
		0.0f, 0.0f, -0.25f, //back top/right

		0.25f, 0.0f, 0.0f, // right side back / bottom
		0.25f, 0.0f, 0.0f, // right side back / bottom
		0.25f, 0.0f, 0.0f, // right side back / bottom

		0.25f, 0.0f, 0.0f, // right side front / top
		0.25f, 0.0f, 0.0f, // right side front / top
		0.25f, 0.0f, 0.0f, // right side front / top

		0.0f, 0.0f, 0.25f, // front - bottom right
		0.0f, 0.0f, 0.25f, // front - bottom right
		0.0f, 0.0f, 0.25f, // front - bottom right

		0.0f, 0.0f,  0.25f, // front - left top
		0.0f, 0.0f,  0.25f, // front - left top
		0.0f, 0.0f,  0.25f, // front - left top

		-0.25f, 0.0f, 0.0f, // left side - front bottom
		-0.25f, 0.0f, 0.0f, // left side - front bottom
		-0.25f, 0.0f, 0.0f, // left side - front bottom

		-0.25f, 0.0f, 0.0f, // left side - back top
		-0.25f, 0.0f, 0.0f, // left side - back top
		-0.25f, 0.0f, 0.0f, // left side - back top

		0.0f,  0.25f, 0.0f, // top back right 5
		0.0f,  0.25f, 0.0f, // top back right 5
		0.0f,  0.25f, 0.0f, // top back right 5

		0.0f,  0.25f, 0.0f, // top  front left 5
		0.0f,  0.25f, 0.0f, // top  front left 5
		0.0f,  0.25f, 0.0f, // top  front left 5

		0.0f, -0.25f, 0.0f, // bottom back right 6
		0.0f, -0.25f, 0.0f, // bottom back right 6
		0.0f, -0.25f, 0.0f, // bottom back right 6

		0.0f, -0.25f, 0.0f, // bottom front left 6
		0.0f, -0.25f, 0.0f, // bottom front left 6
		0.0f, -0.25f, 0.0f, // bottom front left 6
	};

	ShaderManager::GetInstance()->LoadVertices(ShaderType, &CubeVB[0], sizeof(CubeVB));
	ShaderManager::GetInstance()->LoadUVs(ShaderType, &uvVB[0], sizeof(uvVB));
	ShaderManager::GetInstance()->LoadNormals(ShaderType, &normalsVB[0], sizeof(normalsVB));
}

void Cube::Draw()
{
}

void Cube::Update()
{
}