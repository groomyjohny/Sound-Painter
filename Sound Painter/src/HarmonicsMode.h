#pragma once
#include "ProgramMode.h"
#include "HarmonicMixer.h"
class HarmonicsMode : public ProgramMode
{
public:
	HarmonicsMode(ProgramState* state);
	virtual void draw();
protected:
	HarmonicMixer mixer;

	virtual void runIndividualEventHandler(SDL_Event& event); //default event handlers
	virtual void runOncePerFrameHandlers(std::vector<SDL_Event>& events);
	virtual void clear();
	virtual void save();
	virtual void setPoint(int x, int y);

	virtual void setRandomIntensities();
	virtual void changeHarmonicsCount(int amount = 1);

	std::map<int, int> bars;
	int harmonicsCount;
};