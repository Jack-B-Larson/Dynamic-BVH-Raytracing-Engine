#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "EngineVariables.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "MouseLook.h"
#include "Transform.h"
void MouseLook::Render()
{
	float sensitivity = EngineVariables::GetVariableFloat("camera_mouse_sensitivity");
	yaw += InputHandler::GetRawMouseDelta().x * sensitivity;
	yaw = yaw - int(yaw / 360) * 360;
	pitch += InputHandler::GetRawMouseDelta().y * sensitivity;
	pitch = std::fmin(std::fmax(pitch, -90.0f), 90.0f);
	GetOwner()->GetComponentOfType<Transform>()->SetLocalRotation(glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0)), true);
}
