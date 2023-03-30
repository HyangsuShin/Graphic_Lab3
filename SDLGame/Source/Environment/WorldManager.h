#pragma once

#include "..\Objects\ObjectData.h"
#include "..\Objects\Cube.h"

class WorldManager
{
public:
	static const int WORLD_WIDTH = 30;
	static const int WORLD_DEPTH = 30;
	static const int WORLD_HEIGHT = 10;

	~WorldManager() {};
	static WorldManager* GetInstance();

	void Startup();
	void Update( double currentTime );
	void ShutDown();
	void GenerateWorld();
	Cube* getCube( int type, int index) { return blocks[type][index]; }
	int getNumBlocksByType(BlockTypes type) { return NumBlocks[type]; }
	
private:
	WorldManager() {};
	void InitializeBlock( int x, int y, int , bool enabled );
	void GenerateHeightMap();
	int GetFriendsHeightAverage(int x, int z);
	void SmoothMap();
	static WorldManager* singleton;

	enum {
		UNINITIALIZED,
		INITIALIZED,
		GENERATED,
		LOADED,
	} currState;

	//used to keep track of the enabled cubes.
	Cube* blocks[NUM_BLOCK_TYPES][WORLD_HEIGHT * WORLD_DEPTH * WORLD_WIDTH];
	int HeightMap[WORLD_WIDTH][WORLD_DEPTH];
	int BiomMap[WORLD_WIDTH][WORLD_DEPTH];
	int NumBlocks[NUM_BLOCK_TYPES];
};