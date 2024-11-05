#pragma once
#include <glad/glad.h>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>

struct GraphicsState
{
	static constexpr uint8_t VERTEX_COUNT = 9;

	float color[4] = {1.0f, 0.5f, 0.0f, 1.0f};

	GLuint graphicsPipelineShaderProgram = 0;
	GLuint vao = 0;
	GLuint vbo = 0;

	GLfloat vertexPosition[VERTEX_COUNT] = {-0.8f, -0.8f, 0.0f, 0.8f, -0.8f,
											0.0f,  0.0f,  0.8f, 0.0f};
};

//////////////////////////////////////////////////
//~ helper function to load shaders
std::string LoadShaderSource(const std::string& filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open shader file: " + filepath);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

//////////////////////////////////////////////////
//~ shader compiling
GLuint CompileShader(GLuint type, const std::string& source)
{
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	// error check
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Shader compilation failed:\n" << infoLog << std::endl;
	}

	return shader;
}

//////////////////////////////////////////////////
//~ attaching and linking shaders
GLuint CreateShaderProgram()
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, LoadShaderSource("shaders/vs.glsl"));
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, LoadShaderSource("shaders/fs.glsl"));

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

//////////////////////////////////////////////////
//~ init graphics
void InitializeGraphics(GraphicsState& state)
{
	state.graphicsPipelineShaderProgram = CreateShaderProgram();

	glGenVertexArrays(1, &state.vao);
	glGenBuffers(1, &state.vbo);

	glBindVertexArray(state.vao);

	glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(state.vertexPosition), state.vertexPosition,
				 GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}

//////////////////////////////////////////////////
//~ clean up graphics
void CleanupGraphics(GraphicsState& state)
{
	glDeleteProgram(state.graphicsPipelineShaderProgram);
	glDeleteBuffers(1, &state.vbo);
	glDeleteVertexArrays(1, &state.vao);
}
