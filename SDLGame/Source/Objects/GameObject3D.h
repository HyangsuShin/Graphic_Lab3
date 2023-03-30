#pragma once
#include <string>
#include "..\Math.h"
#include "SDL.h"

// new OpenGL/GLEW headers:
#define GL3_PROTOTYPES 1
#include "glew.h"

#include "ObjectData.h"

using namespace std;

class GameObject3D
{
public:
	GameObject3D();
	~GameObject3D();
	
	// used for any variables and data that needs to be setup on the object before we try to use it.
	virtual void Init() {};

	// used to render the object to the screen
	virtual void Draw() {};

	// used to update processing on the object (in case it's moving or has some processing to compute)
	virtual void Update();

	// when the object is destroyed to ensure any memory allocated during it's lifetime is cleaned up.
	virtual void Destroy() {};

	void SetObjectType(GameObjectsTypeList objType) { ObjectType = objType; }

	void SetPosition(int x, int y, int z);
	Vec3 GetPosition() { return transform.position;  }

	// when the object is destroyed to ensure any memory allocated during it's lifetime is cleaned up.
	bool CheckCollision(GameObject3D otherObj);

	void SetShaderType(ShaderTypes shaderID) { ShaderType = shaderID; }


protected:

	// pos, rot, scale
	Transform3D transform;	

	// movement speed in direction
	Vec3 velocity;

	// width, height, depth/length
	Vec3 dimensions;

	GLuint bufferHandle;

	int iframes = 0;

	GameObjectsTypeList ObjectType;

	ShaderTypes ShaderType;

	
private:

};

