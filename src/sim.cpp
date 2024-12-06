#include "sim.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_events.h"
#include "base.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_mouse.h>

void ProcessInput(InputState& input, const SDL_Event& event)
{
	if (event.type == SDL_EVENT_QUIT)
	{
		input.quitRequested = true;
	}

	if (event.type == SDL_EVENT_WINDOW_RESIZED)
	{
		//
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
	else if (event.type == SDL_EVENT_MOUSE_MOTION)
	{
		input.mouseDelta.x = static_cast<f32>(event.motion.xrel);

		//invert y
		input.mouseDelta.y = -static_cast<f32>(event.motion.yrel);
	}
	else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (event.button.button == SDL_BUTTON_MIDDLE)
		{
			input.middleMouseDown = true;
			input.isPanning = true;
		}
	}
	else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		if (event.button.button == SDL_BUTTON_MIDDLE)
		{
			input.middleMouseDown = false;
			input.isPanning = false;
		}
	}
}

void UpdateSimulation(SimState& sim, InputState& input, f32 deltaTime)
{
	if (input.quitRequested)
	{
		sim.quit = true;
	}

	if (input.IsKeyJustPressed(SDL_SCANCODE_O))
	{
		sim.showImGuiWindow = !sim.showImGuiWindow;
	}
	if (input.IsKeyJustPressed(SDL_SCANCODE_ESCAPE))
	{
		sim.quit = true;
	}

	//camera movements
	glm::vec3 moveOffset = {0.0f, 0.0f, 0.0f};
	f32 speed = 5.0f; //temp
	if (input.IsKeyPressed(SDL_SCANCODE_W))
	{
		moveOffset += sim.cameraTransform.viewDir * speed * deltaTime;
	}
	if (input.IsKeyPressed(SDL_SCANCODE_S))
	{
		moveOffset -= sim.cameraTransform.viewDir * speed * deltaTime;
	}
	if (input.IsKeyPressed(SDL_SCANCODE_A))
	{
		moveOffset -= sim.cameraTransform.right * speed * deltaTime;
	}
	if (input.IsKeyPressed(SDL_SCANCODE_D))
	{
		moveOffset += sim.cameraTransform.right * speed * deltaTime;
	}

	//update mouse
	if (input.isPanning)
	{
		moveOffset += sim.cameraTransform.right * input.mouseDelta.x * deltaTime * speed;
		moveOffset += sim.cameraTransform.up * input.mouseDelta.y * deltaTime * speed;
	}
	else
	{
		sim.mouse.yaw += input.mouseDelta.x * sim.mouse.sensitivity;
		sim.mouse.pitch += input.mouseDelta.y * sim.mouse.sensitivity;
	}
	//reset mouse
	input.mouseDelta = glm::vec2(0.0f, 0.0f);

	//update cameras
	TranslateCamera(&sim.cameraTransform, moveOffset);
	RotateCamera(&sim.cameraTransform, &sim.mouse);
	UpdateCameraVectors(&sim.cameraTransform);

	//update matrices
	sim.cameraMatrices.view = CalculateViewMatrix(&sim.cameraTransform);
	sim.cameraMatrices.projection = CalculateProjectionMatrix(&sim.cameraProjection);
}

void SetCameraCursorLock(SDL_Window* win, bool locked)
{
	//toggles focus camera
	SDL_SetWindowRelativeMouseMode(win, locked);
}
