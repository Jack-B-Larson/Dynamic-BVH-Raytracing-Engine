#include <GLFW/glfw3.h>
#include "GameObject.h"
#include "GameTimeHandler.h"
#include "InputHandler.h"
#include "Movement.h"
#include "Transform.h"
void Movement::Update()
{
	Transform* transform = GetOwner()->GetComponentOfType<Transform>();
	glm::vec3 newPosition = transform->GetLocalPosition();
	glm::vec3 delta = glm::vec3(0);
	delta += transform->TransformDirection(glm::vec3(0, 0, InputHandler::GetKeyHeld(GLFW_KEY_W) - InputHandler::GetKeyHeld(GLFW_KEY_S)));
	delta += transform->TransformDirection(glm::vec3(InputHandler::GetKeyHeld(GLFW_KEY_D) - InputHandler::GetKeyHeld(GLFW_KEY_A), 0, 0));
	delta *= 25 * GameTimeHandler::GetUpdateDeltaTime();
	transform->SetLocalPosition(newPosition + delta, false);
}
