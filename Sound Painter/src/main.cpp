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
#include <memory>
#include "HarmonicMixer.h"
#include "ProgramState.h"
#include "ProgramMode.h"
#include "DrawMode.h"

#pragma comment(lib,"SDL2.lib")
#undef main

ProgramState state;
ProgramMode* mode;

void fill_audio(void *udata, Uint8 *stream, int len)
{
	if (state.PLAYBACK_IS_MUTED || !state.MIXER)
	{
		memset(stream, 0, len); //zero out the buffer to avoid sound looping
		return;
	}

	double t = state.TIMER.getTime();
	double cycleTime = 1.0 / state.AUDIO_BUFFER_RATE;
	for (int i = 0; i < len/sizeof(float); ++i)
	{
		((float*)(stream))[i] = state.MIXER ? state.MIXER->getSample(t) : 0;
		t += cycleTime;
	}
}

int main()
{
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);

	state.w = 1280;
	state.h = 720;
	state.windowX = SDL_WINDOWPOS_CENTERED;
	state.windowY = SDL_WINDOWPOS_CENTERED;

	SDL_CreateWindowAndRenderer(state.w, state.h, 0, &state.wnd, &state.rend);

	int error = SDL_GL_SetSwapInterval(1);
	std::cout << "Failed to set vsync: " << SDL_GetError() << "\n";

	SDL_AudioSpec desired,obtained;
	desired.freq = state.AUDIO_BUFFER_RATE;
	desired.format = AUDIO_F32SYS;
	desired.channels = 1;
	desired.samples = state.AUDIO_BUFFER_SAMPLES;
	desired.userdata = 0;
	desired.callback = fill_audio;

	//int openAudioError = SDL_OpenAudio(&desired, &obtained);
	SDL_AudioDeviceID audioDeviceId = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	//SDL_QueueAudio(dev, buf.data(), sizeof(float) * buf.size());

	state.TIMER.resume();
	std::vector<float> audioBuffer;
	SDL_PauseAudioDevice(audioDeviceId, 0); 

	mode = new DrawMode(state);
	state.currentMode = mode;

	while (true)
	{
		SDL_Event events;
		mode->pollAndHandleEvents();
			//input processing goes here
			

		//SDL_ClearQueuedAudio(audioDeviceId);
		//double currT = timer.getTime();
		//audioBuffer = MIXER.getSamplesFromUntil(currT, currT + AUDIO_BUFFER_DURATION, obtained);
		//SDL_QueueAudio(audioDeviceId, (void*)&audioBuffer.front(), sizeof(float)*audioBuffer.size());
		
		/*points.clear();
		for (auto& it : pointMap) points.emplace_back(SDL_Point{ it.first,it.second });*/

			/*if (!HARMONICS_MODE && !points.empty()) SDL_RenderDrawPoints(rend, &points.front(), points.size());
			if (HARMONICS_MODE && !harmonicsModePointMap.empty())
			{
				auto p = std::dynamic_pointer_cast<HarmonicMixer>(MIXER);
				int n = p->getHarmonicCount();
				std::vector<SDL_Rect> rects;
				int i = 0;
				auto it = harmonicsModePointMap.begin();
				SDL_Rect r;
				int harmonicWidth = w / HARMONICS_MODE_HARMONIC_COUNT;

				while (it != harmonicsModePointMap.end())
				{
					r.x = it->first*harmonicWidth;
					r.y = it->second;
					r.w = harmonicWidth;
					r.h = h - r.y;
					rects.emplace_back(r);
					++it;
				}
				SDL_RenderDrawRects(rend, &rects.front(), rects.size());
			}*/


		//frame rendering goes here
		mode->draw();
		SDL_RenderPresent(state.rend);
		
		SDL_RenderClear(state.rend);
		SDL_Delay(16);
	}

	std::cin.ignore(999999);
	std::cin.get();
	return 0;
}