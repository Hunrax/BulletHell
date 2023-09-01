#pragma once
#include "Common.h"

class HealthPoint
{
public:
	HealthPoint(int setX, int setY)
	{
		x = setX;
		y = setY;
		image = SDL_LoadBMP("./images/health_point.bmp");
	}
	HealthPoint() { x = 0; y = 0; };
	SDL_Surface* image;
	int x, y;
};

