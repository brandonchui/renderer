
#pragma once
#include "sim_state.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>

struct InputState
{
	static constexpr i16 MAX_KEYS = 512;

	//current frame key
	bool keys[MAX_KEYS] = {};

	//leading edge detect
	bool prevKeys[MAX_KEYS] = {};

	//commmons
	bool quitRequested = false;

	bool IsKeyPressed(SDL_Scancode key) const { return keys[key]; }

	//for leading edge use this
	bool IsKeyJustPressed(SDL_Scancode key) const { return keys[key] && !prevKeys[key]; }

	void UpdatePrevKeys() { memcpy(prevKeys, keys, sizeof(keys)); }
};

void ProcessInput(InputState& input, const SDL_Event& event);
void UpdateSimulation(SimState& sim, const InputState& input, f32 deltaTime);
