
#include "model.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>

bool LoadObjModel(const char* filename, Model* outModel)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, nullptr, true))
	{
		//TODO error check
		return false;
	}

	//first mesh
	outModel->meshCount = 1;
	outModel->meshes = (Mesh*)malloc(sizeof(Mesh));
	Mesh* mesh = &outModel->meshes[0];

	//filling data from mesh
	std::vector<Vertex> vertices;
	std::vector<u32> indices;

	for (const auto& shape : shapes)
	{
		for (i32 f = 0; f < shape.mesh.indices.size() / 3; f++)
		{
			for (i32 v = 0; v < 3; v++)
			{
				tinyobj::index_t idx = shape.mesh.indices[3 * f + v];

				Vertex vertex{};

				//pos
				vertex.position = {attrib.vertices[3 * idx.vertex_index + 0],
								   attrib.vertices[3 * idx.vertex_index + 1],
								   attrib.vertices[3 * idx.vertex_index + 2]};

				//normal
				if (idx.normal_index >= 0)
				{
					vertex.normal = {attrib.normals[3 * idx.normal_index + 0],
									 attrib.normals[3 * idx.normal_index + 1],
									 attrib.normals[3 * idx.normal_index + 2]};
				}

				//uv's
				if (idx.texcoord_index >= 0)
				{
					vertex.texCoords = {attrib.texcoords[2 * idx.texcoord_index + 0],
										attrib.texcoords[2 * idx.texcoord_index + 1]};
				}

				indices.push_back(vertices.size());
				vertices.push_back(vertex);
			}
		}
	}
	mesh->indexCount = indices.size();

	///////////////////////////////////
	/////~ mesh to opengl

	//vao
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	//vbo
	glGenBuffers(1, &mesh->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),
				 GL_STATIC_DRAW);

	//ebo index buffer
	glGenBuffers(1, &mesh->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(),
				 GL_STATIC_DRAW);

	//attribs//

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  (void*)offsetof(Vertex, position));

	//normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  (void*)offsetof(Vertex, normal));

	//uv's
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
	return true;
}

void DrawModel(const Model* model, const glm::mat4& transform, const glm::mat4& viewProjection,
			   GLuint shaderProgram)
{
	glUseProgram(shaderProgram);

	//uniforms from glsl
	GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	GLint viewProjLoc = glGetUniformLocation(shaderProgram, "viewProjection");
	glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

	for (i32 i = 0; i < model->meshCount; i++)
	{
		const Mesh* mesh = &model->meshes[i];
		glBindVertexArray(mesh->vao);
		glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

void CleanupModel(Model* model)
{
	for (i32 i = 0; i < model->meshCount; i++)
	{
		Mesh* mesh = &model->meshes[i];
		glDeleteBuffers(1, &mesh->vbo);
		glDeleteBuffers(1, &mesh->ebo);
		glDeleteVertexArrays(1, &mesh->vao);
	}
	free(model->meshes);
	model->meshes = nullptr;
	model->meshCount = 0;
}
