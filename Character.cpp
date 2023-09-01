#include "Character.h"

void Character::loseHealth()
{
	if (health > 0)
		health--;
	char healthBarFile[17];
	sprintf(healthBarFile, "./images/hp%d.bmp", health);
	healthBar = SDL_LoadBMP(healthBarFile);
}

void Character::moveBullets(int time)
{
	for (int i = 0; i < bulletCount; i++)
	{
		bool toBeRemoved = bullets[i].move(time);
		if (toBeRemoved) // usuwanie bulletow ktore wyszly za plansze
		{
			for (int j = i; j < bulletCount - 1; j++)
			{
				bullets[j] = bullets[j + 1];
			}
			bulletCount--;
			i--;
		}
	}
}
