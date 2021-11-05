#pragma once
#include "ProgramState.h"
#include <SDL/SDL.h>
#include <adm/Timer.h>
#include <memory>
#include "Input.h"

class ProgramMode;

struct ProgramState
{
	ProgramState() = default;
	int w, h, windowX, windowY;
	SDL_Window* wnd;
	SDL_Renderer* rend;

	double lowFrq = 20;
	double highFrq = 20e3;
	double dbFloor = 96;

	double lnLow = log(lowFrq);
	double lnHigh = log(highFrq);

	double AUDIO_BUFFER_DURATION_SECONDS = 0.1;
	const int AUDIO_BUFFER_SAMPLES = 512;
	const int AUDIO_BUFFER_RATE = 44100;
	adm::Timer TIMER = adm::Timer(false);
	std::shared_ptr<ProgramMode> currentMode;
	std::shared_ptr<ProgramMode> pendingMode;

	bool PLAYBACK_IS_MUTED = false;
	bool HARMONICS_MODE = false; //DO NOT CHANGE TO TRUE, it needs other stuff to work properly! Look into H press handler
	const int HARMONICS_MODE_HARMONIC_COUNT = 64;

	Input input;
};