#pragma once

#include <iostream>
#include <string>

// Simple logging functionality
class Logger
{
public:
	enum LoggingType : unsigned int
	{
		LoggingType_none = 0,
		LoggingType_console,
		LoggingType_textBox
	};

	// Log the percentage of current progress of the global task
	static void logProgress(int p_percentage);

	// Log any event by text
	static void log(const std::string &p_text);

	// Set how the logs are handled
	static void setLoggingMode(const Logger::LoggingType p_loggingMode) { m_loggingMode = p_loggingMode; }

private:
	static Logger::LoggingType m_loggingMode;
};