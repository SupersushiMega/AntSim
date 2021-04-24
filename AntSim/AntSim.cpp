#include "AntSim.h"

Colony::Colony(Graphics* Graphic)
{
	graphics = Graphic;
};

Colony::~Colony()
{

};

void Colony::MakeTileMap(uint16_t Width, uint16_t Height)
{
	tileMap.Ptr = new tile[Width * Height];
	tileMap.width = Width;
	tileMap.height = Height;
	tileMap.graphics = graphics;
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
			tile temp = tileMap.ReadMap(x, y);
			if ((temp.FoodStrength || temp.HomeStrength))
			{
				graphics->setDrawColor(0.0f, temp.FoodStrength, temp.HomeStrength);
				graphics->DrawRect(x * (TileSize.x + 1), y * (TileSize.y + 1), TileSize.x, TileSize.y);
			}
			if (temp.HomeStrength > 0)
			{
				temp.HomeStrength -= 0.02f;
				tileMap.WriteToMap(x, y, temp);
			}
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

void Colony::Ant::placePheromone()
{
	switch (state)
	{
		case SCOUTING:
		{
			tile temp = tilemap->ReadMap_WC(Coordinates.x, Coordinates.y);
			temp.HomeStrength = 1;
			tilemap->WriteToMap_WC(Coordinates.x, Coordinates.y, temp);
			break;
		}
		default:
		{
			break;
		}
	}
}

void Colony::addAnt()
{
	Ant tempAnt = Ant(graphics, &tileMap);
	Ants.push_back(tempAnt);
}

void Colony::drawAnts()
{
	for (uint32_t antID = 0; antID < Ants.size(); antID++)
	{
		graphics->setDrawColor(1, 1, 1);
		graphics->DrawCircle(Ants[antID].Coordinates.x, Ants[antID].Coordinates.y, 1);
		Ants[antID].placePheromone();
		Ants[antID].AntMove();
	}
}
