
#pragma once
#include "sim_state.h"
#include "graphics_api.h"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

struct RenderContext
{
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;
};

struct RenderCommand
{
	glm::vec3 position;
	f32 color[4];
	bool showImGuiWindow;
};

void InitializeRenderer(RenderContext& context);

void CleanupRenderer(RenderContext& context);
RenderCommand BuildRenderCommand(const SimState& sim);
void RenderFrame(const RenderContext& context, const RenderCommand& cmd, SimState& simState);
