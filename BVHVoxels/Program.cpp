#include <string>
#include <thread>
#include <vector>
#include "EngineVariables.h"
#include "GameHandler.h"
#include "GameTimeHandler.h"
#include "GraphicsHandler.h"
#include "HelperFunctions.h"
#include "Logger.h"
#include "Program.h"
namespace Program
{
	void Initialize(const std::string const& arguments)
	{
		//start logger thread
		std::thread loggerThread(Logger::LoggerLoop);

		//set engine variables to defaults before attempting to run
		SetDefaultEngineVariables();

		//update engine variables based on launch options
		SetLaunchEngineVariables(arguments);

		//create window
		GraphicsHandler::Initialize();

		//start game thread
		std::thread gameThread(GameHandler::GameLoop);

		//start render thread
		std::thread renderThread(GraphicsHandler::RenderLoop);

		initialized = true;
		//enter glfw update loop
		GraphicsHandler::UpdateLoop();


		//close and join render thread
		renderThread.join();

		//close and join game thread
		gameThread.join();

		//close and join logger thread
		Logger::CloseLogger();
		loggerThread.join();

		//cleanup
		GraphicsHandler::Destroy();
	}
	void SetDefaultEngineVariables()
	{
		//set window title
		EngineVariables::SetVariableString("window_title", "BVHVoxels");
		//register callback so window title stays up to date with variables
		EngineVariables::RegisterCallbackFunction("window_title", GraphicsHandler::WindowTitleVariableCallback);


		//set to windowed mode
		EngineVariables::SetVariableBool("window_fullscreen", false);
		//set glfw version
		EngineVariables::SetVariableInt("glfw_context_version", 46);
		//set glfw swap interval (vsync)
		EngineVariables::SetVariableInt("glfw_swap_interval", 0);
		//register callback so window glfw and fullscreen settings stay up to date with variables
		EngineVariables::RegisterCallbackFunction("window_fullscreen", GraphicsHandler::WindowFullscreenVariableCallback);
		EngineVariables::RegisterCallbackFunction("glfw_context_version", GraphicsHandler::CreateMainWindow);



		//set window width, height, and aspect ratio
		EngineVariables::SetVariableInt("window_width", 800);
		EngineVariables::SetVariableInt("window_height", 450);
		EngineVariables::SetVariableFloat("window_aspect_ratio", 16.0f / 9.0f);
		//register callback so aspect ratio stays up to date with window width and height
		EngineVariables::RegisterCallbackFunction("window_width", GraphicsHandler::RecalculateAspectRatio);
		EngineVariables::RegisterCallbackFunction("window_height", GraphicsHandler::RecalculateAspectRatio);
		//register callback so window size stays up to date with window width and height
		EngineVariables::RegisterCallbackFunction("window_width", GraphicsHandler::WindowSizeVariableCallback);
		EngineVariables::RegisterCallbackFunction("window_height", GraphicsHandler::WindowSizeVariableCallback);


		//set resolution width and height
		EngineVariables::SetVariableInt("resolution_width", 3440);//800);
		EngineVariables::SetVariableInt("resolution_height", 1440);//450);
		EngineVariables::SetVariableFloat("resolution_aspect_ratio", 16.0f / 9.0f);
		//register callback so aspect ratio stays up to date with window width and height
		EngineVariables::RegisterCallbackFunction("resolution_width", GraphicsHandler::RecalculateResolutionAspectRatio);
		EngineVariables::RegisterCallbackFunction("resolution_height", GraphicsHandler::RecalculateResolutionAspectRatio);
		//register callback so texture size stays up to date with resolution width and height
		EngineVariables::RegisterCallbackFunction("resolution_width", GraphicsHandler::ResolutionSizeVariableCallback);
		EngineVariables::RegisterCallbackFunction("resolution_height", GraphicsHandler::ResolutionSizeVariableCallback);

		//register callback so update and render rates stay up to date with variables
		EngineVariables::RegisterCallbackFunction("target_update_rate", GameTimeHandler::TargetUpdateRateVariableCallback);
		EngineVariables::RegisterCallbackFunction("target_render_rate", GameTimeHandler::TargetRenderRateVariableCallback);
		//set update and render rates
		EngineVariables::SetVariableFloat("target_update_rate", 20);
		EngineVariables::SetVariableFloat("target_render_rate", -60);





		EngineVariables::SetVariableFloat("camera_mouse_sensitivity", 0.1f);

	}
	void SetLaunchEngineVariables(const std::string const& arguments)
	{
		//split launch options by +, gets executable name and then commands
		std::vector<std::string> commands = HelperFunctions::SplitString(arguments, "+", true);

		//first value will always be executable name
		EngineVariables::SetVariableString("executable_name", HelperFunctions::TrimWhiteSpace(commands[0]));

		//loop through all commands
		for (int i = 1; i < commands.size(); i++)
		{
			//split commands by spaces, first entry is key, second is value
			//then enter into engine variables
			std::vector<std::string> args = HelperFunctions::SplitString(commands[i], " ", true);
			EngineVariables::SetVariableString(args[0], args[1]);
		}
	}
	bool GetShouldExit()
	{
		return shouldExit;
	}
	bool GetInitialized()
	{
		return GraphicsHandler::GetInitialized() && GameHandler::GetInitialized() && initialized;
	}
	void Exit()
	{
		Logger::Log("program shutdown requested");
		shouldExit = true;
	}
}