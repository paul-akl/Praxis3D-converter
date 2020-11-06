#pragma once

#include <msclr/gcroot.h>

#include "MainWindowForm.h"

// MainWindowForm wrapper, used for connecting the functionality of managed and unmanaged objects
// (native C++ and Microsoft Windows C++/CLI)
ref class MainWindowWrapper
{
public:
	static Praxis3Dconverter::MainWindowForm ^getMainWindowForm() { return m_mainWindowForm; }
	static void setMainWindowForm(Praxis3Dconverter::MainWindowForm ^p_mainWindowForm) { m_mainWindowForm = p_mainWindowForm; }

private:
	static Praxis3Dconverter::MainWindowForm ^m_mainWindowForm = nullptr;
};

