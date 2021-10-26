#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <SDL\SDL.h>
#include <string>

#include "Helpers.h"

#pragma comment(lib,"SDL2.lib")
#undef main
int main()
{
	srand(time_t(0));
	SDL_Init(SDL_INIT_EVERYTHING);

	int w = 1280;
	int h = 720;
	int windowX = SDL_WINDOWPOS_CENTERED;
	int windowY = SDL_WINDOWPOS_CENTERED;

	SDL_Window* wnd;
	SDL_Renderer* rend;
	SDL_CreateWindowAndRenderer(w, h, 0, &wnd, &rend);

	std::vector<SDL_Point> points;
	std::map<int, int> pointMap;

	bool mouseButtonDown = false;
	while (true)
	{
		SDL_Event events;
		while (SDL_PollEvent(&events))
		{
			//input processing goes here
			if (events.key.keysym.scancode == SDL_SCANCODE_S)
			{
				//save routine
				double lowFrq = 20;
				double highFrq = 20e3;
				double dbFloor = 96;

				double lnLow = log(lowFrq);
				double lnHigh = log(highFrq);			
				
				std::string name = std::to_string(rand()) + std::to_string(rand())+".txt";
				std::ofstream out(name);
				out << "spectrum\n30\n";
				
				for (auto& it : pointMap)
				{
					double x = it.first;
					double y = it.second;

					double frq = exp(lnLow + x / w * (lnHigh - lnLow));
					//double intensity = y / h;
					double intensity = -dbFloor*y / h;

					out << frq << " " << intensity << "\n";
				}
			}
			if (events.key.keysym.scancode == SDL_SCANCODE_C)
			{
				//clear routine
				pointMap.clear();
			}

			
			if (events.type == SDL_MOUSEBUTTONDOWN)  //mouse click
			{
				
				auto button = events.button.button;
				if (button == SDL_BUTTON_LEFT) mouseButtonDown = true;
			}

			if (events.type == SDL_MOUSEMOTION && mouseButtonDown)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				pointMap[x] = y;
			}

			if (events.type == SDL_MOUSEBUTTONUP)
			{
				auto button = events.button.button;
				if (button == SDL_BUTTON_LEFT) mouseButtonDown = false;
			}
			
			if (events.key.keysym.scancode == SDL_SCANCODE_ESCAPE) exit(-1);
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

		points.clear();
		for (auto& it : pointMap) points.emplace_back(SDL_Point{ it.first,it.second });
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