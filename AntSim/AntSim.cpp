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
			if ((temp.FoodStrength > 0) || (temp.HomeStrength > 0) || temp.type != EMPTY)
			{
				for (uint16_t x2 = x * (TileSize.x + 1); x2 < (x + 1) * (TileSize.x + 1); x2++)
				{
					for (uint16_t y2 = y * (TileSize.y + 1); y2 < (y + 1) * (TileSize.y + 1); y2++)
					{
						Color PixCol = { temp.FoodStrength, 0.0f, temp.HomeStrength};
						if (temp.type = FOOD);
						{
							PixCol.g = 0.5f;
						}
						graphics->imageBuff.PutPix(x2, y2, PixCol);
					}
				}
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

	heading += ((float)((rand() % 200)/100) - 1) * WalkCurveFactor;
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

void Colony::Ant::checkArea()
{
	uint8_t r = 0;
	float angle = 0;

	tile temp1;
	temp1.FoodStrength = 1;

	tile temp2;
	temp2.HomeStrength = 1;

	tile temp;

	for (r = 4; r < viewDistance + 4; r++)
	{
		for (angle = -((float)FOV / 2); angle < ((float)FOV / 2); angle += 0.1f)
		{
			temp = tilemap->ReadMap_WC(Coordinates.x + (sin(angle + heading) * r), Coordinates.y + (cos(angle + heading) * r));	//read data
			if (angle < 0)
			{
				heading -= pheromonAttraction * temp.HomeStrength;
				//tilemap->WriteToMap_WC(Coordinates.x + (sin(angle + heading) * r), Coordinates.y + (cos(angle + heading) * r), temp2);	//Debuging
			}
			else
			{
				heading += pheromonAttraction * temp.HomeStrength;
				//tilemap->WriteToMap_WC(Coordinates.x + (sin(angle + heading) * r), Coordinates.y + (cos(angle + heading) * r), temp2);	//Debuging
			}
		}
	}
}

void Colony::addAnt()
{
	Ant tempAnt = Ant(graphics, &tileMap);
	Ants.push_back(tempAnt);
}

void Colony::simulateStep()
{
	for (uint32_t antID = 0; antID < Ants.size(); antID++)
	{
		Ants[antID].checkArea();
		Ants[antID].placePheromone();
		Ants[antID].AntMove();
	}
	for (uint16_t x = 0; x <= tileMap.width; x++)
	{
		for (uint16_t y = 0; y <= tileMap.height; y++)
		{
			tile temp = tileMap.ReadMap(x, y);
			if (temp.HomeStrength > 0)
			{
				temp.HomeStrength -= tileMap.pheromonDegration;
				tileMap.WriteToMap(x, y, temp);
			}
			if (temp.FoodStrength > 0)
			{
				temp.FoodStrength -= tileMap.pheromonDegration;
				tileMap.WriteToMap(x, y, temp);
			}
		}
	}
}


