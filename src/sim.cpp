#include "sim.h"

void ProcessInput(InputState& input, const SDL_Event& event)
{
	if (event.type == SDL_EVENT_QUIT)
	{
		input.quitRequested = true;
	}

	if (event.type == SDL_EVENT_KEY_DOWN)
	{
		if (event.key.scancode < InputState::MAX_KEYS)
		{
			input.keys[event.key.scancode] = true;
		}
	}
	else if (event.type == SDL_EVENT_KEY_UP)
	{
		if (event.key.scancode < InputState::MAX_KEYS)
		{
			input.keys[event.key.scancode] = false;
		}
	}
}

void UpdateSimulation(SimState& sim, const InputState& input, f32 deltaTime)
{
	if (input.quitRequested)
	{
		sim.quit = true;
	}

	if (input.IsKeyJustPressed(SDL_SCANCODE_O))
	{
		sim.showImGuiWindow = !sim.showImGuiWindow;
	}

	if (input.IsKeyPressed(SDL_SCANCODE_W))
	{
		sim.position.y += 2.0f * deltaTime;
	}
	if (input.IsKeyPressed(SDL_SCANCODE_S))
	{
		sim.position.y -= 2.0f * deltaTime;
	}
}
