#pragma once

//Made with help of javidx9s Programming Perlin-like Noice (C++) video

#include<iostream>

class Perlin1D
{
public:
	Perlin1D(uint32_t Length)
	{
		seed = new float[Length];
		noise = new float[Length];
		length = Length;
		generateSeed();
	};

	~Perlin1D()
	{
		delete[] seed;
		delete[] noise;

		seed = nullptr;
		noise = nullptr;
	};
	
	void generateSeed();
	void generateNoise(uint8_t OctaveCount, float scaleBias = 2.0f);

	float* seed = nullptr;
	float* noise = nullptr;
	uint32_t length;

private:

};

class Perlin2D
{
public:
	Perlin2D(uint32_t Height, uint32_t Width)
	{
		seed = new float[Height * Width];
		noise = new float[Height * Width];
		height = Height;
		width = Width;
		generateSeed();
	};

	~Perlin2D()
	{
		delete[] seed;
		delete[] noise;

		seed = nullptr;
		noise = nullptr;
	};

	void generateSeed();
	void generateNoise(uint8_t OctaveCount, float scaleBias = 2.0f);

	float* seed = nullptr;
	float* noise = nullptr;
	uint32_t height = 0;
	uint32_t width = 0;

private:

};