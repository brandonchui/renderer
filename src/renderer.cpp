
// renderer.cpp
#include "renderer.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

static GraphicsAPI gGraphics;

//inits the context container and returns SDL window and GL context
void InitializeRenderer(RenderContext& context)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL could not initialize");
		return;
	}

	context.window = SDL_CreateWindow("Renderer", 640, 480,
									  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!context.window)
	{
		SDL_Log("Window could not be created");
		return;
	}

	context.glContext = SDL_GL_CreateContext(context.window);
	if (!context.glContext)
	{
		SDL_Log("OpenGL context could not be created");
		return;
	}

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// debug gpu info on client
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	// creates vertex array,buffers, binds
	InitializeGraphicsAPI(gGraphics);

	// initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForOpenGL(context.window, context.glContext);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void CleanupRenderer(RenderContext& context)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	CleanupGraphicsAPI(gGraphics);

	if (context.glContext)
	{
		SDL_GL_DestroyContext(context.glContext);
	}
	if (context.window)
	{
		SDL_DestroyWindow(context.window);
	}
	SDL_Quit();
}

RenderCommand BuildRenderCommand(const SimState& sim)
{
	RenderCommand cmd = {};
	cmd.position = sim.position;
	memcpy(cmd.color, sim.color, sizeof(cmd.color));
	cmd.showImGuiWindow = sim.showImGuiWindow;
	return cmd;
}

static void PrepareDraw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, 640, 480);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

static void DrawImGui(const RenderCommand& cmd, const RenderContext& context, SimState& sim)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	if (cmd.showImGuiWindow)
	{
		ImGui::Begin("Triangle Controls");
		ImGui::ColorEdit4("Color", sim.color);
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void RenderFrame(const RenderContext& context, const RenderCommand& cmd, SimState& sim)
{
	PrepareDraw();

	//testing
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), cmd.position);

	//draw
	DrawFrame(gGraphics, transform, cmd.color);
	DrawImGui(cmd, context, sim);

	//present
	SDL_GL_SwapWindow(context.window);
}
