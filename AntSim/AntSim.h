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
	Colony(Graphics* Graphic);

	~Colony();

	Graphics* graphics;

	class TileMap
	{
	public:
		tile* Ptr = nullptr;
		uint16_t width = 0;
		uint16_t height = 0;
		Graphics* graphics = nullptr;

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
			Ptr[x + (width * y)] = Tile;
		};
		void WriteToMap_WC(float x, float y, tile Tile)	//write to tilemap using world coordinates
		{
			vec2D TileSize;

			TileSize.x = graphics->resolution.right / width + 1;
			TileSize.y = graphics->resolution.bottom / height + 1;
			Ptr[(uint16_t)(x / TileSize.x) + (width * (uint16_t)(y / TileSize.y))] = Tile;
		};

		tile ReadMap(uint16_t x, uint16_t y)	//write from tilemap using map coordinates
		{
			if (y < height)
			{
				return Ptr[x + (width * y)];
			}
		};

		tile ReadMap_WC(float x, float y)	//read from tilemap using world coordinates
		{
			vec2D TileSize;

			TileSize.x = graphics->resolution.right / width + 1;
			TileSize.y = graphics->resolution.bottom / height + 1;
			return Ptr[(uint16_t)(x / TileSize.x) + (width * (uint16_t)(y / TileSize.y))];
		};
	};

	TileMap tileMap;

	class Ant
	{
	public:
		Ant(Graphics* Graphic, TileMap* Tilemap)
		{
			graphics = Graphic;
			tilemap = Tilemap;
			heading = (float)(rand() % 628) / 100;	//create random start heading
		}

		~Ant()
		{

		}
		Graphics* graphics;
		TileMap* tilemap;

		uint8_t state = SCOUTING;

		uint8_t speed = 1;
		float heading = 0; //heading in radians
		float WalkCurveFactor = 0.1f;
		vec2D Coordinates = { 0, 0 };

		void AntMove();
		void placePheromone();

	private:

	};

	vector<Ant> Ants;

	void simulateStep();

	void MakeTileMap(uint16_t Width, uint16_t Height);

	void drawTileMap();

	void addAnt();
	void drawAnts();

private:

};