#pragma once
#include <vector>
#include "Scene.h"
namespace GameHandler
{
	void GameLoop();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void Render();
	void AddScene(Scene* scene);
	bool GetInitialized();
	namespace
	{
		bool initialized = false;

		size_t loadedScene = 0;
		std::vector<Scene*> scenes = std::vector<Scene*>();
	}
}