#include "WorldManager.h"
//#include "../Shaders/ShaderManager.h"

// for threading the world generation
#include <thread>

//#include <shader.h> // used as an automatic shader loader
//#include <vmath.h> // used for matrix math
#include <stdlib.h>  // rand
#include <time.h>   // time

WorldManager *WorldManager::singleton = 0;

WorldManager* WorldManager::GetInstance()
{
	if ( !singleton )
	{
		singleton = new WorldManager();
	}
	return singleton;
}

void WorldManager::Startup()
{

	for (int i = 0; i < NUM_BLOCK_TYPES; i++)
	{
		NumBlocks[i] = 0;
	}
	GenerateWorld();
}

void WorldManager::GenerateWorld()
{
	srand( time( NULL ) );
	GenerateHeightMap();

	for ( int y = 0; y < WORLD_HEIGHT; y++ )
	{
		for ( int z = 0; z < WORLD_DEPTH; z++ )
		{
			for ( int x = 0; x < WORLD_WIDTH; x++ )
			{
				// bottom needs to be all set
				if ( y <= 1 )
				{
					InitializeBlock( x, y, z, true );
				}
				// else if we are not at the very top, or the cube below exists then evaluate
				else if (  y != ( WORLD_HEIGHT - 1 ) && y <= HeightMap[x][z])
				{
					InitializeBlock( x, y, z, true );
				}
			}
		}
	}
}

void WorldManager::GenerateHeightMap()
{
	int FriendsHeight = 1;

	for (int x = 0; x < WORLD_WIDTH; x++)
	{
		for (int z = 0; z < WORLD_DEPTH; z++)
		{
			if (x + z == 0)
			{
				HeightMap[x][z] = FriendsHeight;
			}
			else
			{
				FriendsHeight = GetFriendsHeightAverage( x, z );
				if (FriendsHeight < 1)
				{
					FriendsHeight = 1; 
				}
				int modifier = 0;

				// Chance to change level from avg
				if (rand() % 5 == 0)
				{
					modifier = ( (rand() % 4) - 1);
				}

				HeightMap[x][z] = FriendsHeight + modifier;
			}
		}
	}

	SmoothMap( );
}

//Removes individual blocks sticking out from the random processing
void WorldManager::SmoothMap()
{
	for (int x = 0; x < WORLD_WIDTH; x++)
	{
		for (int z = 0; z < WORLD_DEPTH; z++)
		{
			// Make sure we are not the outer edge worth.
			if (z > 0 && z < WORLD_DEPTH -1 && x > 0 && x < WORLD_WIDTH-1)
			{
				int currHeight = HeightMap[x][z];
				int friendHeights[4] = { 0,0,0,0 };

				friendHeights[0] = HeightMap[x-1][z];
				friendHeights[1] = HeightMap[x+1][z];
				friendHeights[2] = HeightMap[x][z-1];
				friendHeights[3] = HeightMap[x][z+1];

				if (currHeight > friendHeights[0] &&
					currHeight > friendHeights[1] &&
					currHeight > friendHeights[2] &&
					currHeight > friendHeights[3])
				{
					//if it's a single block higher then everything around it, put it back down...
					HeightMap[x][z] = HeightMap[x][z] - 1;
				}

				if (currHeight < friendHeights[0] &&
					currHeight < friendHeights[1] &&
					currHeight < friendHeights[2] &&
					currHeight < friendHeights[3])
				{
					//if it's a single block lower then everything around it, put it back up...
					HeightMap[x][z] = HeightMap[x][z] + 1;
				}
			}
		}
	}
}

//gets the average height of blocks around it
int WorldManager::GetFriendsHeightAverage(int x, int z)
{
	int avg = 0;
	int numFriends = 0;

	if (x != 0)
	{
		avg += HeightMap[x - 1][z];
		numFriends++;
	}

	if (z != 0)
	{
		avg += HeightMap[x][z-1];
		numFriends++;
		if (x != 0)
		{
			avg += HeightMap[x - 1][z - 1];
			numFriends++;
		}
	}

	return (int) (((float)avg / (float)numFriends) + 0.51f);
}

