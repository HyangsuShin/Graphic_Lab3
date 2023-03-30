#include "GamePlayManager.h"
#include <iostream>
#include "CameraManager.h"
#include "Objects\ObjectData.h"
#include "Environment\WorldManager.h"
#include "ShaderManager.h"


GamePlayManager* GamePlayManager::instance = nullptr;


GamePlayManager::GamePlayManager()
{
}


GamePlayManager::~GamePlayManager()
{
}

void GamePlayManager::DisplaySplashScreen(SDL_Window* mainWindow, SDL_Renderer* mainRenderer)
{
	// this object is defined inside the GameObject2D file.
	GameObject2D SplashScreen;
	SplashScreen.Init("Data\\Art\\SplashScreen.bmp", mainRenderer);

	// Draw a black background
	SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(mainRenderer);

	SwapSplash(mainWindow, mainRenderer, 1000, SplashScreen);
	
	// start generating the world while the splash screen is displayed as this can take some time.
	WorldManager::GetInstance()->Startup();

}

void GamePlayManager::SwapSplash(SDL_Window* mainWindow, SDL_Renderer* mainRenderer, Uint32 delay, GameObject2D screen)
{
	SDL_RenderClear(mainRenderer);
	screen.Draw(mainWindow, mainRenderer);

	// Update the screen!
	SDL_RenderPresent(mainRenderer);
	SDL_Delay(delay);
}


void GamePlayManager::InitGameplay()
{
	glEnable(GL_DEPTH_TEST);

	LightManager::GetInstance()->SetupSimpleLighting();
	CameraManager::GetInstance()->SetupCamera();

	water.SetObjectType(GameObjectsTypeList::PLANE);
	water.SetTextureType(TextureTypes::WATER_PLANE);
	water.SetShaderType(ShaderTypes::WATER);
	water.Init();


	//JD 2: set the skybox objtype, texturetype and shadertype using the water as an example above but configured for a skybox, then init.
	skybox.SetObjectType(GameObjectsTypeList::SKYBOX);
	skybox.SetTextureType(TextureTypes::SKYBOX_BLOCKS);
	skybox.SetShaderType(ShaderTypes::SKY);
	skybox.Init();
}


void GamePlayManager::Update()
{
	CameraManager::GetInstance()->Update();

	//JD 2: update and draw the skybox first
	skybox.Update();
	skybox.Draw();

	ShaderManager::GetInstance()->Render(CUBE, GRASS_BLOCKS, GRASS_BLOCK_TYPE);
	ShaderManager::GetInstance()->Render(CUBE, DIRT_BLOCKS, DIRT_BLOCK_TYPE);
	ShaderManager::GetInstance()->Render(CUBE, ROCK_BLOCKS, ROCKS_BLOCK_TYPE);
	ShaderManager::GetInstance()->Render(CUBE, CHARCOAL_BLOCKS, CHARCOAL_BLOCK_TYPE);
	ShaderManager::GetInstance()->Render(CUBE, GOLD_BLOCKS, GOLD_BLOCK_TYPE);


	water.Update();
	water.Draw();
}