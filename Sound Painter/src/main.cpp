#include <iostream>
#include <vector>
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

	SDL_Window* wnd;
	SDL_Renderer* rend;
	SDL_CreateWindowAndRenderer(w, h, 0, &wnd, &rend);

	std::vector<SDL_Point> points;

	while (true)
	{
		SDL_Event events;
		while (SDL_PollEvent(&events))
		{
			//input processing goes here
			if (events.key.keysym.scancode == SDL_SCANCODE_S)
			{
				//save routine
			}
			if (events.key.keysym.scancode == SDL_SCANCODE_C)
			{
				//clear routine
				points.clear();
			}

			//if (events.type == SDL_MOUSEMOTION) this->onMouseMotion();
			if (events.type == SDL_MOUSEBUTTONDOWN)  //mouse click
			{
				auto button = events.button.button;
				int x, y;
				SDL_GetMouseState(&x, &y);

				if (button == SDL_BUTTON_LEFT)
				{
					points.emplace_back(SDL_Point{ x,y });
				}
			}
			
			if (events.key.keysym.scancode == SDL_SCANCODE_ESCAPE) exit(-1);
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
		if (!points.empty()) SDL_RenderDrawLines(rend, &points.front(), points.size());
		//frame rendering goes here
		SDL_RenderPresent(rend);

		SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(rend);
	}

	std::cin.ignore(999999);
	std::cin.get();
	return 0;
}