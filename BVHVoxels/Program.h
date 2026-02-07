#pragma once
#include <string>
namespace Program
{
	void Initialize(const std::string const& arguments);
	void SetDefaultEngineVariables();
	void SetLaunchEngineVariables(const std::string const& arguments);
	bool GetShouldExit();
	bool GetInitialized();
	void Exit();
	namespace
	{
		bool initialized = false;
		bool shouldExit = false;
	}
}