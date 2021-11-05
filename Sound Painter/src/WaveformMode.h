#pragma once
#include "ProgramMode.h"
class WaveformMode : public ProgramMode
{
public:
	WaveformMode(ProgramState* state);
	virtual void draw();
	virtual Mixer* getMixer();
protected:
	virtual void runOncePerFrameHandlers(std::vector<SDL_Event>& events);
	virtual void runIndividualEventHandler(SDL_Event& event); //default event handlers
	virtual void clear();
	virtual void save();
	virtual void addPoint(int x, int y);

	Mixer mixer;
};