#include "Logger.h"
#include "MainWindowWrapper.h"

Logger::LoggingType Logger::m_loggingMode = Logger::LoggingType::LoggingType_none;

void Logger::logProgress(int p_percentage)
{
	MainWindowWrapper::getMainWindowForm()->setProgressBarValue(p_percentage);
}

void Logger::log(const std::string &p_text)
{
	switch(m_loggingMode)
	{
	// Do not log anything
	case LoggingType_none:
		break;
	// Print the logging text to console
	case LoggingType_console:
		std::cout << p_text << std::endl;
		break;
	// Output the logging text to the text box console inside the main window form
	case LoggingType_textBox:
		MainWindowWrapper::getMainWindowForm()->textBoxConsoleAddTextLine(p_text);
		break;
	}
}
