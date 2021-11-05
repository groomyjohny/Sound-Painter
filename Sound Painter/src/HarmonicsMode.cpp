#include "HarmonicsMode.h"

HarmonicsMode::HarmonicsMode(ProgramState* state)
{
	this->state = state;
	harmonicsCount = state->HARMONICS_MODE_HARMONIC_COUNT;
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
	double harmonicWidth = double(state->w) / harmonicsCount;

	while (it != bars.end())
	{
		r.x = it->first*harmonicWidth;
		r.w = harmonicWidth;
		r.y = it->second;
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

void HarmonicsMode::runOncePerFrameHandlers(std::vector<SDL_Event>& events)
{
	ProgramMode::runOncePerFrameHandlers(events);
	
	auto inp = state->input;
	if (inp.isKeyboardButtonPressed(SDL_SCANCODE_R)) this->setRandomIntensities();
	if (inp.isKeyboardButtonPressed(SDL_SCANCODE_MINUS)) this->changeHarmonicsCount(-1);
	if (inp.isKeyboardButtonPressed(SDL_SCANCODE_EQUALS)) this->changeHarmonicsCount(1);
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
	double harmonicWidth = double(state->w) / harmonicsCount;
	int harmonicNumber = x / harmonicWidth;

	bars[harmonicNumber] = y;
	mixer.setHarmonicDbAmplitude(harmonicNumber, -state->dbFloor*double(y) / state->h);
}

void HarmonicsMode::setRandomIntensities()
{
	for (int i = 0; i < 65536; ++i)
	{
		int x = rand() % state->w;
		int y = rand() % state->h;
		setPoint(x, y);
	}
}

void HarmonicsMode::changeHarmonicsCount(int amount)
{
	int futureHarmonicsCount = harmonicsCount + amount;
	if (futureHarmonicsCount < 1) return;

	harmonicsCount = futureHarmonicsCount;
	mixer.setHarmoicsCount(futureHarmonicsCount);
}
