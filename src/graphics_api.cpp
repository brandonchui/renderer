
// graphics_api.cpp
#include "graphics_api.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

//might move this somewhere else but trying to keep tidy for now
namespace
{
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

	GLuint CompileShader(GLuint type, const std::string& source)
	{
		GLuint shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

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
} // namespace

void InitializeGraphicsAPI(GraphicsAPI& graphics)
{
	graphics.shaderProgram = CreateShaderProgram();

	//vertex array object
	glGenVertexArrays(1, &graphics.vao);
	glBindVertexArray(graphics.vao);

	//vertex array buffer
	glGenBuffers(1, &graphics.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(graphics.vertexPosition), graphics.vertexPosition,
				 GL_STATIC_DRAW);

	//index buffer
	glGenBuffers(1, &graphics.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphics.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(graphics.iboData), graphics.iboData,
				 GL_STATIC_DRAW);

	//attrib
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//unbind
	glBindVertexArray(0);
}

void CleanupGraphicsAPI(GraphicsAPI& graphics)
{
	if (graphics.shaderProgram)
	{
		glDeleteProgram(graphics.shaderProgram);
		graphics.shaderProgram = 0;
	}

	if (graphics.vbo)
	{
		glDeleteBuffers(1, &graphics.vbo);
		graphics.vbo = 0;
	}

	if (graphics.ibo)
	{
		glDeleteBuffers(1, &graphics.ibo);
		graphics.ibo = 0;
	}

	if (graphics.vao)
	{
		glDeleteVertexArrays(1, &graphics.vao);
		graphics.vao = 0;
	}
}

void DrawFrame(const GraphicsAPI& graphics, const glm::mat4& transform, const float* color)
{
	glUseProgram(graphics.shaderProgram);

	GLint colorLoc = glGetUniformLocation(graphics.shaderProgram, "triangleColor");
	glUniform4fv(colorLoc, 1, color);

	GLint transformLoc = glGetUniformLocation(graphics.shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(graphics.vao);
	glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}
