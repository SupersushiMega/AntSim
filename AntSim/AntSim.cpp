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
					Color PixCol = { temp.FoodStrength / 2, 0.0f, temp.HomeStrength / 2};
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
	heading += ((float)((rand() % 3)) - 1) * WalkCurveFactor;
	if ((Coordinates.x + (speed * sin(heading))) > graphics->resolution.right || (Coordinates.x + (speed * sin(heading))) < 0)
	{
		heading += M_PI * (((float)(rand() % 300) - 100) / 100);
	}
	else
	{
		Coordinates.x += speed * sin(heading);
	}

	if ((Coordinates.y + (speed * cos(heading))) > graphics->resolution.bottom || (Coordinates.y + (speed * cos(heading))) < 0)
	{
		heading += M_PI * (((float)(rand() % 300) - 100) / 100);
	}
	else
	{
		Coordinates.y += speed * cos(heading);
	}

}

void Colony::Ant::placePheromone()
{
	switch (state)
	{
		case SCOUTING:
		{
			tile temp = tilemap->ReadMap_WC(Coordinates.x, Coordinates.y);
			if (temp.HomeStrength < 2)
			{
				temp.HomeStrength += 0.08f;
			}
			tilemap->WriteToMap_WC(Coordinates.x, Coordinates.y, temp);
			break;
		}
		case TRANSPORTING_FOOD:
		{
			tile temp = tilemap->ReadMap_WC(Coordinates.x, Coordinates.y);
			if (temp.FoodStrength < 2)
			{
				temp.FoodStrength += 0.1f;
			}
			tilemap->WriteToMap_WC(Coordinates.x, Coordinates.y, temp);
			break;
		}
		case GOING_HOME:
		{

		}
		default:
		{
			break;
		}
	}
}

bool Colony::Ant::simulateNeeds()
{
	if (hunger > 0)
	{
		hunger -= hungerRate;
	}
	else
	{
		health -= starvationRate;
	}

	return (health <= 0);
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
		bool foundHomePhero = false;

		volatile tile CurTile = tilemap->ReadMap_WC(Coordinates.x, Coordinates.y);	//Tile the ant is curently standing on;

		if ((CurTile.type == FOOD) && (state == SCOUTING))
		{
			state = TRANSPORTING_FOOD;
			tile temp;
			temp.type = EMPTY;
			tilemap->WriteToMap_WC(Coordinates.x + (sin(angle + heading) * r), Coordinates.y + (cos(angle + heading) * r), temp);
			heading += M_PI;
		}

		headingDelta = heading;

		for (r = 4; r < viewDistance + 4; r++)
		{
			for (angle = -((float)FOV / 2); angle < ((float)FOV / 2); angle += 0.1f)
			{
				volatile tile ViewTile = tilemap->ReadMap_WC(Coordinates.x + (sin(angle + headingDelta) * r), Coordinates.y + (cos(angle + headingDelta) * r));	//Tile the ant is curently standing on
				volatile float coloDistX = abs(Coordinates.x - parentCol->colonyX);
				volatile float coloDistY = abs(Coordinates.y - parentCol->colonyY);
				float coloDist = sqrt((coloDistX * coloDistX) + (coloDistY * coloDistY));

				if (state == SCOUTING)
				{
					foundHomePhero = false;
					if (ViewTile.type == FOOD)
					{
						if (angle < 0)
						{
							heading -= pheromonAttraction*2;
						}
						else
						{
							heading += pheromonAttraction*2;
						}
						foundFood = true;
					}
					else if(!foundFood)
					{
						if (ViewTile.FoodStrength != 0)
						{
							if (angle < 0)
							{
								headingDelta -= pheromonAttraction * ViewTile.FoodStrength;
							}
							else
							{
								headingDelta += pheromonAttraction * ViewTile.FoodStrength;
							}
						}
					}
				}
				else if (state == TRANSPORTING_FOOD)
				{
					if (ViewTile.HomeStrength != 0)
					{
						foundHomePhero = true;
						if (angle < 0)
						{
							headingDelta -= pheromonAttraction * ViewTile.HomeStrength;
						}
						else
						{
							headingDelta += pheromonAttraction * ViewTile.HomeStrength;
						}
						if (coloDist < parentCol->colonySize)
						{
							state = SCOUTING;
							parentCol->food++;
							hunger = 100.0f;
							health = 100.0f;
						}
					}
				}
				//if (ViewTile.type == SOLID && !hasTurned)
				//{
				//	heading += M_PI;
				//	hasTurned = true;
				//}
			}
		}
		if (!foundHomePhero && SawHomePhero && state == TRANSPORTING_FOOD)
		{
			heading += M_PI;
		}
		else
		{
			if (!foundFood)
			{
				heading = headingDelta;
			}
			if (headingDelta > headingDeltaMax)
			{
				headingDelta = headingDeltaMax;
			}
			if (headingDelta < -headingDeltaMax)
			{
				headingDelta = -headingDeltaMax;
			}
		}
		SawHomePhero = foundHomePhero;
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
	bool isSorted = false;
	uint32_t i = 0;	//generic variable for array indication
	uint32_t antID = 0;	//Variable to indicate which ant from the Ants array is selected

	uint32_t buffer = 0;

	vector<uint32_t> DeadAnts;	//vector to store all antID of ants which are ready to die
	
	if (Ants.size() < 2500)
	{
		for (food; food > 0; food--)	//use stored food to make ants
		{
			addAnt();
		}
	}
	else
	{
		food = 0;
	}

	for (antID = 0; antID < Ants.size(); antID++)	//go through ants and simulate them
	{
		Ants[antID].checkArea();
		Ants[antID].placePheromone();
		Ants[antID].AntMove();
		if (Ants[antID].simulateNeeds())
		{
			DeadAnts.push_back(antID);
		}
	}

	if (DeadAnts.size())
	{
		for (i = DeadAnts.size(); i != 0; i--)
		{
			Ants.erase(Ants.begin() + DeadAnts[i - 1]);	//delete ant
		}
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


