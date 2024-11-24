
#pragma once
#include <base.h>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/mat4x4.hpp>

struct GraphicsAPI
{
	// hard coded for now
	static constexpr u8 VERTEX_COUNT = 12;
	GLuint shaderProgram = 0;
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;

	GLuint iboData[6] = {0, 1, 2, 2, 1, 3};
	GLfloat vertexPosition[VERTEX_COUNT] = {-0.8f, -0.8f, 0.0f, 0.8f, -0.8f, 0.0f,
											-0.8f, 0.8f,  0.0f, 0.8f, 0.8f,	 0.0f};
};

void InitializeGraphicsAPI(GraphicsAPI& graphics);
void CleanupGraphicsAPI(GraphicsAPI& graphics);
void DrawFrame(const GraphicsAPI& graphics, const glm::mat4& transform, const f32* color);
