#pragma once
#include "Common.h"

#define CZARNY SDL_MapRGB(screen->format, 0x00, 0x00, 0x00)
#define ZIELONY SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00)
#define CZERWONY SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00)
#define NIEBIESKI SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC)
#define NAVY SDL_MapRGB(screen->format, 0x1E, 0x3D, 0x59)
#define ALMOND SDL_MapRGB(screen->format, 0xF5, 0xF0, 0xE1)
#define VIOLET SDL_MapRGB(screen->format, 0x32, 0x0D, 0x3E)
#define GRANATOWY SDL_MapRGB(screen->format, 0x1E, 0x27, 0x61)

// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	}
}


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
}


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	}
}


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
}

bool IsPointInRect(int pointX, int pointY, int minX, int maxX, int minY, int maxY)
{
	if (pointX > minX && pointX < maxX && pointY > minY && pointY < maxY)
		return true;
	return false;
}

bool HitboxFunction(int fMiddleX, int fMiddleY, int fSizeX, int fSizeY, int sMiddleX, int sMiddleY, int sSizeX, int sSizeY)
{
	if (IsPointInRect(fMiddleX - fSizeX / 2, fMiddleY - fSizeY / 2, sMiddleX - sSizeX / 2, sMiddleX + sSizeX / 2, sMiddleY - sSizeY / 2, sMiddleY + sSizeY / 2))
		return true;
	if (IsPointInRect(fMiddleX + fSizeX / 2, fMiddleY + fSizeY / 2, sMiddleX - sSizeX / 2, sMiddleX + sSizeX / 2, sMiddleY - sSizeY / 2, sMiddleY + sSizeY / 2))
		return true;
	if (IsPointInRect(fMiddleX - fSizeX / 2, fMiddleY + fSizeY / 2, sMiddleX - sSizeX / 2, sMiddleX + sSizeX / 2, sMiddleY - sSizeY / 2, sMiddleY + sSizeY / 2))
		return true;
	if (IsPointInRect(fMiddleX + fSizeX / 2, fMiddleY - fSizeY / 2, sMiddleX - sSizeX / 2, sMiddleX + sSizeX / 2, sMiddleY - sSizeY / 2, sMiddleY + sSizeY / 2))
		return true;

	return false;
}