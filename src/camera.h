#pragma once
#include "base.h"
#include "glm/glm.hpp"

struct CameraState
{
	glm::vec3 position;
	glm::vec3 viewDir;
	glm::vec3 up;

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

void InitializeCameraState(CameraState* camera);
glm::mat4 CalculateViewMatrix(const CameraState* camera);
glm::mat4 CalculateProjectionMatrix(const CameraState* camera);

//move
void Move(const CameraState* camera, f32 speed, Direction dir);
