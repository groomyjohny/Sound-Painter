#pragma once
#include <SDL/SDL.h>
#include <adm/Timer.h>
#include <memory>
#include "Mixer.h"
#include "ProgramState.h"

class ProgramMode
{
public:
	ProgramMode() = default;
	ProgramMode(ProgramState* state);
	virtual std::vector<SDL_Event> pollAndHandleEvents(); //basic mode switching
	virtual void draw() = 0;
	virtual Mixer* getMixer();
protected:
	virtual void runOncePerFrameHandlers(std::vector<SDL_Event>& events);
	virtual void runIndividualEventHandler(SDL_Event& event); //default event handlers
	virtual void clear() = 0;
	virtual void save() = 0;
	virtual std::string generateNameBase();
	virtual void runBenchmark();

	ProgramState* state;
};