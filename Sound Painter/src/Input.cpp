#include "Input.h"

std::vector<SDL_Event> Input::pollEvents()
{
	std::vector<SDL_Event> ret;
	previousSnapshot = currentSnapshot;
	mouseEvents.clear();
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		ret.push_back(event);

		if (event.type == SDL_KEYDOWN) currentSnapshot.keyboardDown[event.key.keysym.scancode] = true;
		if (event.type == SDL_KEYUP) currentSnapshot.keyboardDown[event.key.keysym.scancode] = false;
		
		if (event.type == SDL_MOUSEBUTTONDOWN) currentSnapshot.mouseButtonDown[event.button.button] = true;
		if (event.type == SDL_MOUSEBUTTONUP) currentSnapshot.mouseButtonDown[event.button.button] = false;

		if (event.type == SDL_MOUSEMOTION) mouseEvents.push_back(event);
	}
	return ret;
}

bool Input::isKeyboardButtonHeld(SDL_Scancode code)
{
	return currentSnapshot.keyboardDown[code];
}

bool Input::isKeyboardButtonPressed(SDL_Scancode code)
{
	return !previousSnapshot.keyboardDown[code] && currentSnapshot.keyboardDown[code];
}

bool Input::isMouseButtonHeld(Uint8 button)
{
	return currentSnapshot.mouseButtonDown[button];
}

bool Input::isMouseButtonPressed(Uint8 button)
{
	return !previousSnapshot.mouseButtonDown[button] && currentSnapshot.mouseButtonDown[button];
}
