#include <cmath>
#include <cstdlib>
#include <string>
#include "EngineVariables.h"
#include "Logger.h"
namespace EngineVariables
{
	void SetVariableString(const std::string const& key, const std::string const& value)
	{
		//skip if unchanged
		if (GetVariableString(key) == value)
		{
			return;
		}

		//log change and update value
		Logger::Log("set variable \"" + key + "\" to \"" + value + "\"");
		engineVariables[key] = value;

		//run callbacks
		RunCallbacks(key);
	}
	void SetVariableFloat(const std::string const& key, float value)
	{
		//convert to string and update value
		SetVariableString(key, std::to_string(value));
	}
	void SetVariableInt(const std::string const& key, int value)
	{
		//convert to string and update value
		SetVariableString(key, std::to_string(value));
	}
	void SetVariableBool(const std::string const& key, bool value)
	{
		//convert to int to string and update value
		SetVariableInt(key, value);
	}


	const std::string const& GetVariableString(const std::string const& key)
	{
		//return value
		return engineVariables[key];
	}
	float GetVariableFloat(const std::string const& key)
	{
		//convert to float and return value
		return std::atof(GetVariableString(key).c_str());
	}
	int GetVariableInt(const std::string const& key)
	{
		//convert to int and return value
		return std::atoi(GetVariableString(key).c_str());
	}
	bool GetVariableBool(const std::string const& key)
	{
		//convert to int to bool and return value
		return GetVariableInt(key);
	}

	void RegisterCallbackFunction(const std::string const& key, void(*callback)(std::string const&))
	{
		//append function to callbacks mapped to key
		callbackFunctions[key].push_back(callback);
	}

	void RegisterCallbackFunction(const std::string const& key, void(*callback)())
	{
		//append function to callbacks mapped to key
		parameterlessCallbackFunctions[key].push_back(callback);
	}


	namespace
	{
		void RunCallbacks(const std::string const& key)
		{
			//loop through registered callbacks and run all with key
			for (void(*callback)(std::string const&) : callbackFunctions[key])
			{
				callback(key);
			}
			for (void(*callback)() : parameterlessCallbackFunctions[key])
			{
				callback();
			}
		}
	}
}