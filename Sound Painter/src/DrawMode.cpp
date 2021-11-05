#include "DrawMode.h"
#include <cmath>

DrawMode::DrawMode(ProgramState * state)
{
	this->state = state;
	state->MIXER = &mixer;
	state->currentMode = this;
}

void DrawMode::draw()
{
	SDL_SetRenderDrawColor(state->rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
	if (!points.empty()) SDL_RenderDrawPoints(state->rend, &points.front(), points.size());
	SDL_SetRenderDrawColor(state->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

void DrawMode::runIndividualEventHandler(SDL_Event & event)
{
	ProgramMode::runIndividualEventHandler(event);
	
	if (event.type == SDL_MOUSEMOTION)
	{
		if (state->input.isMouseButtonHeld(SDL_BUTTON_LEFT))
		{
			addPoint(event.motion.x, event.motion.y);
		}
	}
}

void DrawMode::clear()
{
	points.clear();
	mixer.clear();
}

void DrawMode::save()
{
	std::string name = "output/" + generateNameBase();
	mixer.saveSpectrumToFile(name + ".txt");
	mixer.saveSoundToFile(name + ".wav");	
}

void DrawMode::addPoint(int x, int y)
{
	points.emplace_back(SDL_Point{ x, y });
	double frq = exp(state->lnLow + double(x) / state->w * (state->lnHigh - state->lnLow));
	double db = -state->dbFloor * double(y) / state->h;
	mixer.addEvent(MixerEvent(frq, pow(10, db / 10)));
}
