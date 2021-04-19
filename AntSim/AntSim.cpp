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
			//graphics->DrawRect(0, 0, 100, 200);
		}
	}
};

void Colony::drawAnts()
{

}


void Colony::Ant::AntMove()
{
	Coordinates.x = speed * sin(heading);
	Coordinates.x = speed * cos(heading);
	heading += WalkCurveFactor;
}