#include "AntSim.h"

Colony::Colony(Graphics* Graphics)
{
	graphics = Graphics;
};

Colony::~Colony()
{

};

void Colony::MakeTileMap(uint16_t Width, uint16_t Height)
{
	tileMap.Ptr = new tile[Width * Height];
	tileMap.width = Width;
	tileMap.height = Height;
}

void Colony::drawTileMap()
{
	vec2D TileSize;

	TileSize.x = graphics->resolution.right / tileMap.width;
	TileSize.y = graphics->resolution.bottom / tileMap.height;
	for (uint16_t x = 0; x <= tileMap.width; x++)
	{
		for (uint16_t y = 0; y <= tileMap.height; y++)
		{
			graphics->DrawRect(x * (TileSize.x + 1), y * (TileSize.y + 1), TileSize.x, TileSize.y);
		}
	}
};

void Colony::Ant::AntMove()
{
	if ((Coordinates.x + (speed * sin(heading))) > graphics->resolution.right || (Coordinates.x + (speed * sin(heading))) < 0)
	{
		heading += M_PI;
	}
	else
	{
		Coordinates.x += speed * sin(heading);
	}

	if ((Coordinates.y + (speed * cos(heading))) > graphics->resolution.bottom || (Coordinates.y + (speed * cos(heading))) < 0)
	{
		heading += M_PI;
	}
	else
	{
		Coordinates.y += speed * cos(heading);
	}

	heading += ((int8_t)(rand() % 3) - 1) * WalkCurveFactor;
}

void Colony::addAnt()
{
	Ant tempAnt = Ant(graphics);
	Ants.push_back(tempAnt);
}

void Colony::drawAnts()
{
	for (uint32_t antID = 0; antID < Ants.size(); antID++)
	{
		graphics->DrawCircle(Ants[antID].Coordinates.x, Ants[antID].Coordinates.y, 1);
		Ants[antID].AntMove();
	}
}
