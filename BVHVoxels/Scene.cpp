#include "BaseComponent.h"
#include "Camera.h"
#include "GameObject.h"
#include "GraphicsHandler.h"
#include "Scene.h"

void Scene::PreUpdate()
{
	//preupdate all game objects
	for (std::type_index i : executionOrder)
	{
		for (BaseComponent* component : components[i])
		{
			//skip component if disabled
			if (!component->GetEnabled())
			{
				continue;
			}
			if (!component->GetOwner()->GetEnabled())
			{
				continue;
			}
			//preupdate individual component
			component->PreUpdate();
		}
	}
}

void Scene::Update()
{
	//update all game objects
	for (std::type_index i : executionOrder)
	{
		for (BaseComponent* component : components[i])
		{
			//skip component if disabled
			if (!component->GetEnabled())
			{
				continue;
			}
			if (!component->GetOwner()->GetEnabled())
			{
				continue;
			}
			//update individual component
			component->Update();
		}
	}
}

void Scene::PostUpdate()
{
	//postupdate all game objects
	for (std::type_index i : executionOrder)
	{
		for (BaseComponent* component : components[i])
		{
			//skip component if disabled
			if (!component->GetEnabled())
			{
				continue;
			}
			if (!component->GetOwner()->GetEnabled())
			{
				continue;
			}
			//postupdate individual component
			component->PostUpdate();
		}
	}
}

void Scene::Render()
{
	//create pointer for active camera selection
	Camera* activeCamera = nullptr;

	//render all game objects
	for (std::type_index i : executionOrder)
	{
		for (BaseComponent* component : components[i])
		{
			//skip component if disabled
			if (!component->GetEnabled())
			{
				continue;
			}
			if (!component->GetOwner()->GetEnabled())
			{
				continue;
			}
			//render individual component
			component->Render();
		}
	}

	for (BaseComponent* component : components[Camera::GetComponentType()])
	{
		Camera* cameraComponent = static_cast<Camera*>(component);

		//if current camera is null set it regardless of priority
		if (activeCamera == nullptr)
		{
			activeCamera = cameraComponent;
			continue;
		}

		//update active camera based on priority
		if (cameraComponent->GetPriority() > activeCamera->GetPriority())
		{
			activeCamera = cameraComponent;
		}
	}

	//if a valid camera was found, update shader values
	if (activeCamera != nullptr)
	{
		GraphicsHandler::SetCamera(activeCamera);
	}


	bvh.Render();
}

GameObject* Scene::InstantiateGameObject()
{
	//create a new game object and link it to this scene
	GameObject* addedGameObject = new GameObject();
	addedGameObject->SetScene(this);
	gameObjects.push_back(addedGameObject);
	//return game object
	return addedGameObject;
}

BVH* Scene::GetBVH()
{
	return &bvh;
}
