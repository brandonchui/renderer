#include "sim.h"
#include "renderer.h"

int main()
{
	SimState simState;
	RenderContext renderContext;
	InputState inputState = {};
	TimeState time = {};

	InitializeRenderer(renderContext);

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
