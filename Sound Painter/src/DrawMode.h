#pragma once
#include "ProgramMode.h"
class DrawMode : public ProgramMode
{
public:
	DrawMode(ProgramState* state);
	virtual void draw();
	virtual Mixer* getMixer();
protected:
	std::vector<SDL_Point> points;
	Mixer mixer;

	virtual void runIndividualEventHandler(SDL_Event& event); //default event handlers
	virtual void clear();
	virtual void save();
	virtual void addPoint(int x, int y);
};