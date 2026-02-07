#include <vector>
#include "BaseComponent.h"
#include "GameObject.h"

const std::vector<BaseComponent*>& const GameObject::GetComponents()
{
	return components;
}

bool GameObject::GetEnabled() const
{
	return enabled;
}

void GameObject::Enable()
{
	enabled = true;
}

void GameObject::Disable()
{
	enabled = false;
}

void GameObject::SetScene(Scene* newScene)
{
	scene = newScene;
}

Scene* GameObject::GetScene() const
{
	return scene;
}
