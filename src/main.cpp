#include "sim.h"
#include "renderer.h"
#include <SDL3/SDL_mouse.h>

#include <filesystem>
#include <iostream>

int main()
{
	SimState simState;

	RenderContext renderContext;
	InputState inputState = {};
	TimeState time = {};

	InitializeRenderer(renderContext);

	//create camera
	simState.cameraProjection = {.fov = 45.0f,
								 .aspect = 640.0f / 480.0f,
								 .nearPlane = 0.1f,
								 .farPlane = 100.0f};

	simState.cameraTransform = {.position = glm::vec3(0.0f, 1.5f, 9.0f), //initial camera pos
								.viewDir = glm::vec3(0.0f, 0.0f, -1.0f),
								.up = glm::vec3(0.0f, 1.0f, 0.0f)};

	//model test
	const std::filesystem::path currentPath = std::filesystem::current_path();
	std::filesystem::path modelPath = currentPath / "util" / "teapot.obj";
	if (!std::filesystem::exists(modelPath))
	{
		std::cout << "Error finding model: " << modelPath << std::endl;
		return -1;
	}
	else
	{
		std::cout << modelPath << " exists!" << std::endl;
	}
	LoadObjModel(modelPath.string().c_str(), &simState.model);

	//main loop
	while (!simState.quit)
	{
		inputState.UpdatePrevKeys(); //for leading edge
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			ProcessInput(inputState, event);
		}

		f32 dt = GetDeltaTime(time);
		UpdateSimulation(simState, inputState, dt);

		RenderCommand renderCmd = BuildRenderCommand(simState);
		RenderFrame(renderContext, renderCmd, simState);
	}

	CleanupRenderer(renderContext);
	return 0;
}
