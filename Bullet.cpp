#include "Bullet.h"

bool Bullet::move(int time)
{
	double newSpeed = time * speed / TIMESPEED;
	if (direction == 45 || direction == 90 || direction == 135)
		x += 1.6 * newSpeed;
	else if (direction == 225 || direction == 270 || direction == 315)
		x -= 1.6 * newSpeed;
	else if (direction == 23 || direction == 158)
		x += 0.7 * newSpeed;
	else if (direction == 338 || direction == 203)
		x -= 0.7 * newSpeed;
	else if (direction == 68 || direction == 113)
		x += 1.5 * newSpeed;
	else if (direction == 248 || direction == 293)
		x -= 1.5 * newSpeed;

	if (direction == 135 || direction == 180 || direction == 225)
		y += 1.6 * newSpeed;
	else if (direction == 45 || direction == 315 || direction == 360)
		y -= 1.6 * newSpeed;
	else if (direction == 23 || direction == 338)
		y -= 1.5 * newSpeed;
	else if (direction == 158 || direction == 203)
		y += 1.5 * newSpeed;
	else if (direction == 68 || direction == 293)
		y -= 0.7 * newSpeed;
	else if (direction == 248 || direction == 113)
		y += 0.7 * newSpeed;

	if (x < 0 || y < 0 || x > MAP_WIDTH || y > MAP_HEIGHT)
		return true;
	else
		return false;
}