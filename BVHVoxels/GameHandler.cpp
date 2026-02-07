#include "EngineVariables.h"
#include "ExecutionOrderHandler.h"
#include "GameHandler.h"
#include "GameTimeHandler.h"
#include "HelperFunctions.h"
#include "InputHandler.h"
#include "Program.h"
#include "Scene.h"
#include "SceneBuilder.h"
namespace GameHandler
{
	void GameLoop()
	{
		ExecutionOrderHandler::PopulateOrder();

		AddScene(SceneBuilder::TestScene());

		PreUpdate();
		PostUpdate();

		initialized = true;

		while (!Program::GetInitialized())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		//loop update while program isn't closing
		while (!Program::GetShouldExit())
		{
			//update if allowed by target update time
			if (GameTimeHandler::ShouldUpdate())
			{
				InputHandler::FlushKeyStatus();

				PreUpdate();

				GameTimeHandler::Update();

				//transform->SetLocalPosition(glm::vec3(sin(GameTimeHandler::GetUpdateTime()), 0, cos(GameTimeHandler::GetUpdateTime())));
				//transform->SetLocalRotation(glm::quatLookAt(-transform->GetLocalPosition(), glm::vec3(0, 1, 0)));
				Update();

				PostUpdate();
			}
		}
	}
	void PreUpdate()
	{
		EngineVariables::SetVariableString("window_title", HelperFunctions::FormatTimeDouble(GameTimeHandler::GetElapsedTime()) + " UPS: " + std::to_string(1 / GameTimeHandler::GetUpdateDeltaTime()) + ", FPS: " + std::to_string(1 / GameTimeHandler::GetRenderDeltaTime()));

		//skip preupdate if no loaded scene
		if (loadedScene >= scenes.size())
		{
			return;
		}

		//preupdate loaded scene
		scenes[loadedScene]->PreUpdate();
	}
	void Update()
	{
		//skip update if no loaded scene
		if (loadedScene >= scenes.size())
		{
			return;
		}

		//update loaded scene
		scenes[loadedScene]->Update();
	}
	void PostUpdate()
	{
		//skip update if no loaded scene
		if (loadedScene >= scenes.size())
		{
			return;
		}

		//update loaded scene
		scenes[loadedScene]->PostUpdate();
	}
	void Render()
	{
		InputHandler::ResetMouseDelta();

		//skip render if no loaded scene
		if (loadedScene >= scenes.size())
		{
			return;
		}
		//render loaded scene
		scenes[loadedScene]->Render();

	}
	void AddScene(Scene* scene)
	{
		//add scene to list
		scenes.push_back(scene);
	}
	bool GetInitialized()
	{
		return initialized;
	}
}