#include "Window.h"
#include "HelperFunctions.h"	

Window::Window()
{
	fps = 0;
	level = 0;
	healthPointsCounter = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		exit(0);
	}

	int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);

	if (rc != 0)
	{
		SDL_Quit();
		exit(0);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "BULLET HELL");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	charset = SDL_LoadBMP("./images/cs8x8.bmp");
	if (charset == NULL)
	{
		quit();
		exit(0);
	}
	SDL_SetColorKey(charset, true, 0x000000);

	if (player.image == NULL) 
	{
		quit();
		exit(0);
	}

	if (boss.bulletImage == NULL)
	{
		quit();
		exit(0);
	}

	if (player.healthBar == NULL)
	{
		quit();
		exit(0);
	}
	player.shield = SDL_LoadBMP("./images/tarcza.bmp");
	game_over = SDL_LoadBMP("./images/game_over.bmp");
	victory = SDL_LoadBMP("./images/victory.bmp");
}

bool Window::run()
{
	srand(time(NULL));
	level = menu();
	loadLevel();

	exitGame = 0;
	char text[128];
	int frames = 0;
	double worldTime = 0;
	double fpsTimer = 0;
	int lastPlayerShot = 0;
	float score = 0;
	int lastHealthPoint = 0;

	int t1 = SDL_GetTicks();

	while (!exitGame)
	{
		int t2 = SDL_GetTicks();

		double delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		fpsTimer += delta;
		if (fpsTimer > 0.5)
		{
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		}

		DrawSurface(screen, background, (MAP_WIDTH / 2) + player.diffX(), (MAP_HEIGHT / 2) + player.diffY()); // tlo

		if (player.health > 0 && boss.health > 0) // strzelanie bossa
		{
			if (level == 1) // boss1
			{
				boss.shoot(t1);
			}

			if (level == 2) // boss2
			{
				if (int(worldTime) % 5 == 0 && worldTime > 1)
					boss.shootBigBullet();
				else
					boss.shoot2(t1);
			}

			if (level == 3) // boss3
			{
				srand(time(NULL));
				if (rand() % 2 + 1 == 1)
					boss.shoot3Plus(t1);
				else
					boss.shoot3Cross(t1);

				boss.move(worldTime, delta);
			}

			if (worldTime >= 1)
			{
				boss.moveBullets(delta * 1000);
				if (level == 2)
				{
					boss.bigBullet.move(delta * 1000);
					DrawSurface(screen, boss.bigBulletImage, boss.bigBullet.x + player.diffX(), boss.bigBullet.y + player.diffY());
				}
				for (int i = 0; i < boss.bulletCount; i++)
				{
					DrawSurface(screen, boss.bulletImage, boss.bullets[i].x + player.diffX(), boss.bullets[i].y + player.diffY()); // bullet

					if (t1 - player.lastHit > 3000)
					{ // hitbox gracza
						if (HitboxFunction(boss.bullets[i].x, boss.bullets[i].y, 20, 20, player.x, player.y, player.sizeX, player.sizeY))
						{
							player.lastHit = t1; // zmniejszanie hp graczowi
							score -= 100 * boss.hitStreak;
							boss.hitStreak += 0.2;
							player.hitStreak = 1;
							player.loseHealth();
							for (int j = i; j < boss.bulletCount - 1; j++)  // usuwanie trafionego bulleta bossa
							{
								boss.bullets[j] = boss.bullets[j + 1];
							}
							boss.bulletCount--;
							i--;
						}
						if (level == 2 && HitboxFunction(player.x, player.y, player.sizeX, player.sizeY, boss.bigBullet.x, boss.bigBullet.y, 100, 100))
						{
							player.lastHit = t1; // zmniejszanie hp graczowi (big bullet)
							score -= 100 * boss.hitStreak;
							boss.hitStreak += 0.2;
							player.hitStreak = 1;
							player.loseHealth();
							player.loseHealth();
						}
					}
					else
						DrawSurface(screen, player.shield, player.x + player.diffX(), player.y + player.diffY()); // tarcza
				}
			}

			player.moveBullets(delta * 1000); // strzelanie gracza
			for (int i = 0; i < player.bulletCount; i++) 
			{
				DrawSurface(screen, player.bulletImage, player.bullets[i].x + player.diffX(), player.bullets[i].y + player.diffY()); // bullet
				// hitbox bossa
				if (t1 - boss.lastHit > 1000)
				{
					if (HitboxFunction(player.bullets[i].x, player.bullets[i].y, 20, 20, boss.x, boss.y, boss.sizeX, boss.sizeY))
					{
						boss.lastHit = t1;  // zmniejszanie hp bossowi
						score += 100 * player.hitStreak;
						player.hitStreak += 0.2;
						boss.hitStreak = 1;
						boss.loseHealth();
						for (int j = i; j < player.bulletCount - 1; j++) // usuwanie trafionego bulleta gracza
						{
							player.bullets[j] = player.bullets[j + 1];
						}
						player.bulletCount--;
						i--;
						if (boss.health == 0)
						{
							if (level == 1)
								score += 200;
							if (level == 2)
								score += 500;
							if (level == 3)
								score += 1000;
						}
					}
				}
			}
			if (t1 - lastHealthPoint > 4000) // spawn health point
			{
				lastHealthPoint = t1;
				spawnHealthPoint();
			}

			for (int i = 0; i < healthPointsCounter; i++)
			{
				DrawSurface(screen, healthPoints[i].image, healthPoints[i].x + player.diffX(), healthPoints[i].y + player.diffY()); // wypisywanie health pointow
				if (HitboxFunction(healthPoints[i].x, healthPoints[i].y, 60, 60, player.x, player.y, player.sizeX, player.sizeY))
				{
					for (int j = i; j < healthPointsCounter - 1; j++) // usuwanie zebranego health pointa
					{
						healthPoints[j] = healthPoints[j + 1];
					}
					healthPointsCounter--;
					i--;
					player.gainHealth(); // odzyskiwanie hp
					score += 20;
				}
			}
		}

		DrawSurface(screen, boss.image, boss.x + player.diffX(), boss.y + player.diffY()); // boss
		DrawSurface(screen, boss.healthBar, boss.x + player.diffX(), boss.y + player.diffY() - 120); // pasek hp bossa 

		DrawSurface(screen, player.image, player.x + player.diffX(), player.y + player.diffY()); // postac
		DrawSurface(screen, player.healthBar, player.x + player.diffX(), player.y + player.diffY() - 50); // pasek hp gracza

		winLose(); // wygrana / przegrana

		texts(worldTime); // teksty w ramkach

		DrawRectangle(screen, 4, 34, 170, 20, ALMOND, NAVY); // wynik
		sprintf(text, "twoj wynik: %.0lf", score);
		DrawString(screen, 10, 40, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_Event event1; // EVENTY

		while (SDL_PollEvent(&event1))
		{
			switch (event1.type)
			{
			case SDL_KEYDOWN:
				if (event1.key.keysym.sym == SDLK_ESCAPE)
					exitGame = 1;
				else if (event1.key.keysym.sym == SDLK_n)
				{
					quit();
					return true;
				}
				else if (event1.key.keysym.sym == SDLK_LSHIFT)
					player.speed = 2;
				else if (event1.key.keysym.sym == SDLK_w) // sztrzelanie gracza
				{
					player.shootDir = 360;
				}
				else if (event1.key.keysym.sym == SDLK_a)
				{
					player.shootDir = 270;
				}
				else if (event1.key.keysym.sym == SDLK_s)
				{
					player.shootDir = 180;
				}
				else if (event1.key.keysym.sym == SDLK_d)
				{
					player.shootDir = 90;
				}
				if (event1.key.keysym.sym == SDLK_w || event1.key.keysym.sym == SDLK_a || event1.key.keysym.sym == SDLK_s || event1.key.keysym.sym == SDLK_d)
				{
					if (t1 - lastPlayerShot > 1000)
					{
						lastPlayerShot = t1;
						player.shoot();
					}
				}
				if (event1.key.keysym.sym == SDLK_l && boss.health == 0 && level == 1) // przejscie na nowy level
				{
					boss.health = 5;
					level += 1;
					worldTime = 0;
					loadLevel();
				}
				if (event1.key.keysym.sym == SDLK_l && boss.health == 0 && level == 2)
				{
					boss.health = 5;
					level += 1;
					worldTime = 0;
					loadLevel();
				}
				break;
			case SDL_KEYUP:
				if (event1.key.keysym.sym == SDLK_LSHIFT)
					player.speed = 1;
			}
		}
		
		const Uint8* keyboardStateArray = SDL_GetKeyboardState(NULL); // poruszanie gracza
		SDL_PollEvent(&event1);
		
		if (event1.type == SDL_KEYDOWN || event1.type == SDL_KEYUP)
		{
			if (keyboardStateArray[SDL_SCANCODE_UP] && !(keyboardStateArray[SDL_SCANCODE_DOWN]) && player.health > 0)
			{
				if (player.y > 27)
					player.y -= 1 * player.speed * delta * 1000 / TIMESPEED;
			}
			else if (!keyboardStateArray[SDL_SCANCODE_UP] && keyboardStateArray[SDL_SCANCODE_DOWN] && player.health > 0)
			{
				if (player.y < MAP_HEIGHT - 27)
					player.y += 1 * player.speed * delta * 1000 / TIMESPEED;
			}
		
			if (keyboardStateArray[SDL_SCANCODE_RIGHT] && !keyboardStateArray[SDL_SCANCODE_LEFT] && player.health > 0)
			{
				if (player.x < MAP_WIDTH - 24)
				{
					player.x += 1 * player.speed * delta * 1000 / TIMESPEED;
					player.image = SDL_LoadBMP("./images/postac.bmp");
				}
		
			}
			else if (!keyboardStateArray[SDL_SCANCODE_RIGHT] && keyboardStateArray[SDL_SCANCODE_LEFT] && player.health > 0)
			{
				if (player.x > 24)
				{
					player.x -= 1 * player.speed * delta * 1000 / TIMESPEED;
					player.image = SDL_LoadBMP("./images/postac_left.bmp");
				}
			}
		}
		else if (event1.type == SDL_QUIT)
		{
			exitGame = 1;
			break;
		}
		frames++;
	}
	quit();
	return false;
}

void Window::loadLevel()
{
	boss.health = 5;
	boss.healthBar = SDL_LoadBMP("./images/hp5.bmp");
	boss.bulletCount = 0;
	boss.lastShot = 0;
	boss.turn = 0;
	healthPointsCounter = 0;
	
	char BGFile[19];
	sprintf(BGFile, "./images/tlo%d.bmp", level);
	background = SDL_LoadBMP(BGFile);
	if (background == NULL)
	{
		quit();
		exit(0);
	}

	char bossFile[20];
	sprintf(bossFile, "./images/boss%d.bmp", level);
	boss.image = SDL_LoadBMP(bossFile);
	if (boss.image == NULL)
	{
		quit();
		exit(0);
	}

	if (level == 1)
	{
		boss.sizeX = 120;
		boss.sizeY = 188;
	}
	if (level == 2)
	{
		boss.bulletImage = SDL_LoadBMP("./images/bullet2.bmp");
		boss.sizeX = 300;
		boss.sizeY = 230;
	}
	if (level == 3)
	{
		boss.bulletImage = SDL_LoadBMP("./images/bullet3.bmp");
		boss.sizeX = 200;
		boss.sizeY = 239;
	}
}

void Window::quit()
{
	if (charset)
		SDL_FreeSurface(charset);
	if (screen)
		SDL_FreeSurface(screen);
	if(scrtex)
		SDL_DestroyTexture(scrtex);
	if (renderer)
		SDL_DestroyRenderer(renderer);
	if (window)
		SDL_DestroyWindow(window);
	if (player.image)
		SDL_FreeSurface(player.image);
	if (boss.bulletImage)
		SDL_FreeSurface(boss.bulletImage);
	SDL_Quit();
}

void Window::texts(double time)
{
	char text[128];
	DrawRectangle(screen, 310, 4, 340, 20, ALMOND, NAVY); // tytul
	sprintf(text, "B  U  L  L  E  T     H  E  L  L");
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

	DrawRectangle(screen, 4, 4, 192, 20, ALMOND, NAVY); // timer
	sprintf(text, "czas trwania = %.1lf s", time);
	DrawString(screen, 10, 10, text, charset);

	DrawRectangle(screen, 822, 4, 134, 20, ALMOND, NAVY); // fpsy
	sprintf(text, "%.0lf klatek / s", fps);
	DrawString(screen, 830, 10, text, charset);

	DrawRectangle(screen, 4, 676, 140, 40, ALMOND, NAVY); // wyjscie / nowa gra
	sprintf(text, "'Esc' - wyjscie");
	DrawString(screen, 10, 682, text, charset);
	sprintf(text, "'n' - nowa gra");
	DrawString(screen, 10, 702, text, charset);

	DrawRectangle(screen, 360, 696, 240, 20, ALMOND, NAVY); // autor
	sprintf(text, "Autor: Radoslaw Gajewski");
	DrawString(screen, 385, 702, text, charset);

	DrawRectangle(screen, 816, 650, 140, 66, ALMOND, NAVY); // poruszanie
	sprintf(text, "PORUSZANIE");
	DrawString(screen, 850, 660, text, charset);
	sprintf(text, "\030");							// strzalka w gore
	DrawString(screen, 886, 675, text, charset);
	sprintf(text, "\032");							// strzalka w prawo
	DrawString(screen, 866, 685, text, charset);
	sprintf(text, "\031");							// strzalka w dol
	DrawString(screen, 886, 700, text, charset);
	sprintf(text, "\033 ");							// strzalka w lewo
	DrawString(screen, 906, 685, text, charset);

	DrawRectangle(screen, 816, 580, 140, 66, ALMOND, NAVY); // strzelanie
	sprintf(text, "STRZELANIE");
	DrawString(screen, 850, 590, text, charset);
	sprintf(text, "W");							// w
	DrawString(screen, 886, 605, text, charset);
	sprintf(text, "D");							// d
	DrawString(screen, 866, 615, text, charset);
	sprintf(text, "S");							// s
	DrawString(screen, 886, 630, text, charset);
	sprintf(text, "A ");							// a
	DrawString(screen, 906, 615, text, charset);
}

int Window::menu()
{
	menuBackground = SDL_LoadBMP("./images/menu.bmp");
	int menuExit = 0;
	while (!menuExit)
	{
		char text[128];

		DrawSurface(screen, menuBackground, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

		DrawRectangle(screen, 310, 4, 340, 20, ALMOND, NAVY); // tytul
		sprintf(text, "B  U  L  L  E  T     H  E  L  L");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

		DrawRectangle(screen, 250, 184, 460, 360, VIOLET, GRANATOWY); // menu
		sprintf(text, "   M  E  N  U");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 210, text, charset);

		DrawRectangle(screen, 310, 254, 340, 20, ALMOND, NAVY); // poziom 1
		sprintf(text, "   POZIOM I - WCISNIJ '1'");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 260, text, charset);

		DrawRectangle(screen, 310, 304, 340, 20, ALMOND, NAVY); // poziom 2
		sprintf(text, "   POZIOM II - WCISNIJ '2'");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 310, text, charset);

		DrawRectangle(screen, 310, 354, 340, 20, ALMOND, NAVY); // poziom 3
		sprintf(text, "   POZIOM III - WCISNIJ '3'");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 360, text, charset);

		DrawRectangle(screen, 310, 454, 340, 20, ALMOND, NAVY); // wyjscie
		sprintf(text, "   WYJSCIE - WCISNIJ 'ESCAPE'");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 460, text, charset);

		DrawRectangle(screen, 360, 696, 240, 20, ALMOND, NAVY); // autor
		sprintf(text, "Autor: Radoslaw Gajewski");
		DrawString(screen, 385, 702, text, charset);


		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_Event menuEvent;
		const Uint8* keyboardStateArrayMenu = SDL_GetKeyboardState(NULL);
		SDL_PollEvent(&menuEvent);
		
		if (menuEvent.type == SDL_KEYDOWN)
		{
			if (keyboardStateArrayMenu[SDL_SCANCODE_ESCAPE])
			{
				menuExit = 1;
				quit();
				exit(0);
			}
			else if (keyboardStateArrayMenu[SDL_SCANCODE_1])
			{
				return 1;
			}
			else if (keyboardStateArrayMenu[SDL_SCANCODE_2])
				return 2;
			else if (keyboardStateArrayMenu[SDL_SCANCODE_3])
				return 3;
		}
		else if (menuEvent.type == SDL_QUIT)
		{
			menuExit = 1;
			quit();
			exit(0);
		}
	}
}

void Window::spawnHealthPoint()
{
	if (healthPointsCounter < 10)
	{
		int y = rand() % MAP_HEIGHT;
		int x = rand() % MAP_WIDTH;
		healthPoints[healthPointsCounter] = HealthPoint(x, y);
		healthPointsCounter++;
	}
}

void Window::winLose()
{
	if (boss.health == 0) // wygrana
	{
		if (level == 3)
		{
			char text[128];
			DrawRectangle(screen, 290, 634, 380, 20, ALMOND, NAVY); // Przejscie gry
			sprintf(text, "Gratuluje ukonczenia gry!");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 640, text, charset);
		}

		DrawSurface(screen, victory, 480, 130);
		if (level != 3)
		{
			char text[128];
			DrawRectangle(screen, 290, 34, 380, 20, ALMOND, NAVY); // nastepny poziom
			sprintf(text, "Wcisnij 'l' aby przejsc do nastepnego poziomu");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 40, text, charset);
		}
	}

	if (player.health == 0) // przegrana
	{
		DrawSurface(screen, game_over, 480, 130);
		player.image = SDL_LoadBMP("./images/postac_martwa.bmp");
	}
}