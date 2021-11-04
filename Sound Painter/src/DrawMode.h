#pragma once
#include "ProgramMode.h"
class DrawMode : public ProgramMode
{
public:
	DrawMode(ProgramState* state);
	virtual void draw();
protected:
	std::vector<SDL_Point> points;
	Mixer mixer;

	virtual void handleEvent(SDL_Event& event); //default event handlers
	virtual void clear();
	virtual void save();
	virtual void addPoint(int x, int y);
};