#include "BobAndSpin.h"
#include "GameObject.h"
#include "GameTimeHandler.h"
#include "Transform.h"
void BobAndSpin::Update()
{
	progress += GameTimeHandler::GetUpdateDeltaTime() * 0.25f;
	progress -= int(progress);

	Transform* transform = GetOwner()->GetComponentOfType<Transform>();
	transform->SetLocalPosition(glm::vec3(0, sin(progress * glm::radians(720.0f)), 0), false);
	transform->SetLocalRotation(glm::quat(glm::vec3(0, progress * glm::radians(360.0f), 0)), false);
}
