#include "Boss.h"
#include <cmath>

Boss::Boss()
{
	x = MAP_WIDTH / 2;
	y = MAP_HEIGHT / 2;
	bulletCount = 0;
	lastShot = 0;
	turn = 0;
	health = 5;
	healthBar = SDL_LoadBMP("./images/hp5.bmp");
	lastHit = 0;
	hitStreak = 1;
	lastTrap = 0;
	bulletImage = SDL_LoadBMP("./images/bullet1.bmp");
	bigBulletImage = SDL_LoadBMP("./images/big_bullet2.bmp");
	srand(time(NULL));
}

void Boss::shoot(int now)
{
	if (now - lastShot > 1000)
	{
		lastShot = now;
		for (int i = 0; i < 8; i++)
		{
			int angle = 0;
			if (turn % 2 == 0)
				angle = 45 * ((i % 8) + 1);
			else if (turn % 2 == 1)
				angle = 45 * ((i % 8) + 1) + 23;
			if (angle > 360)
				angle -= 360;
			bullets[bulletCount] = Bullet(x, y, angle, 1);
			bulletCount++;
		}
		turn++;
	}
}

void Boss::move(int worldTime, double delta)
{
	double speed = 1 * delta * 1000 / TIMESPEED / 3;
	if (worldTime % 10 == 0)
	{
		srand(time(NULL));
		moveDirection = (rand() % 4 + 1)*45;
	}

	if (moveDirection == 45 || moveDirection == 90 || moveDirection == 135)
		if (x < MAP_WIDTH - 125)
			x += 1 * speed;
		else
			moveDirection += 180;
	else if (moveDirection == 225 || moveDirection == 270 || moveDirection == 315)
		if(x > 125)
			x -= 1 * speed;
		else
			moveDirection += 180;
	if (moveDirection == 135 || moveDirection == 180 || moveDirection == 225)
		if(y < MAP_HEIGHT - 195)
			y += 1 * speed;
		else
			moveDirection += 180;
	else if (moveDirection == 45 || moveDirection == 315 || moveDirection == 360)
		if(y > 195)
			y -= 1 * speed;
		else
			moveDirection += 180;
}

void Boss::shoot2(int now)
{
	if (now - lastShot > 75)
	{
		lastShot = now;
		int angle = 22 * (turn+1) + 1;
		if (turn % 2 == 0)
		{
			for (int i = 0; i < turn; i += 2)
				angle += 1;
		}
		if (turn % 2 == 1)
			angle += turn / 2;

		if (turn < 16)
			turn++;
		if (turn == 16)
			turn = 0;

		bullets[bulletCount] = Bullet(x, y, angle, 1);
		bulletCount++;
	}
}

void Boss::shoot3Cross(int now)
{
	if (now - lastShot > 100)
	{
		lastShot = now;
		for (int i = 0; i < 4; i++)
		{
			int angle = 90 * (i + 1) + 45;
			if (angle > 360)
				angle -= 360;

			bullets[bulletCount] = Bullet(x, y, angle, 1);
			bulletCount++;
		}
	}
}

void Boss::shoot3Plus(int now)
{
	if (now - lastShot > 100)
	{
		lastShot = now;
		for (int i = 0; i < 4; i++)
		{
			int angle = 90 * (i + 1);
			if (angle > 360)
				angle -= 360;

			bullets[bulletCount] = Bullet(x, y, angle, 1);
			bulletCount++;
		}
	}
}
void Boss::shootBigBullet()
{
	int angle = 45 * (rand() % 8 + 1);
	bigBullet = Bullet(x, y, angle, 1);
}