#include "AntSim.h"

Colony::Colony(Graphics* Graphic, uint16_t x, uint16_t y)
{
	graphics = Graphic;
	colonyX = x;
	colonyY = y;
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
			for (uint16_t x2 = x * (TileSize.x + 1); x2 < (x + 1) * (TileSize.x + 1); x2++)
			{
				for (uint16_t y2 = y * (TileSize.y + 1); y2 < (y + 1) * (TileSize.y + 1); y2++)
				{
					Color PixCol = { temp.FoodStrength, 0.0f, temp.HomeStrength};
					if (temp.type == FOOD)
					{
						PixCol.g = 0.5f;
					}
					graphics->imageBuff.PutPix(x2, y2, PixCol);
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

	heading += ((float)((rand() % 299)/100) - 1) * WalkCurveFactor;
}

void Colony::Ant::placePheromone()
{
	switch (state)
	{
		case SCOUTING:
		{
			tile temp = tilemap->ReadMap_WC(Coordinates.x, Coordinates.y);
			if (temp.HomeStrength < (1-0.08f))
			{
				temp.HomeStrength += 0.08f;
			}
			tilemap->WriteToMap_WC(Coordinates.x, Coordinates.y, temp);
			break;
		}
		case TRANSPORTING_FOOD:
		{
			tile temp = tilemap->ReadMap_WC(Coordinates.x, Coordinates.y);
			temp.FoodStrength = 1;
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
	if (graphics != nullptr)
	{
		uint8_t r = 0;
		float angle = 0;

		tile temp1;
		temp1.HomeStrength = 1;

		tile temp2;
		temp2.HomeStrength = 1;

		float headingDelta = 0.0f;
		float headingDeltaMax = 0.1f;

		bool hasTurned = false;
		bool foundFood = false;

		volatile tile CurTile = tilemap->ReadMap_WC(Coordinates.x, Coordinates.y);	//Tile the ant is curently standing on;

		if ((CurTile.type == FOOD) && (state == SCOUTING))
		{
			state = TRANSPORTING_FOOD;
			tile temp;
			temp.type = EMPTY;
			tilemap->WriteToMap_WC(Coordinates.x + (sin(angle + heading) * r), Coordinates.y + (cos(angle + heading) * r), temp);
			heading += M_PI;
		}

		for (r = 4; r < viewDistance + 4; r++)
		{
			for (angle = -((float)FOV / 2); angle < ((float)FOV / 2); angle += 0.1f)
			{

				volatile tile ViewTile = tilemap->ReadMap_WC(Coordinates.x + (sin(angle + heading) * r), Coordinates.y + (cos(angle + heading) * r));	//Tile the ant is curently standing on
				volatile float coloDistX = abs(Coordinates.x - parentCol->colonyX);
				volatile float coloDistY = abs(Coordinates.y - parentCol->colonyY);
				float coloDist = sqrt((coloDistX * coloDistX) + (coloDistY * coloDistY));

				if (angle < 0)
				{
					if (state == SCOUTING)
					{
						if (ViewTile.type == FOOD)
						{
							headingDelta = -headingDeltaMax;
						}
						else
						{
							headingDelta -= pheromonAttraction * ViewTile.FoodStrength;
						}
					}
					else if (state == TRANSPORTING_FOOD)
					{
						if (coloDist < parentCol->colonySize)
						{
							state = SCOUTING;
							parentCol->food++;
						}
						if (ViewTile.HomeStrength != 0)
						{
							headingDelta -= pheromonAttraction * ViewTile.HomeStrength;
						}
					}
					//tilemap->WriteToMap_WC(Coordinates.x + (sin(angle + heading) * r), Coordinates.y + (cos(angle + heading) * r), temp2);	//Debuging
				}
				else
				{
					if (state == SCOUTING)
					{
						if (ViewTile.type == FOOD)
						{
							headingDelta = headingDeltaMax;
						}
						else
						{
							headingDelta += pheromonAttraction * ViewTile.FoodStrength;
						}
					}
					else if (state == TRANSPORTING_FOOD)
					{
						if (coloDist < parentCol->colonySize)
						{
							state = SCOUTING;
							parentCol->food++;
						}
						if (ViewTile.HomeStrength != 0)
						{
							headingDelta += pheromonAttraction * ViewTile.HomeStrength;
						}
					}
					//tilemap->WriteToMap_WC(Coordinates.x + (sin(angle + heading) * r), Coordinates.y + (cos(angle + heading) * r), temp2);	//Debuging
				}
				//if (ViewTile.type == SOLID && !hasTurned)
				//{
				//	heading += M_PI;
				//	hasTurned = true;
				//}
			}
		}
		if (headingDelta > headingDeltaMax)
		{
			headingDelta = headingDeltaMax;
		}
		if (headingDelta < -headingDeltaMax)
		{
			headingDelta = -headingDeltaMax;
		}
		heading += headingDelta;
	}
}

void Colony::drawAnts()
{
	Color col = { 1.0f, 1.0f, 1.0f };
	for (uint32_t antID = 0; antID < Ants.size(); antID++)
	{
		graphics->imageBuff.PutPix(Ants[antID].Coordinates.x, Ants[antID].Coordinates.y, col);
	}
}

void Colony::addAnt()
{
	Ant tempAnt = Ant(graphics, &tileMap, this, colonyX, colonyY);
	Ants.push_back(tempAnt);
}

void Colony::simulateStep()
{
	for (food; food > 0; food--)
	{
		addAnt();
	}
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
			if (temp.HomeStrength > tileMap.HomeDegration)
			{
				temp.HomeStrength -= tileMap.HomeDegration;
			}
			if (temp.FoodStrength > tileMap.FoodDegration)
			{
				temp.FoodStrength -= tileMap.FoodDegration;
			}
			tileMap.WriteToMap(x, y, temp);
		}
	}
}


