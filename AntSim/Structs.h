#pragma once

#include<iostream>

struct vec2D
{
	double x = 0;
	double y = 0;
	bool operator==(vec2D vec2)
	{
		return((x == vec2.x) && (y == vec2.y));
	}
};
	
struct Color
{
	float r = 0;
	float g = 0;
	float b = 0;
};