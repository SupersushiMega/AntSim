#pragma once

#include<iostream>
#include<vector>
#include"Graphics.h"


using namespace std;

enum TileTypes
{
	EMPTY, SOLID, FOOD
};

struct tile
{
	uint8_t type = EMPTY;
	uint8_t HomeStrength = 0;
	uint8_t FoodStrength = 0;
};

class Colony
{
public:
	Colony(Graphics* Graphics);

	~Colony();

	Graphics* graphics;

	class Ant
	{
	public:
		Ant()
		{

		}

		~Ant()
		{

		}

		uint8_t speed = 1;
		float heading = 0; //heading in radians
		float WalkCurveFactor = 0.01f;
		vec2D Coordinates = { 50, 50 };

		void AntMove();
	private:

	};

	class TileMap
	{
	public:
		tile* Ptr = nullptr;
		uint16_t width = 0;
		uint16_t height = 0;

		TileMap()
		{

		}

		~TileMap()
		{
			delete[] Ptr;

			Ptr = nullptr;
		}
	};

	TileMap tileMap;

	void MakeTileMap(uint16_t Width, uint16_t Height);

	void drawTileMap();
	void drawAnts();

private:

};