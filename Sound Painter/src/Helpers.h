#pragma once
#include <SDL/SDL.h>
namespace Helpers
{
	static Uint32 rgba2uint(SDL_PixelFormat* format, uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		Uint32 color = 0;
		color |= b << format->Bshift;
		color |= g << format->Gshift;
		color |= r << format->Rshift;
		color |= a << format->Ashift;
		return color;
	}

	static Uint32* getPixelAddress(SDL_Surface* s, int x, int y)
	{
		SDL_PixelFormat* format = s->format;
		char* px = (char*)s->pixels + s->pitch*y + x * format->BytesPerPixel;
		return (Uint32*)px;
	}

	static void setPixel(SDL_Surface* s, int x, int y, uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		if (x < 0 || y < 0 || x >= s->w || y >= s->h) return;
		SDL_PixelFormat* format = s->format;
		Uint32 color = rgba2uint(format, r, g, b, a);

		*getPixelAddress(s, x, y) = color;
	}

	static void setPixel(SDL_Surface* s, int x, int y, uint32_t r, uint32_t g, uint32_t b)
	{
		setPixel(s, x, y, r, g, b, SDL_ALPHA_OPAQUE);
	}	

	static void getPixel(SDL_Surface* s, int x, int y, uint8_t& retR, uint8_t& retG, uint8_t& retB, uint8_t& retA)
	{
		if (x < 0 || y < 0 || x >= s->w || y >= s->h) return;
		SDL_PixelFormat* format = s->format;
		char* px = (char*)s->pixels + s->pitch*y + x * format->BytesPerPixel;
		Uint32 color = *(Uint32*)px;

		retR = (color & format->Rmask) >> format->Rshift;
		retG = (color & format->Gmask) >> format->Gshift;
		retB = (color & format->Bmask) >> format->Bshift;
		retA = (color & format->Amask) >> format->Ashift;
	}
}