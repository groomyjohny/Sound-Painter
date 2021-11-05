#include "WaveformMode.h"
#include "LineCurve.h"

WaveformMode::WaveformMode(ProgramState * state)
{
	this->state = state;
	state->currentMode = this;
	state->MIXER = &mixer;
}

void WaveformMode::draw()
{
	SDL_SetRenderDrawColor(state->rend, 0, 0, 0, SDL_ALPHA_OPAQUE);

	for (auto& it : mixer.getEvents())
	{
		auto p = std::dynamic_pointer_cast<LineCurve>(it);

		std::vector<SDL_Point> points;
		if (p->getSegments().empty()) continue;
		for (auto& it2 : p->getSegments())
		{
			int x = it2.phaseEnd*state->w;
			int y = (1-it2.ampEnd)*state->h;
			points.push_back({ x,y });
		}
		if (!points.empty()) SDL_RenderDrawLines(state->rend, &points.front(), points.size());
	}
	SDL_SetRenderDrawColor(state->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

void WaveformMode::runOncePerFrameHandlers(std::vector<SDL_Event>& events)
{
	ProgramMode::runOncePerFrameHandlers(events);
	auto inp = state->input;
	if (inp.isMouseButtonPressed(SDL_BUTTON_LEFT)) mixer.addEvent(std::make_shared<LineCurve>());
}

void WaveformMode::runIndividualEventHandler(SDL_Event & event)
{
	ProgramMode::runIndividualEventHandler(event);
	auto inp = state->input;
	if (event.type == SDL_MOUSEMOTION && inp.isMouseButtonHeld(SDL_BUTTON_LEFT)) addPoint(event.motion.x, event.motion.y);
}

void WaveformMode::clear()
{
	mixer.clear();
}

void WaveformMode::save()
{
	std::string baseName = generateNameBase();
	std::string name = "output/waveforms/" + baseName + ".wav";

	for (auto& it : mixer.getEvents()) it->setFrequency(110);
	mixer.saveSoundToFile(name);
}

void WaveformMode::addPoint(int x, int y)
{
	auto p = std::dynamic_pointer_cast<LineCurve>(mixer.getEvents().back());
	
	double phase = double(x)/state->w;
	//double db = -state->dbFloor*double(y) / state->h;
	//double amp = pow(10, db / 10);
	double amp = 1 - double(y) / state->h;
	p->append(phase, amp);
}
