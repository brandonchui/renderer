
#pragma once
#include "base.h"
#include <glm/vec3.hpp>
#include "camera.h"
#include "model.h"

struct SimState
{
	bool quit = false;
	bool showImGuiWindow = true;

	glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f};
	f32 rotateY = 0.0f;
	f32 color[4] = {1.0f, 0.5f, 0.0f, 1.0f};

	//model test
	Model model;

	//camera
	CameraTransform cameraTransform;
	CameraProjection cameraProjection;
	CameraMatrices cameraMatrices;
	Mouse mouse;
};
