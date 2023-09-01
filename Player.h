#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player();
	int diffX();
	int diffY();
	void shoot();
	void gainHealth();
	int shootDir;
	SDL_Surface* shield;
	double baseX, baseY, x, y, speed;
};

