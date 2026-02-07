#pragma once
#include <string>
#include <vector>
namespace FileSystemHandler
{
	bool PathExists(const std::string const& path);
	std::string LoadPlainText(const std::string const& path);
	std::vector<std::string> GetFilesInDirectory(const std::string const& path, bool recursive);
	std::string GetDirectory(const std::string const& path);
	std::string GetStem(const std::string const& path);
	std::string GetExtension(const std::string const& path);
}