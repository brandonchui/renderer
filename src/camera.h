#pragma once
#include "base.h"
#include "glm/glm.hpp"

struct CameraTransform
{
	glm::vec3 position; //eye
	glm::vec3 viewDir;
	glm::vec3 up;
	glm::vec3 right = glm::cross(viewDir, up); //cross product
};

struct CameraProjection
{
	f32 fov;
	f32 aspect;
	f32 nearPlane;
	f32 farPlane;
};

struct CameraMatrices
{
	glm::mat4 view;
	glm::mat4 projection;
};

struct Mouse
{
	glm::vec2 mousePos;
	glm::vec2 prevMousePos;
	f32 sensitivity = 0.7f;
	bool firstMouse = true;
	f32 yaw = -90.0f;
	f32 pitch = 0.0f;
};

void InitializeCameraTransform(CameraTransform* transform);

glm::mat4 CalculateViewMatrix(const CameraTransform* transform);
glm::mat4 CalculateProjectionMatrix(const CameraProjection* projection);

//move
void UpdateCameraVectors(
	CameraTransform*
		transform); // prob don't need this, already doing cross product in camera struct
void RotateCamera(CameraTransform* transform, Mouse* mouse);
void TranslateCamera(CameraTransform* transform, glm::vec3 offset);
