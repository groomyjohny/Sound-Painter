#include "ProgramMode.h"
#include <iostream>

ProgramMode::ProgramMode(ProgramState & state)
	:state(&state)
{
}

std::vector<SDL_Event> ProgramMode::pollAndHandleEvents()
{
	auto events = state->input.pollEvents();
	for (auto& it : events) handleEvent(it);
	return events;
}

void ProgramMode::handleEvent(SDL_Event & event)
{
	auto& inp = state->input;
	if (inp.isKeyboardButtonPressed(SDL_SCANCODE_B))
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
		std::cout << "Generated " << samples << " samples in " << runTime << " sec (" << sampleSpeed << "/s, " << sampleSpeed / 44100 << "x real time @ 44100 Hz" << ")\n";
		std::cout << "Sample difficulty: " << sDiff << "\nTotal: " << bDiff << " points (" << pointSpeed << "/s)\n\n";
	}

	if (inp.isKeyboardButtonPressed(SDL_SCANCODE_S))
	{
		this->save();
	}

	if (inp.isKeyboardButtonPressed(SDL_SCANCODE_C))
	{
		this->clear();
	}

	if (inp.isKeyboardButtonPressed(SDL_SCANCODE_M))
	{
		state->PLAYBACK_IS_MUTED ^= 1; //toggle
	}

	if (inp.isKeyboardButtonPressed(SDL_SCANCODE_ESCAPE)) exit(-1);

	if (event.type == SDL_QUIT) exit(-1);
}

std::string ProgramMode::generateNameBase()
{
	std::string name;
	const std::string chars[2] = { "bcdfghjklmnpqrstvwxyz", "aeiou" };
	for (int i = 0; i < 10; ++i) name += chars[i % 2][rand() % chars[i % 2].size()]; //generate sequence of alterating vowels and consonants
	return name;
}
