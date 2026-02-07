#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "HelperFunctions.h"
namespace HelperFunctions
{
	std::string CombineLaunchOptions(int argumentCount, char* argumentValues[])
	{
		//create output string starting with the first argument (will always be executable name)
		std::string output = argumentValues[0];
		for (int i = 1; i < argumentCount; i++)
		{
			//append each argument with a space between them
			output.append("    ");
			output.append(argumentValues[i]);
		}
		return output;
	}
	std::vector<std::string> SplitString(const std::string const& value, const std::string const& split, bool discardEmpty)
	{
		//create output vector, set start and end based on first segment
		std::vector<std::string> output = std::vector<std::string>();
		size_t start = 0;
		size_t end = value.find(split, start);


		//append first segment
		output.push_back(value.substr(start, end - start));

		//if first segment is not the whole string, loop
		while (end != std::string::npos)
		{
			//offset by one to skip the split character
			start = end + 1;

			//find next split
			end = value.find(split, start);

			//if discardEmpty and the segment is empty, skip
			if (discardEmpty)
			{
				if (value.find_first_not_of(" \n\t", start) >= end)
				{
					continue;
				}
			}

			//otherwise, append
			output.push_back(value.substr(start, end - start));
		}
		return output;
	}
	std::string TrimWhiteSpace(const std::string const& value)
	{
		//return without ends
		size_t start = value.find_first_not_of(" \n\t");
		return value.substr(start, value.find_last_not_of(" \n\t") - start + 1);
	}
	std::string FormatTimeDouble(double time)
	{
		//specify segments
		int milli = time * 1000;
		int seconds = time;
		int minutes = seconds / 60;
		int hours = minutes / 60;

		//combine and format in stream
		std::stringstream stream;
		stream << std::setfill('0') << std::setw(3) << hours << ":" << std::setfill('0') << std::setw(2) << minutes - hours * 60 << ":" << std::setfill('0') << std::setw(2) << seconds - minutes * 60 << "." << std::setfill('0') << std::setw(3) << milli - seconds * 1000;

		//return
		return stream.str();
	}
}