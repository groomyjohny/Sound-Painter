#include "HarmonicsMode.h"

HarmonicsMode::HarmonicsMode(ProgramState* state)
{
	this->state = state;
	state->MIXER = &mixer;
	state->currentMode = this;
	mixer.setHarmoicsCount(state->HARMONICS_MODE_HARMONIC_COUNT);
	mixer.setFundamentalFrequency(110);
}

void HarmonicsMode::draw()
{
	if (bars.empty()) return;

	SDL_SetRenderDrawColor(state->rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

	int n = mixer.getHarmonicCount();
	std::vector<SDL_Rect> rects;
	int i = 0;
	auto it = bars.begin();
	SDL_Rect r;
	int harmonicWidth = state->w / state->HARMONICS_MODE_HARMONIC_COUNT;

	while (it != bars.end())
	{
		r.x = it->first*harmonicWidth;
		r.y = it->second;
		r.w = harmonicWidth;
		r.h = state->h - r.y;
		rects.emplace_back(r);
		++it;
	}
	SDL_RenderDrawRects(state->rend, &rects.front(), rects.size());
	SDL_SetRenderDrawColor(state->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

void HarmonicsMode::runIndividualEventHandler(SDL_Event & event)
{
	ProgramMode::runIndividualEventHandler(event);
	if (state->input.isMouseButtonHeld(SDL_BUTTON_LEFT) && event.type == SDL_MOUSEMOTION)
	{
		this->setPoint(event.motion.x, event.motion.y);
	}
}

void HarmonicsMode::clear()
{
	this->bars.clear();
	mixer.clear();
}

void HarmonicsMode::save()
{
	std::string baseName = generateNameBase();
	std::string name = "output/harmonics/" + baseName;
	mixer.saveSpectrumToFile(name + ".txt");
	mixer.saveSoundToFile(name + ".wav");
}

void HarmonicsMode::setPoint(int x, int y)
{
	int harmonicWidth = state->w / state->HARMONICS_MODE_HARMONIC_COUNT;
	int harmonicNumber = x / harmonicWidth;

	bars[harmonicNumber] = y;
	mixer.setHarmonicDbAmplitude(harmonicNumber, -state->dbFloor*double(y) / state->h);
}
