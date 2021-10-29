#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <SDL\SDL.h>
#include <string>
#include <ctime>

#include <adm/Timer.h>
#include "Mixer.h"
#include "Helpers.h"

#pragma comment(lib,"SDL2.lib")
#undef main

double lowFrq = 20;
double highFrq = 20e3;
double dbFloor = 96;

double lnLow = log(lowFrq);
double lnHigh = log(highFrq);

double AUDIO_BUFFER_DURATION = 0.1;
adm::Timer TIMER(false);
Mixer MIXER;

Event mousePosToEvent(int x, int y, int w, int h)
{
	Event ret;
	double frq = exp(lnLow + double(x) / w * (lnHigh - lnLow));
	double db = -dbFloor * double(y) / h;

	ret.tBegin = -1;
	ret.tEnd = -1;
	ret.frequency = frq;
	ret.amplitude = pow(10, db / 10);
	return ret;
}

void fill_audio(void *udata, Uint8 *stream, int len)
{
	double t = TIMER.getTime();
	double cycleTime = 1.0 / 48000;
	for (int i = 0; i < len; ++i)
	{
		((float*)(stream))[i] = MIXER.getSample(t);
		t += cycleTime;
	}
}
int main()
{
	srand(time(0));
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
	int error = SDL_GL_SetSwapInterval(1);
	std::cout << "Failed to set vsync: " << SDL_GetError() << "\n";

	SDL_AudioSpec desired,obtained;
	desired.freq = 44100;
	desired.format = AUDIO_F32SYS;
	desired.channels = 1;
	desired.samples = 512;
	desired.userdata = 0;
	desired.callback = fill_audio;

	//int openAudioError = SDL_OpenAudio(&desired, &obtained);
	SDL_AudioDeviceID audioDeviceId = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	//SDL_QueueAudio(dev, buf.data(), sizeof(float) * buf.size());

		
	TIMER.resume();
	std::vector<float> audioBuffer;

	while (true)
	{
		SDL_Event events;
		while (SDL_PollEvent(&events))
		{
			//input processing goes here
			if (events.key.keysym.scancode == SDL_SCANCODE_S)
			{
				//save routine
							
				
				std::string name;
				std::string chars[2] = { "bcdfghjklmnpqrstvwxyz", "aeiou" };
				for (int i = 0; i < 10; ++i) name += chars[i % 2][rand() % chars[i % 2].size()];

				//SDL_SaveBMP(SDL_GetWindowSurface(wnd), std::string(name+".bmp").c_str()); //save image
				std::string txtName = name + ".txt";
				std::ofstream out(txtName);
				out << "spectrum\n20\n";
				
				for (auto& it : points)
				{
					double x = it.x;
					double y = it.y;

					double frq = exp(lnLow + x / w * (lnHigh - lnLow));					
					double intensity = -dbFloor*y / h;

					out << frq << " " << intensity << "\n";
				}

				out.close();

				Mixer mx(txtName);
				std::string wavName = name + ".wav";
				mx.saveToFile(wavName);
			}
			if (events.key.keysym.scancode == SDL_SCANCODE_C)
			{
				//clear routine
				points.clear();
				MIXER.clear();
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
				points.emplace_back(SDL_Point{ x,y });

				MIXER.addEvent(mousePosToEvent(x, y, w, h));
			}

			if (events.type == SDL_MOUSEBUTTONUP)
			{
				auto button = events.button.button;
				if (button == SDL_BUTTON_LEFT) mouseButtonDown = false;
			}
			
			if (events.key.keysym.scancode == SDL_SCANCODE_ESCAPE) SDL_Quit();

			if (events.type == SDL_QUIT) SDL_Quit();
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

		//SDL_ClearQueuedAudio(audioDeviceId);
		//double currT = timer.getTime();
		//audioBuffer = MIXER.getSamplesFromUntil(currT, currT + AUDIO_BUFFER_DURATION, obtained);
		//SDL_QueueAudio(audioDeviceId, (void*)&audioBuffer.front(), sizeof(float)*audioBuffer.size());
		SDL_PauseAudioDevice(audioDeviceId, 0);
		/*points.clear();
		for (auto& it : pointMap) points.emplace_back(SDL_Point{ it.first,it.second });*/
		if (!points.empty()) SDL_RenderDrawPoints(rend, &points.front(), points.size());
		//frame rendering goes here
		SDL_RenderPresent(rend);

		SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(rend);
	}

	std::cin.ignore(999999);
	std::cin.get();
	return 0;
}