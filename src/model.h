#pragma once
#include "base.h"
#include "graphics_api.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "tiny_obj_loader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Mesh
{
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	u32 indexCount;
};

struct Model
{
	Mesh* meshes;
	u32 meshCount;
};

bool LoadObjModel(const char* filename, Model* outModel);
void DrawModel(const Model* model, const glm::mat4& transform, const glm::mat4& viewProjection,
			   GLuint shaderProgram);
void CleanupModel(Model* model);
