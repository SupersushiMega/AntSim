#pragma once

#define _USE_MATH_DEFINES

#include<iostream>
#include<vector>
#include<cmath>
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
		Ant(Graphics* Graphics)
		{
			graphics = Graphics;
			heading = (float)(rand() % 628) / 100;
		}

		~Ant()
		{

		}

		Graphics* graphics;

		uint8_t speed = 1;
		float heading = 0; //heading in radians
		float WalkCurveFactor = 0.1f;
		vec2D Coordinates = { 200, 500 };

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

	vector<Ant> Ants;

	void MakeTileMap(uint16_t Width, uint16_t Height);

	void drawTileMap();

	void addAnt();
	void drawAnts();

private:

};