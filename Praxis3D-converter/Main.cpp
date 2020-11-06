#include <iostream>

#include "Logger.h"
#include "MainWindowForm.h"
#include "MainWindowWrapper.h"

[System::STAThread]
int main()
{
	// Make the Windows Forms (GUI) look more modern
	System::Windows::Forms::Application::EnableVisualStyles();
	
	// Create the main GUI window form
	Praxis3Dconverter::MainWindowForm mainWindow;

	// Assign the created window form to the form wrapper
	MainWindowWrapper::setMainWindowForm(mainWindow.getMainWindowForm());

	// Set the logs to be outputted to the GUI window's text box
	Logger::setLoggingMode(Logger::LoggingType::LoggingType_textBox);
	
	// Show the main window
	mainWindow.ShowDialog();
	
	return 0;
}