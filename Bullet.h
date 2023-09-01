#pragma once
#include "Common.h"

class Bullet
{
public:
	Bullet(int setX, int setY, int setDir, int setSpeed)
	{
		x = setX;
		y = setY;
		direction = setDir;
		speed = setSpeed;
	}
	Bullet() { x = 0; y = 0; direction = -1; speed = 0; };
	double x;
	double y;
	int direction;
	double speed;
	bool move(int time);
};

