#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
int Camera::GetPriority() const
{
	return priority;
}

void Camera::SetFovDegrees(float degrees)
{
	fovDegrees = degrees;
	projection = glm::perspective(glm::radians(fovDegrees), 1.0f, 0.1f, 100.0f);
	inverseProjection = glm::inverse(projection);
}

const glm::mat4& const Camera::GetProjection() const
{
	return projection;
}

const glm::mat4& const Camera::GetInverseProjection() const
{
	return inverseProjection;
}
