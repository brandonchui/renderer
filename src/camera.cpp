#include "Camera.h"
#include "base.h"

void InitializeCameraState(CameraState* camera)
{
	//TODO
}
glm::mat4 CalculateViewMatrix(const CameraState* camera)
{
	glm::mat4 view;
	//TODO

	return view;
}
glm::mat4 CalculateProjectionMatrix(const CameraState* camera)
{
	glm::mat4 projection;
	//TODO

	return projection;
}

//move
void Move(const CameraState* camera, f32 speed, Direction dir)
{
	switch (dir)
	{
	case Up:
		//TODO
		break;
	case Down:
		//TODO
		break;
	case Right:
		//TODO
		break;
	case Left:
		//TODO
		break;
	default:
		break;
	}
}
