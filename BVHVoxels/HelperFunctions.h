#pragma once
#include <string>
#include <vector>
namespace HelperFunctions
{
	std::string CombineLaunchOptions(int argumentCount, char* argumentValues[]);
	std::vector<std::string> SplitString(const std::string const& value, const std::string const& split, bool discardEmpty);
	std::string TrimWhiteSpace(const std::string const& value);
	std::string FormatTimeDouble(double time);
}