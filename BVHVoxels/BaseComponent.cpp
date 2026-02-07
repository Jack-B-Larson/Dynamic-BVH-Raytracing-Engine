#include "BaseComponent.h"

void BaseComponent::SetOwner(GameObject* newOwner)
{
	owner = newOwner;
}

GameObject* BaseComponent::GetOwner() const
{
	return owner;
}

void BaseComponent::Initialize()
{
}

void BaseComponent::PreUpdate()
{
}

void BaseComponent::Update()
{
}

void BaseComponent::PostUpdate()
{
}

void BaseComponent::Render()
{
}

bool BaseComponent::GetEnabled() const
{
	return enabled;
}

void BaseComponent::Enable()
{
	enabled = true;
}

void BaseComponent::Disable()
{
	enabled = false;
}
