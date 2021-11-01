#pragma once
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <array>
#include "MixerEvent.h"

class Mixer
{
public:
	Mixer() = default;
	Mixer(std::string fileName);

	double getSample(double t);
	double getDuration();
	size_t getEventCount();
	
	std::vector<float> getSamplesFromUntil(double tBegin, double tEnd, SDL_AudioSpec spec);
	void saveSoundToFile(std::string fileName);
	void saveSpectrumToFile(std::string fileName);
	void addEvent(const MixerEvent& evt);
	void clear();
private:
	std::vector<MixerEvent> events;
	static constexpr double FILE_EXPORT_DURATION = 20.0;
	
};