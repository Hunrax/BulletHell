#pragma once
#include "Player.h"
#include "Boss.h"
#include "HealthPoint.h"

class Window
{
public:
	Window();
	bool run();
	void quit();
	void texts(double time);
	int menu();
	void winLose();
private:
	void loadLevel();
	void spawnHealthPoint();
	void ifBulletHit();
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* charset;
	SDL_Texture* scrtex;
	SDL_Surface* screen;
	SDL_Surface* background;
	SDL_Surface* menuBackground;
	SDL_Surface* game_over;
	SDL_Surface* victory;
	Player player;
	Boss boss;
	HealthPoint healthPoints[10];
	int healthPointsCounter;
	float fps;
	int level;
	int exitGame;
};

