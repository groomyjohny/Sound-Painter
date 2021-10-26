#include <iostream>
#include <SDL\SDL.h>

#include "Helpers.h"

#pragma comment(lib,"SDL2.lib")
#undef main
int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	int w = 1280;
	int h = 720;
	int windowX = SDL_WINDOWPOS_CENTERED;
	int windowY = SDL_WINDOWPOS_CENTERED;

	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			//input processing goes here
		}

		//frame rendering goes here
	}

	std::cin.ignore(999999);
	std::cin.get();
	return 0;
}