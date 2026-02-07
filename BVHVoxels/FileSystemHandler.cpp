#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "FileSystemHandler.h"
#include "Logger.h"
namespace FileSystemHandler
{
	bool PathExists(const std::string const& path)
	{
		//return whether the path exists
		return std::filesystem::exists(path);
	}
	std::string LoadPlainText(const std::string const& path)
	{
		//error if reading non-existant file
		if (!PathExists(path))
		{
			Logger::Error("requested file does not exist at \"" + path + "\"");
		}

		//create stream to file and set exceptions
		std::ifstream fileStream(path);
		fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		//try read-return or error
		try
		{
			return std::string(std::istreambuf_iterator<char>(fileStream), {});
		}
		catch (const std::ifstream::failure exception)
		{
			Logger::Error("file read failed \"" + path + "\"");
		}
	}
	std::vector<std::string> GetFilesInDirectory(const std::string const& path, bool recursive)
	{
		//error if reading non-existant path
		if (!PathExists(path))
		{
			Logger::Error("requested directory does not exist at \"" + path + "\"");
		}

		//skip directories (if non-recursive) and add paths
		std::vector<std::string> files;
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				if (recursive)
				{
					files.append_range(GetFilesInDirectory(entry.path().string(), true));
				}
				continue;
			}
			files.push_back(entry.path().string());
		}

		//return
		return files;
	}
	std::string GetDirectory(const std::string const& path)
	{
		//return directory split of path
		return std::filesystem::path(path).parent_path().string();
	}
	std::string GetStem(const std::string const& path)
	{
		//return stem split of path
		return std::filesystem::path(path).stem().string();
	}
	std::string GetExtension(const std::string const& path)
	{
		//return extension split of path
		return std::filesystem::path(path).extension().string();
	}
}