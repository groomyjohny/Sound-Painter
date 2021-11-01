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
const int AUDIO_BUFFER_SAMPLES = 512;
const int AUDIO_BUFFER_RATE = 44100;
adm::Timer TIMER(false);
Mixer MIXER;

std::array<double, MixerEvent::LOOKUP_TABLE_SAMPLES> MixerEvent::lookupTable;

MixerEvent mousePosToEvent(int x, int y, int w, int h)
{
	double frq = exp(lnLow + double(x) / w * (lnHigh - lnLow));
	double db = -dbFloor * double(y) / h;
	return MixerEvent(frq, pow(10, db / 10));
}

void fill_audio(void *udata, Uint8 *stream, int len)
{
	double t = TIMER.getTime();
	double cycleTime = 1.0 / AUDIO_BUFFER_RATE;
	for (int i = 0; i < len/sizeof(float); ++i)
	{
		((float*)(stream))[i] = MIXER.getSample(t);
		t += cycleTime;
	}
}

int main()
{
	
	for (int i = 0; i < MixerEvent::LOOKUP_TABLE_SAMPLES; ++i)
	{
		MixerEvent::lookupTable[i] = sin(2 * M_PI*double(i) / MixerEvent::LOOKUP_TABLE_SAMPLES);
	}

	/*
	std::ofstream fss("test.csv");
	for (auto it : MixerEvent::lookupTable) fss << it << "\n";
	fss.close();*/
	
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
	desired.freq = AUDIO_BUFFER_RATE;
	desired.format = AUDIO_F32SYS;
	desired.channels = 1;
	desired.samples = AUDIO_BUFFER_DURATION;
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
			if (events.key.keysym.scancode == SDL_SCANCODE_S) //save routine
			{				
				std::string name;
				std::string chars[2] = { "bcdfghjklmnpqrstvwxyz", "aeiou" };
				for (int i = 0; i < 10; ++i) name += chars[i % 2][rand() % chars[i % 2].size()];
				name = "output/" + name;
			
				MIXER.saveSpectrumToFile(name + ".txt");
				MIXER.saveSoundToFile(name + ".wav");				
			}

			if (events.key.keysym.scancode == SDL_SCANCODE_C)
			{
				//clear routine
				points.clear();
				MIXER.clear();
			}
			
			if (events.key.keysym.scancode == SDL_SCANCODE_B)
			{
				std::cout << "Running benchmark...\n";
				Mixer benchmarkMixer("benchmark.txt");
				double t = 0;
				int samples = 44100 * 3;
				std::vector<double> v(samples);
				adm::Timer benchmarkTimer;
				for (int i = 0; i < samples; ++i)
				{
					v[i] = benchmarkMixer.getSample(t);
					t += 1.0 / 44100;
				}
				double runTime = benchmarkTimer.getTime();

				size_t sDiff = benchmarkMixer.getEventCount();
				size_t bDiff = sDiff * samples;
				//std::cout << v[0] << "\n";
				double sampleSpeed = samples / runTime;
				double pointSpeed = bDiff / runTime;
				std::cout << "Generated " << samples << " samples in " << runTime << " sec (" << sampleSpeed << "/s, " << sampleSpeed/44100 << "x real time @ 44100 Hz" << ")\n";
				std::cout << "Sample difficulty: " << sDiff << "\nTotal: " << bDiff << " points (" << pointSpeed << "/s)\n\n";
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
			
			if (events.key.keysym.scancode == SDL_SCANCODE_ESCAPE) exit(-1);

			if (events.type == SDL_QUIT) exit(-1);
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