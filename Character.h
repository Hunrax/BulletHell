#pragma once
#include "Bullet.h"

class Character
{
public:
	void loseHealth();
	void moveBullets(int time);
	SDL_Surface* healthBar;
	int health;
	SDL_Surface* image;
	SDL_Surface* bulletImage;
	Bullet bullets[200];
	int bulletCount;
	int lastHit;
	int sizeX, sizeY;
	double hitStreak;
};

