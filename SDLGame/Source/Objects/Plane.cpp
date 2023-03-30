#include "Plane.h"
#include "..\ShaderManager.h"
#include "..\Utils.h"
#include "..\EventHandler.h"
#include "..\vmath.h"

Plane::Plane()
{
}

Plane::~Plane()
{
}

void Plane::Init()
{

	velocity.x = 0.0f;
	velocity.y = 0.0f;
	velocity.z = 0.0f;

	transform.position.z = -2;
	transform.position.y = -0.75;
	transform.position.x = -1.5;

	transform.scale.x = 50;
	transform.scale.y = 50;
	transform.scale.z = 50;

	dimensions.x = 50.0;	// width
	dimensions.y = 50.0;	// height
	dimensions.z = 50.0;	// length

	static const GLfloat PlaneVB[] =
	{
		0.25f,  0.00f, -0.25f,
		-0.25f,  0.00f, -0.25f, // top back right
		0.25f,  0.00f, 0.25f,

		-0.25f,  0.00f, 0.25f,
		0.25f,  0.00f, 0.25f, // top  front left
		-0.25f,  0.00f, -0.25f,

		-0.25f,  0.00f, -0.25f, // bottom back right
		0.25f,  0.00f, -0.25f,
		0.25f,  0.00f, 0.25f,

		0.25f,  0.00f, 0.25f, // bottom front left
		-0.25f,  0.00f, 0.25f,
		-0.25f,  0.00f, -0.25f,
	};

	static const GLfloat uvVB[] = {

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

	ShaderManager::GetInstance()->LoadVertices(ShaderType, &PlaneVB[0], sizeof(PlaneVB));
	ShaderManager::GetInstance()->LoadUVs(ShaderType, &uvVB[0], sizeof(uvVB));
	ShaderManager::GetInstance()->LoadNormals(ShaderType, &normalsVB[0], sizeof(normalsVB));
}

void Plane::Draw()
{
	ShaderManager::GetInstance()->Render(ShaderType, TextureType, BLOCK_TYPE_NONE);
}

void Plane::Update()
{
}