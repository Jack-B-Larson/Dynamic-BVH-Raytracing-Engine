#pragma once
#include <string>
#include <unordered_map>
#include <vector>
namespace EngineVariables
{
	void SetVariableString(const std::string const& key, const std::string const& value);
	void SetVariableFloat(const std::string const& key, float value);
	void SetVariableInt(const std::string const& key, int value);
	void SetVariableBool(const std::string const& key, bool value);

	const std::string const& GetVariableString(const std::string const& key);
	float GetVariableFloat(const std::string const& key);
	int GetVariableInt(const std::string const& key);
	bool GetVariableBool(const std::string const& key);

	void RegisterCallbackFunction(const std::string const& key, void(*callback)(std::string const&));
	void RegisterCallbackFunction(const std::string const& key, void(*callback)());

	namespace
	{
		void RunCallbacks(std::string const& key);

		std::unordered_map<std::string, std::string> engineVariables = std::unordered_map<std::string, std::string>();
		std::unordered_map<std::string, std::vector<void(*)(std::string const&)>> callbackFunctions = std::unordered_map<std::string, std::vector<void(*)(std::string const&)>>();
		std::unordered_map<std::string, std::vector<void(*)()>> parameterlessCallbackFunctions = std::unordered_map<std::string, std::vector<void(*)()>>();
	}
}