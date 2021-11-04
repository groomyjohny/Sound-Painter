#pragma once
#include "ProgramMode.h"
#include "HarmonicMixer.h"
class HarmonicsMode : ProgramMode
{
public:
	HarmonicsMode(ProgramState& state);
	virtual void draw();
private:
	HarmonicMixer mixer;

	virtual void handleEvent(SDL_Event& event); //default event handlers
	virtual void clear();
	virtual void save();
	virtual void setPoint(int x, int y);

	std::map<int, int> bars;
};