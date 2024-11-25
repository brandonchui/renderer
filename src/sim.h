
#pragma once
#include "sim_state.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include "camera.h"

struct InputState
{
	static constexpr i16 MAX_KEYS = 512;

	//current mouse pos
	glm::vec2 mouseDelta = {};
	bool middleMouseDown = false;
	bool isPanning = false;

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

struct InputCamera
{
	glm::vec2 mouseDelta;
	bool isMouseLocked;
};

void ProcessInput(InputState& input, const SDL_Event& event);
void UpdateSimulation(SimState& sim, InputState& input, f32 deltaTime);
// void UpdateCameraFromInput(CameraState* camera, const InputCamera* input, f32 deltaTime);
// void UpdateCameraMatrices(const CameraState* camera, CameraMatrices* matrices);
