#include <iostream>
#include "SDL3/SDL_log.h"
#include "graphics.h"

//gl
#include <glad/glad.h>
//sdl
#include <SDL3/SDL.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_video.h"
//imgui
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
//glm
#include <gtc/type_ptr.hpp>

struct AppState
{
	bool quit = false;
	bool showImGuiWindow = true;
};

struct RenderContext
{
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;
	GraphicsState graphicsState;
};

//////////////////////////////////////////////////
//~ quit request status from user
struct InputState
{
	bool quitRequested = false;
};

//////////////////////////////////////////////////
//~ Init SDL, openGL, ImGui
void InitializeRenderContext(RenderContext& renderContext)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL could not initialize");
		return;
	}

	renderContext.window = SDL_CreateWindow("Application", 640, 480, SDL_WINDOW_OPENGL);
	if (!renderContext.window)
	{
		SDL_Log("Window could not be created");
		return;
	}

	renderContext.glContext = SDL_GL_CreateContext(renderContext.window);
	if (!renderContext.glContext)
	{
		SDL_Log("OpenGL context could not be created");
		return;
	}

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
	}

	//graphics details
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	// init graphics
	InitializeGraphics(renderContext.graphicsState);

	// [IMGUI] init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForOpenGL(renderContext.window, renderContext.glContext);
	ImGui_ImplOpenGL3_Init("#version 410");
}

//////////////////////////////////////////////////
//~ handles keyboards
void HandleInput(InputState& inputState, AppState& appState, RenderContext& renderContext)
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			inputState.quitRequested = true;
		}
		else if (event.type == SDL_EVENT_KEY_DOWN)
		{
			//press `o` to toggle imgui window
			if (event.key.scancode == SDL_SCANCODE_O)
			{
				appState.showImGuiWindow = !appState.showImGuiWindow;
			}
			else if (event.key.scancode == SDL_SCANCODE_W)
			{
				renderContext.graphicsState.position.y += 0.01f;
			}
			else if (event.key.scancode == SDL_SCANCODE_S)
			{
				renderContext.graphicsState.position.y -= 0.01f;
			}
			renderContext.graphicsState.translate =
				glm::translate(glm::mat4(1.0f), renderContext.graphicsState.position);
		}

		ImGui_ImplSDL3_ProcessEvent(&event);
	}
}

//////////////////////////////////////////////////
//~ draw function specifically for ImGui
void DrawImGui(AppState& appState, RenderContext& renderContext)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	if (appState.showImGuiWindow)
	{
		ImGui::Begin("Triangle Controls");
		ImGui::ColorEdit4("Color", renderContext.graphicsState.color);
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//////////////////////////////////////////////////
//~ handle buffer swapping
void SwapBuffers(RenderContext& renderContext)
{
	SDL_GL_SwapWindow(renderContext.window);
}

//////////////////////////////////////////////////
//~ setting up opengl before actual rendering
void PreDraw(const GraphicsState& graphicsState)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, 640, 480);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(graphicsState.graphicsPipelineShaderProgram);
}

//////////////////////////////////////////////////
//~ begin to draw opengl
void DrawGL(const GraphicsState& graphicsState)
{
	//color
	GLint colorLoc = glGetUniformLocation(graphicsState.graphicsPipelineShaderProgram,
										  "triangleColor");
	glUniform4fv(colorLoc, 1, graphicsState.color);

	//translate
	GLint transformLoc = glGetUniformLocation(graphicsState.graphicsPipelineShaderProgram,
											  "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(graphicsState.translate));

	glBindVertexArray(graphicsState.vao);
	glBindBuffer(GL_ARRAY_BUFFER, graphicsState.vbo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

//////////////////////////////////////////////////
//~ draw function for ImGui
void Draw(RenderContext& renderContext, AppState& appState)
{
	PreDraw(renderContext.graphicsState);
	DrawGL(renderContext.graphicsState);
	DrawImGui(appState, renderContext);
	SwapBuffers(renderContext);
}

//////////////////////////////////////////////////
//~ clean up resources
void Cleanup(RenderContext& renderContext)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	CleanupGraphics(renderContext.graphicsState);

	SDL_GL_DestroyContext(renderContext.glContext);
	SDL_DestroyWindow(renderContext.window);
	SDL_Quit();
}

//////////////////////////////////////////////////
//~ main entry point
void MainLoop(AppState& appState, RenderContext& renderContext, InputState& inputState)
{
	while (!appState.quit)
	{
		HandleInput(inputState, appState, renderContext);
		if (inputState.quitRequested)
		{
			appState.quit = true;
		}
		Draw(renderContext, appState);
	}
}

int main()
{
	AppState appState;
	RenderContext renderContext;
	InputState inputState;

	InitializeRenderContext(renderContext);
	MainLoop(appState, renderContext, inputState);
	Cleanup(renderContext);

	return 0;
}
