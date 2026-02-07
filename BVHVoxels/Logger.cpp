#include <iostream>
#include <string>
#include "GameTimeHandler.h"
#include "HelperFunctions.h"
#include "Logger.h"
#include "Program.h"
namespace Logger
{
	void Log(const std::string const& message)
	{
		//append message to queue
		messageQueue.push(message);
	}
	void Error(const std::string const& message)
	{
		//log error
		Log(message);

		//request program exit
		Program::Exit();

		//request logger exit as the program will be stalled
		Logger::CloseLogger();

		//stall program until logger completes and then crash
		while (!allLogsCompleted)
		{
		}
		throw;
	}

	void LoggerLoop()
	{
		//open logger file
		logFile.open("latest.log");

		//loop until close requested
		while (!shouldCloseLogger)
		{
			//log messages and lock when queue empty
			LogInstant(messageQueue.pop());
		}
		//loop through whatever is left in the queue
		while (!messageQueue.empty())
		{
			//log messages
			LogInstant(messageQueue.pop());
		}

		//close log file
		logFile.close();

		//notify other threads that the log is complete and they may close
		allLogsCompleted = true;
	}
	void CloseLogger()
	{
		//set the logger thread to close
		shouldCloseLogger = true;

		//send a message just to wake the logger thread in case queue is empty and thus locking the thread
		Log("closing logger");
	}
	void LogInstant(const std::string const& message)
	{
		//format line
		std::string formatted = "[" + HelperFunctions::FormatTimeDouble(GameTimeHandler::GetElapsedTime()) + "] " + message + "\n";

		//output and flush to console and file
		logFile << formatted;
		logFile.flush();
		std::cout << formatted;
		std::cout.flush();
	}
}