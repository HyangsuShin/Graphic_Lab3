#pragma once
#ifndef OBJDATA
#define OBJDATA

#include <iostream>

using namespace std;

enum GameObjectsTypeList
{
	BLOCKS,

	// other object
	PLANE,	
	//JD 2: Add the skybox Object to the list

	SKYBOX,

	NUM_GAMEOBJECT_TYPES,
};


enum ShaderTypes
{
	CUBE,

	//JD 2: Add the skybox shader to the list
	SKY,

	WATER,

	NUM_SHADERS,
};

enum TextureTypes
{
	GRASS_BLOCKS,
	DIRT_BLOCKS,
	CHARCOAL_BLOCKS,
	ROCK_BLOCKS,
	GOLD_BLOCKS,


	// other object
	//JD 2: Add the skybox texture to the list
	SKYBOX_BLOCKS,

	WATER_PLANE,
	
	NUM_TEXTURES
};

enum BlockTypes
{
	BLOCK_TYPE_NONE = -1, 

	GRASS_BLOCK_TYPE,
	DIRT_BLOCK_TYPE,
	CHARCOAL_BLOCK_TYPE,
	ROCKS_BLOCK_TYPE,
	GOLD_BLOCK_TYPE,

	NUM_BLOCK_TYPES
};

const int HEIGHT_MINS[NUM_BLOCK_TYPES]
{
	5, 1, 1, 1, 1
};

const int HEIGHT_MAX[NUM_BLOCK_TYPES]
{
	10, 9, 7,4,4
};

extern std::string gShaderNames[NUM_SHADERS];

// add new object textures here, must align 1 to 1 with the Object types enum
extern std::string gTextureStrings[NUM_TEXTURES];


#endif //OBJDATA