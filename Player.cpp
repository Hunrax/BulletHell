#include "Player.h"

Player::Player()
{
	image = SDL_LoadBMP("./images/postac.bmp");
	health = 5;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT / 2;
	baseX = SCREEN_WIDTH / 2;
	baseY = SCREEN_HEIGHT / 2;
	speed = 1;
	bulletCount = 0;
	bulletImage = SDL_LoadBMP("./images/player_bullet.bmp");
	healthBar = SDL_LoadBMP("./images/hp5.bmp");
	lastHit = 0;
	sizeX = 48;
	sizeY = 55;
	hitStreak = 1;
}

int Player::diffX()
{
	if (x < SCREEN_WIDTH / 2)
		return baseX - SCREEN_WIDTH / 2;
	else if (x > MAP_WIDTH - SCREEN_WIDTH / 2)
		return baseX - (MAP_WIDTH - SCREEN_WIDTH / 2);
	else
		return baseX - x;
}

int Player::diffY() 
{ 
	if (y < SCREEN_HEIGHT / 2)
		return baseY - SCREEN_HEIGHT / 2;
	else if (y > MAP_HEIGHT - SCREEN_HEIGHT / 2)
		return baseY - (MAP_HEIGHT - SCREEN_HEIGHT / 2);
	else
		return baseY - y; 
}

void Player::shoot()
{
	bullets[bulletCount] = Bullet(x, y, shootDir, 2);
	bulletCount++;
}

void Player::gainHealth()
{
	if (health < 5)
		health++;
	char healthBarFile[17];
	sprintf(healthBarFile, "./images/hp%d.bmp", health);
	healthBar = SDL_LoadBMP(healthBarFile);
}