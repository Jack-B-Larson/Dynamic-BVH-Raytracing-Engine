#pragma once
#include <vector>
#include "BaseComponent.h"
#include "Logger.h"

class Scene;

class GameObject
{
public:
	template<typename componentType>
	componentType* GetComponentOfType()
	{
		//loop through components
		for (BaseComponent* component : components)
		{
			//return first matching type
			if (component->GetType() == componentType::GetComponentType())
			{
				return static_cast<componentType*>(component);
			}
		}
		//otherwise error
		Logger::Error("gameobject lacks requested component type");
	}


	void AddComponent(BaseComponent* component)
	{
		//link it to this object
		component->SetOwner(this);
		components.push_back(component);
		component->Initialize();
	}

	const std::vector<BaseComponent*>& const GetComponents();

	bool GetEnabled() const;

	void Enable();
	void Disable();

	void SetScene(Scene* newScene);
	Scene* GetScene() const;

private:
	bool enabled = false;
	Scene* scene = nullptr;
	std::vector<BaseComponent*> components = std::vector<BaseComponent*>();
};