void WorldManager::InitializeBlock( int x, int y, int z, bool enabled )
{
	bool assigned = false;
	BlockTypes type = NUM_BLOCK_TYPES;
	
	if (!assigned && y > HEIGHT_MINS[GOLD_BLOCK_TYPE] && y < HEIGHT_MAX[GOLD_BLOCK_TYPE])
	{
		int randVal = (rand() % 100);
		if (randVal < 1) // 1% chance to have a gold block
		{
			type = GOLD_BLOCK_TYPE;
			blocks[type][NumBlocks[type]] = new Cube();
			blocks[type][NumBlocks[type]]->SetShaderType(ShaderTypes::CUBE);
			blocks[type][NumBlocks[type]]->Init();
			blocks[type][NumBlocks[type]]->SetTextureType(GOLD_BLOCKS);
			assigned = true;
		}
	}

	if (!assigned && y > HEIGHT_MINS[ROCKS_BLOCK_TYPE] && y < HEIGHT_MAX[ROCKS_BLOCK_TYPE])
	{
		int randVal = (rand() % 100);
		if (randVal < 75)
		{
			type = ROCKS_BLOCK_TYPE;
			blocks[type][NumBlocks[type]] = new Cube();
			blocks[type][NumBlocks[type]]->SetShaderType(ShaderTypes::CUBE);
			blocks[type][NumBlocks[type]]->Init();
			blocks[type][NumBlocks[type]]->SetTextureType(ROCK_BLOCKS);
			assigned = true;
		}
	}

	if (!assigned && y > HEIGHT_MINS[GRASS_BLOCK_TYPE] && y < HEIGHT_MAX[GRASS_BLOCK_TYPE] && HeightMap[x][z] == y)
	{
		type = GRASS_BLOCK_TYPE;
		blocks[type][NumBlocks[type]] = new Cube();
		blocks[type][NumBlocks[type]]->SetShaderType(ShaderTypes::CUBE);
		blocks[type][NumBlocks[type]]->Init();
		blocks[type][NumBlocks[type]]->SetTextureType(GRASS_BLOCKS);
		assigned = true;
	}

	if (!assigned && y > HEIGHT_MINS[CHARCOAL_BLOCK_TYPE] && y < HEIGHT_MAX[CHARCOAL_BLOCK_TYPE] && HeightMap[x][z] > y)
	{
		int randVal = (rand() % 100);
		if (randVal < 25)
		{
			type = CHARCOAL_BLOCK_TYPE;
			blocks[type][NumBlocks[type]] = new Cube();
			blocks[type][NumBlocks[type]]->SetShaderType(ShaderTypes::CUBE);
			blocks[type][NumBlocks[type]]->Init();
			blocks[type][NumBlocks[type]]->SetTextureType(CHARCOAL_BLOCKS);
			assigned = true;
		}
	}

	if (!assigned && y > HEIGHT_MINS[ROCKS_BLOCK_TYPE] && y < HEIGHT_MAX[ROCKS_BLOCK_TYPE])
	{
		type = ROCKS_BLOCK_TYPE;
		blocks[type][NumBlocks[type]] = new Cube();
		blocks[type][NumBlocks[type]]->SetShaderType(ShaderTypes::CUBE);
		blocks[type][NumBlocks[type]]->Init();
		blocks[type][NumBlocks[type]]->SetTextureType(ROCK_BLOCKS);
		assigned = true;
	}

	if (!assigned && y > HEIGHT_MINS[DIRT_BLOCK_TYPE] && y < HEIGHT_MAX[DIRT_BLOCK_TYPE] )
	{
		type = DIRT_BLOCK_TYPE;
		blocks[type][NumBlocks[type]] = new Cube();
		blocks[type][NumBlocks[type]]->SetShaderType(ShaderTypes::CUBE);
		blocks[type][NumBlocks[type]]->Init();
		blocks[type][NumBlocks[type]]->SetTextureType(DIRT_BLOCKS);
		assigned = true;
	}

	if (type != NUM_BLOCK_TYPES && blocks[type][NumBlocks[type]] != nullptr)
	{
		blocks[type][NumBlocks[type]]->SetPosition(x, y, z);
		NumBlocks[type]++;
	}
}


void WorldManager::Update( double currentTime )
{

}

void WorldManager::ShutDown()
{
	for (int type = 0; type < NUM_BLOCK_TYPES; type++)
	{
		for (int x = 0; x < WORLD_WIDTH * WORLD_DEPTH * WORLD_HEIGHT; x++)
		{
			if (blocks[type][x] != nullptr)
			{
				delete blocks[type][x];
			}
		}
	}
}