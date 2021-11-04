#pragma once
#include <map>
#include <SDL/SDL.h>
#include <vector>

struct InputSnapshot
{
	std::map<Uint8, bool> mouseButtonDown;
	std::map<SDL_Scancode, bool> keyboardDown;
};

class Input
{
public:
	Input() = default;
	std::vector<SDL_Event> pollEvents();

	bool isKeyboardButtonHeld(SDL_Scancode code);
	bool isKeyboardButtonPressed(SDL_Scancode code); //will return true on the first frame after button is pressed and false otherwise
	bool isMouseButtonHeld(Uint8 button);

	InputSnapshot previousSnapshot, currentSnapshot;
	std::vector<SDL_Event> mouseEvents;
};