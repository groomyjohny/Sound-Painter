#pragma once
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <array>
#include "MixerEvent.h"
#include <filesystem>
class Mixer
{
public:
	Mixer() = default;
	Mixer(std::string fileName);

	virtual double getSample(double t);
	virtual double getDuration();
	virtual size_t getEventCount();
	virtual std::vector<float> getSamplesFromUntil(double tBegin, double tEnd, SDL_AudioSpec spec);

	virtual void saveSoundToFile(std::string fileName);
	virtual void saveSoundToFile(std::filesystem::path fileName);
	virtual void saveSpectrumToFile(std::string fileName);
	virtual void addEvent(const MixerEvent& evt);
	virtual void clear();
protected:
	std::vector<MixerEvent> events;
	static constexpr double FILE_EXPORT_DURATION = 20.0;	
};