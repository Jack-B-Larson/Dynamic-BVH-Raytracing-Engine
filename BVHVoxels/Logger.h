#pragma once
#include <fstream>
#include <string>
#include "ThreadSafeQueue.h"
namespace Logger
{
	void Log(const std::string const& message);
	void Error(const std::string const& message);

	void LoggerLoop();

	void CloseLogger();

	void LogInstant(const std::string const& message);

	namespace
	{
		bool shouldCloseLogger = false;
		bool allLogsCompleted = false;
		ThreadSafeQueue<std::string> messageQueue = ThreadSafeQueue<std::string>();

		std::ofstream logFile = std::ofstream();
	}
}