#include "Perlin.h"

void Perlin1D::generateSeed()
{
	for (uint32_t i = 0; i < length; i++)
	{
		seed[i] = (float)(rand() % 100) / 100;	//generate seed;
	}
}

void Perlin1D::generateNoise(uint8_t OctaveCount, float scaleBias)
{
	float noiseF = 0.0f;
	float scaleF = 0.0f;
	float blendF = 0.0f;
	float sampleF = 0.0f;
	float scaleAccuF = 0.0f;

	uint32_t pitch = 0;
	uint32_t sample1 = 0;
	uint32_t sample2 = 0;

	if (scaleBias < 0)
	{
		scaleBias = 0.2f;
	}

	for (uint32_t x = 0; x < length; x++)
	{
		noiseF = 0.0f;
		scaleF = 1.0f;
		scaleAccuF = 0.0f;
		for (uint8_t oct = 0; oct < OctaveCount; oct++)
		{
			pitch = length >> oct;
			if (pitch != 0)
			{
				sample1 = (x / pitch) * pitch;
				sample2 = (sample1 + pitch) % length;

				blendF = (float)(x - sample1) / (float)pitch;
				sampleF = ((1.0f - blendF) * seed[sample1]) + (blendF * seed[sample2]);

				noiseF += sampleF * scaleF;
				scaleAccuF += scaleF;
				scaleF /= scaleBias;
			}
		}
		noise[x] = noiseF / scaleAccuF;
	}
}


void Perlin2D::generateSeed()
{
	for (uint64_t i = 0; i < (height * width); i++)
	{
		seed[i] = (float)(rand()) / (float)RAND_MAX;	//generate seed;
	}
}

void Perlin2D::generateNoise(uint8_t OctaveCount, float scaleBias)
{
	float noiseF = 0.0f;
	float scaleF = 0.0f;
	float blendFX = 0.0f;
	float blendFY = 0.0f;
	float sampleF1 = 0.0f;
	float sampleF2 = 0.0f;
	float scaleAccuF = 0.0f;

	uint32_t pitch = 0;
	uint32_t sampleX1 = 0;
	uint32_t sampleX2 = 0;
	uint32_t sampleY1 = 0;
	uint32_t sampleY2 = 0;

	if (scaleBias < 0)
	{
		scaleBias = 0.2f;
	}

	for (uint32_t x = 0; x < width; x++)
	{
		for (uint32_t y = 0; y < width; y++)
		{
			noiseF = 0.0f;
			scaleF = 1.0f;
			scaleAccuF = 0.0f;
			for (uint8_t oct = 0; oct < OctaveCount; oct++)
			{
				pitch = width >> oct;
				if (pitch != 0)
				{
					sampleX1 = (x / pitch) * pitch;
					sampleY1 = (y / pitch) * pitch;

					sampleX2 = (sampleX1 + pitch) % width;
					sampleY2 = (sampleY1 + pitch) % width;

					blendFX = (float)(x - sampleX1) / (float)pitch;
					blendFY = (float)(y - sampleY1) / (float)pitch;
					
					sampleF1 = ((1.0f - blendFX) * seed[sampleY1 * width + sampleX1]) + (blendFX * seed[sampleY1 * width + sampleX2]);
					sampleF2 = ((1.0f - blendFX) * seed[sampleY2 * width + sampleX1]) + (blendFX * seed[sampleY2 * width + sampleX2]);

					noiseF += (blendFY * (sampleF2 - sampleF1) + sampleF1) * scaleF;
					scaleAccuF += scaleF;
					scaleF /= scaleBias;
				}
				noise[x + (y*width)] = noiseF / scaleAccuF;
			}
		}
	}
}