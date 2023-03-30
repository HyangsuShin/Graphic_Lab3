#include "ObjectData.h"

// add new object textures here, must align 1 to 1 with the Object types enum
std::string gTextureStrings[NUM_TEXTURES] =
{
	//blocks
	"Data\\Art\\grass.bmp",
	"Data\\Art\\dirt.bmp",
	"Data\\Art\\charcoal.bmp",
	"Data\\Art\\rock.bmp",
	"Data\\Art\\gold.bmp",

	//JD 2: Add the skybox image to the list
	"Data\\Art\\skybox.bmp",
	// other object
	"Data\\Art\\water.bmp",
};

std::string gShaderNames[NUM_SHADERS] =
{
	//blocks
	"Cubes",

	//JD 2: Add the skybox to the list of shader names
	"Skybox",

	"Water",
};