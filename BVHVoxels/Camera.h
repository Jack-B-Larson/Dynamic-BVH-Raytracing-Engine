#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
class Camera : public Component<Camera>
{
public:
	void SetFovDegrees(float degrees);
	const glm::mat4& const GetProjection() const;
	const glm::mat4& const GetInverseProjection() const;
	int GetPriority() const;
private:
	int priority = 0;
	float fovDegrees = 90.0f;
	glm::mat4 projection = glm::perspective(glm::radians(fovDegrees), 1.0f, 0.1f, 100.0f);
	glm::mat4 inverseProjection = glm::inverse(glm::perspective(glm::radians(fovDegrees), 1.0f, 0.1f, 100.0f));
};

