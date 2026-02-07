#pragma once
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "BVH.h"
#include "ExecutionOrderHandler.h"
#include "GameObject.h"
class Scene
{
public:
	void PreUpdate();
	void Update();
	void PostUpdate();
	void Render();

	GameObject* InstantiateGameObject();

	template<typename componentType>
	componentType* InstantiateComponent()
	{
		//create a new component and link it to this object
		componentType* newComponent = new componentType();
		newComponent;
		components[componentType::GetComponentType()].push_back(newComponent);
		//return component
		return newComponent;
	}

	BVH* GetBVH();

private:
	const std::vector<std::type_index>& const executionOrder = ExecutionOrderHandler::GetOrder();

	std::unordered_map<std::type_index, std::vector<BaseComponent*>> components = std::unordered_map<std::type_index, std::vector<BaseComponent*>>();
	std::vector<GameObject*> gameObjects = std::vector<GameObject*>();
	BVH bvh = BVH();
};

