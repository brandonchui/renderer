#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"
#include "base.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <SDL3/SDL_log.h>

void InitializeCameraTransform(CameraTransform* camera)
{
	camera->position = glm::vec3(0.0f, 0.0f, 3.0f); //slight back
	camera->viewDir = glm::vec3(0.0f, 0.0f, -1.0f); // forward default
	camera->up = glm::vec3(0.0f, 0.0f, 1.0f);
}
glm::mat4 CalculateViewMatrix(const CameraTransform* camera)
{
	glm::mat4 view = glm::lookAt(camera->position, camera->position + camera->viewDir, camera->up);

	return view;
}
glm::mat4 CalculateProjectionMatrix(const CameraProjection* projection)
{
	return glm::perspective(glm::radians(projection->fov), 640.0f / 480.0f, projection->nearPlane,
							projection->farPlane);
}

void UpdateCameraVectors(CameraTransform* transform)
{
	transform->right = glm::normalize(cross(transform->viewDir, glm::vec3(0.0f, 1.0f, 0.0f)));
	transform->up = glm::normalize(cross(transform->right, transform->viewDir));
}
void RotateCamera(CameraTransform* transform, Mouse* mouse)
{
	//fix bug when first start up
	if (mouse->firstMouse)
	{
		mouse->prevMousePos = mouse->mousePos;
		mouse->firstMouse = false;
		return;
	}

	//deltas
	float xoffset = mouse->mousePos.x - mouse->prevMousePos.x;
	float yoffset = mouse->prevMousePos.y - mouse->mousePos.y;

	//apply sensitivity
	xoffset *= mouse->sensitivity;
	yoffset *= mouse->sensitivity;

	mouse->yaw += xoffset;
	mouse->pitch += yoffset;

	//lock
	if (mouse->pitch > 89.0f)
		mouse->pitch = 89.0f;
	if (mouse->pitch < -89.0f)
		mouse->pitch = -89.0f;

	//camera front vector TODO seperate?
	transform->viewDir.x = cos(glm::radians(mouse->yaw)) * cos(glm::radians(mouse->pitch));
	transform->viewDir.y = sin(glm::radians(mouse->pitch));
	transform->viewDir.z = sin(glm::radians(mouse->yaw)) * cos(glm::radians(mouse->pitch));
	transform->viewDir = glm::normalize(transform->viewDir);

	//save result to do calculations for next iter
	mouse->prevMousePos = mouse->mousePos;
}
void TranslateCamera(CameraTransform* transform, glm::vec3 offset)
{
	transform->position += offset;
}
