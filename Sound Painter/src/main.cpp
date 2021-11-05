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

ProgramState* realState = new ProgramState();
ProgramState& state = *realState;

void fill_audio(void *udata, Uint8 *stream, int len)
{
	if (state.PLAYBACK_IS_MUTED || !state.MIXER)
	{
		memset(stream, 0, len); //zero out the buffer to avoid sound looping
		return;
	}

	double t = state.TIMER.getTime();
	double cycleTime = 1.0 / state.AUDIO_BUFFER_RATE;
	auto m = state.MIXER->getMutex();
	std::lock_guard g(*m);

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
	SDL_AudioDeviceID audioDeviceId = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

	state.TIMER.resume();
	std::vector<float> audioBuffer;
	SDL_PauseAudioDevice(audioDeviceId, 0); 

	state.currentMode = new DrawMode(realState);

	while (true)
	{
		state.currentMode->pollAndHandleEvents();
		state.currentMode->draw();
		SDL_RenderPresent(state.rend);
		
		SDL_RenderClear(state.rend);
		SDL_Delay(16);
	}

	std::cin.ignore(999999);
	std::cin.get();
	return 0;
}