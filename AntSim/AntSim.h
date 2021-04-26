#pragma once

#define _USE_MATH_DEFINES

#include<iostream>
#include<vector>
#include<cmath>
#include<thread>
#include"Graphics.h"


using namespace std;

enum TileTypes
{
	EMPTY, SOLID, FOOD
};

enum AntStates
{
	SCOUTING, TRANSPORTING_FOOD, GOING_HOME
};

struct tile
{
	uint8_t type = EMPTY;
	float HomeStrength = 0;
	float FoodStrength = 0;
};

class Colony
{
public:
	Colony(Graphics* Graphic, uint16_t x, uint16_t y);

	~Colony();

	Graphics* graphics;
	uint16_t colonyY;
	uint16_t colonyX;

	uint8_t colonySize = 20;
	
	uint32_t food = 0;

	class TileMap
	{
	public:
		tile* Ptr = nullptr;
		uint16_t width = 0;
		uint16_t height = 0;
		Graphics* graphics = nullptr;

		float HomeDegration = 0.001f;	//the speed at which pheromons degrade
		float FoodDegration = 0.01f;	//the speed at which pheromons degrade

		TileMap()
		{

		};

		~TileMap()
		{
			delete[] Ptr;

			Ptr = nullptr;
		}


		void WriteToMap(uint16_t x, uint16_t y, tile Tile)	//write to tilemap using map coordinates
		{
			if ((x > 0) && (y < height) && (x > 0) && (y < width))
			{
				Ptr[x + (width * y)] = Tile;
			}
		};
		void WriteToMap_WC(float x, float y, tile Tile)	//write to tilemap using world coordinates
		{
			vec2D TileSize;

			TileSize.x = graphics->resolution.right / width + 1;
			TileSize.y = graphics->resolution.bottom / height + 1;

			uint16_t mapX = (uint16_t)(x / TileSize.x);
			uint16_t mapY = (uint16_t)(y / TileSize.y);

			if ((mapY > 0) && (mapY < height) && (mapX > 0) && (mapX < width))
			{
				Ptr[mapX + (width * mapY)] = Tile;
			}
		};

		tile ReadMap(uint16_t x, uint16_t y)	//write from tilemap using map coordinates
		{
			if (y < height)
			{
				return Ptr[x + (width * y)];
			}
			else
			{
				tile temp;
				return temp;
			}
		};

		tile ReadMap_WC(float x, float y)	//read from tilemap using world coordinates
		{
			volatile vec2D TileSize;

			TileSize.x = graphics->resolution.right / width + 1;
			TileSize.y = graphics->resolution.bottom / height + 1;

			volatile uint16_t mapX = (uint16_t)(x / TileSize.x);
			volatile uint16_t mapY = (uint16_t)(y / TileSize.y);
			if ((mapY > 0) && (mapY < height) && (mapX > 0) && (mapX < width))
			{
				return Ptr[mapX + (width * mapY)];
			}
			else
			{
				tile temp;
				return temp;
			}
		};
	};

	TileMap tileMap;

	class Ant
	{
	public:
		Ant(Graphics* Graphic, TileMap* Tilemap, Colony* ParentCol, uint16_t x, uint16_t y)
		{
			graphics = Graphic;
			tilemap = Tilemap;
			parentCol = ParentCol;
			Coordinates.x = x;
			Coordinates.y = y;
			heading = (float)(rand() % 628) / 100;	//create random start heading
		}

		~Ant()
		{

		}
		
		Graphics* graphics;
		TileMap* tilemap;
		Colony* parentCol;

		uint8_t viewDistance = 6;
		float FOV = M_PI;	//FOV of ant in radians
		float WalkCurveFactor = 0.05f;
		float pheromonAttraction = 0.01f;	//how strong the ant turns when it detects pheromons

		uint8_t state = SCOUTING;

		uint8_t speed = 1;
		float heading = 0; //heading in radians
		vec2D Coordinates = { 0, 0 };

		void AntMove();
		void placePheromone();
		void checkArea();

	private:

	};

	vector<Ant> Ants;

	void simulateStep();

	void MakeTileMap(uint16_t Width, uint16_t Height);

	void drawTileMap();

	void drawAnts();

	void addAnt();

private:

};