#pragma once
#include "Character.h"

class Boss : public Character
{
public:
	Boss();
	void shoot(int now);
	void shoot2(int now);
	void shoot3Cross(int now);
	void shoot3Plus(int now);
	void shootBigBullet();
	void move(int worldTime, double delta);
	SDL_Surface* trapImage;
	SDL_Surface* bigBulletImage;
	Bullet bigBullet;
	int lastTrap;
	double x, y;
	int turn;
	int lastShot;
	int lastExplosion;
	int moveDirection;
};

