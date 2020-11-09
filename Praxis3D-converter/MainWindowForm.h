#pragma once

#include <stdlib.h>
#include <string.h>
#include <msclr\marshal_cppstd.h>
#include <string>
#include <thread>

#include "SceneConverter.h"

namespace Praxis3Dconverter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainWindowForm
	/// </summary>
	public ref class MainWindowForm : public System::Windows::Forms::Form
	{
	public:
		MainWindowForm(void)
		{
			m_converter = new SceneConverter();
			InitializeComponent();
		}
		
		// Adds the text to the TextBoxConsole with a NewLine at the start
		void textBoxConsoleAddTextLine(const std::string p_text)
		{
			// Convert unmanaged std::string to managed System::String
			// and call the thread-safe method
			textBoxConsoleAddTextLineSafe(toString(p_text));
		}
		
		// Sets the value of the progress bar used to show the progress of conversion
		void setProgressBarValue(int p_value)
		{
			setProgressBarValueSafe(p_value);
		}

		// Returns a managed pointer to itself
		MainWindowForm ^getMainWindowForm() { return this; }

	protected:
		~MainWindowForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		System::Windows::Forms::TextBox ^m_textBoxConsole;
		System::Windows::Forms::Button ^m_buttonConvert;
		System::Windows::Forms::FolderBrowserDialog ^m_folderBrowserDialogExport;
		System::Windows::Forms::OpenFileDialog ^m_openFileDialogImporter;
		System::Windows::Forms::TextBox ^m_textBoxPathImport;
		System::Windows::Forms::Button ^m_buttonBrowseImport;
		System::Windows::Forms::Button ^m_buttonBrowseExport;
		System::Windows::Forms::TextBox ^m_textBoxPathExport;
		System::Windows::Forms::Label ^m_labelImporter;
		System::Windows::Forms::Label ^m_labelExporter;
		System::Windows::Forms::Button ^m_buttonBrowseAssets;
		System::Windows::Forms::TextBox ^m_textBoxPathAssets;
		System::Windows::Forms::Label ^m_labelAssets;
		System::Windows::Forms::CheckBox ^m_checkBoxAssets;
		System::Windows::Forms::FolderBrowserDialog ^m_folderBrowserDialogAssets;
		System::Windows::Forms::ProgressBar ^m_progressBarConverter;
		System::Windows::Forms::MenuStrip ^menuStrip1;
		System::Windows::Forms::ToolStripMenuItem ^fileToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem ^m_exitToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem ^m_aboutToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem ^m_openFileToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem ^m_saveDirectoryToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem ^m_helpToolStripMenuItem;
		System::Windows::Forms::ToolStripSeparator ^toolStripSeparator1;
		System::Windows::Forms::ToolStripStatusLabel ^m_toolStripStatusLabel;
		System::Windows::Forms::StatusStrip ^m_statusStrip;
		System::Windows::Forms::ToolStripMenuItem ^m_documentationToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem ^m_licenseToolStripMenuItem;

		SceneConverter *m_converter;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->m_textBoxConsole = (gcnew System::Windows::Forms::TextBox());
			this->m_buttonConvert = (gcnew System::Windows::Forms::Button());
			this->m_folderBrowserDialogExport = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->m_openFileDialogImporter = (gcnew System::Windows::Forms::OpenFileDialog());
			this->m_textBoxPathImport = (gcnew System::Windows::Forms::TextBox());
			this->m_buttonBrowseImport = (gcnew System::Windows::Forms::Button());
			this->m_buttonBrowseExport = (gcnew System::Windows::Forms::Button());
			this->m_textBoxPathExport = (gcnew System::Windows::Forms::TextBox());
			this->m_labelImporter = (gcnew System::Windows::Forms::Label());
			this->m_labelExporter = (gcnew System::Windows::Forms::Label());
			this->m_buttonBrowseAssets = (gcnew System::Windows::Forms::Button());
			this->m_textBoxPathAssets = (gcnew System::Windows::Forms::TextBox());
			this->m_labelAssets = (gcnew System::Windows::Forms::Label());
			this->m_checkBoxAssets = (gcnew System::Windows::Forms::CheckBox());
			this->m_folderBrowserDialogAssets = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->m_progressBarConverter = (gcnew System::Windows::Forms::ProgressBar());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_openFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_saveDirectoryToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->m_exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_documentationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_licenseToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->m_toolStripStatusLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->m_statusStrip = (gcnew System::Windows::Forms::StatusStrip());
			this->menuStrip1->SuspendLayout();
			this->m_statusStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// m_textBoxConsole
			// 
			this->m_textBoxConsole->Location = System::Drawing::Point(12, 201);
			this->m_textBoxConsole->Multiline = true;
			this->m_textBoxConsole->Name = L"m_textBoxConsole";
			this->m_textBoxConsole->ReadOnly = true;
			this->m_textBoxConsole->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->m_textBoxConsole->Size = System::Drawing::Size(560, 235);
			this->m_textBoxConsole->TabIndex = 0;
			// 
			// m_buttonConvert
			// 
			this->m_buttonConvert->Location = System::Drawing::Point(472, 40);
			this->m_buttonConvert->Name = L"m_buttonConvert";
			this->m_buttonConvert->Size = System::Drawing::Size(100, 30);
			this->m_buttonConvert->TabIndex = 1;
			this->m_buttonConvert->Text = L"Convert";
			this->m_buttonConvert->UseVisualStyleBackColor = true;
			this->m_buttonConvert->Click += gcnew System::EventHandler(this, &MainWindowForm::m_buttonConvert_Click);
			// 
			// m_openFileDialogImporter
			// 
			this->m_openFileDialogImporter->Filter = L"Exodus JSON files (*.json)|*.json";
			// 
			// m_textBoxPathImport
			// 
			this->m_textBoxPathImport->Location = System::Drawing::Point(102, 92);
			this->m_textBoxPathImport->Name = L"m_textBoxPathImport";
			this->m_textBoxPathImport->Size = System::Drawing::Size(409, 20);
			this->m_textBoxPathImport->TabIndex = 2;
			// 
			// m_buttonBrowseImport
			// 
			this->m_buttonBrowseImport->Location = System::Drawing::Point(517, 90);
			this->m_buttonBrowseImport->Name = L"m_buttonBrowseImport";
			this->m_buttonBrowseImport->Size = System::Drawing::Size(55, 23);
			this->m_buttonBrowseImport->TabIndex = 3;
			this->m_buttonBrowseImport->Text = L"Browse";
			this->m_buttonBrowseImport->UseVisualStyleBackColor = true;
			this->m_buttonBrowseImport->Click += gcnew System::EventHandler(this, &MainWindowForm::m_buttonBrowseImport_Click);
			// 
			// m_buttonBrowseExport
			// 
			this->m_buttonBrowseExport->Location = System::Drawing::Point(517, 119);
			this->m_buttonBrowseExport->Name = L"m_buttonBrowseExport";
			this->m_buttonBrowseExport->Size = System::Drawing::Size(55, 23);
			this->m_buttonBrowseExport->TabIndex = 4;
			this->m_buttonBrowseExport->Text = L"Browse";
			this->m_buttonBrowseExport->UseVisualStyleBackColor = true;
			this->m_buttonBrowseExport->Click += gcnew System::EventHandler(this, &MainWindowForm::m_buttonBrowseExport_Click);
			// 
			// m_textBoxPathExport
			// 
			this->m_textBoxPathExport->Location = System::Drawing::Point(102, 121);
			this->m_textBoxPathExport->Name = L"m_textBoxPathExport";
			this->m_textBoxPathExport->Size = System::Drawing::Size(409, 20);
			this->m_textBoxPathExport->TabIndex = 5;
			// 
			// m_labelImporter
			// 
			this->m_labelImporter->AutoSize = true;
			this->m_labelImporter->Location = System::Drawing::Point(41, 95);
			this->m_labelImporter->Name = L"m_labelImporter";
			this->m_labelImporter->Size = System::Drawing::Size(55, 13);
			this->m_labelImporter->TabIndex = 6;
			this->m_labelImporter->Text = L"Import file:";
			// 
			// m_labelExporter
			// 
			this->m_labelExporter->AutoSize = true;
			this->m_labelExporter->Location = System::Drawing::Point(13, 124);
			this->m_labelExporter->Name = L"m_labelExporter";
			this->m_labelExporter->Size = System::Drawing::Size(83, 13);
			this->m_labelExporter->TabIndex = 7;
			this->m_labelExporter->Text = L"Export directory:";
			// 
			// m_buttonBrowseAssets
			// 
			this->m_buttonBrowseAssets->Enabled = false;
			this->m_buttonBrowseAssets->Location = System::Drawing::Point(517, 148);
			this->m_buttonBrowseAssets->Name = L"m_buttonBrowseAssets";
			this->m_buttonBrowseAssets->Size = System::Drawing::Size(55, 23);
			this->m_buttonBrowseAssets->TabIndex = 8;
			this->m_buttonBrowseAssets->Text = L"Browse";
			this->m_buttonBrowseAssets->UseVisualStyleBackColor = true;
			this->m_buttonBrowseAssets->Click += gcnew System::EventHandler(this, &MainWindowForm::m_buttonBrowseAssets_Click);
			// 
			// m_textBoxPathAssets
			// 
			this->m_textBoxPathAssets->Enabled = false;
			this->m_textBoxPathAssets->Location = System::Drawing::Point(102, 151);
			this->m_textBoxPathAssets->Name = L"m_textBoxPathAssets";
			this->m_textBoxPathAssets->Size = System::Drawing::Size(409, 20);
			this->m_textBoxPathAssets->TabIndex = 9;
			// 
			// m_labelAssets
			// 
			this->m_labelAssets->AutoSize = true;
			this->m_labelAssets->Location = System::Drawing::Point(12, 154);
			this->m_labelAssets->Name = L"m_labelAssets";
			this->m_labelAssets->Size = System::Drawing::Size(84, 13);
			this->m_labelAssets->TabIndex = 10;
			this->m_labelAssets->Text = L"Assets directory:";
			// 
			// m_checkBoxAssets
			// 
			this->m_checkBoxAssets->AutoSize = true;
			this->m_checkBoxAssets->Location = System::Drawing::Point(102, 177);
			this->m_checkBoxAssets->Name = L"m_checkBoxAssets";
			this->m_checkBoxAssets->Size = System::Drawing::Size(155, 17);
			this->m_checkBoxAssets->TabIndex = 11;
			this->m_checkBoxAssets->Text = L"Use custom assets location";
			this->m_checkBoxAssets->UseVisualStyleBackColor = true;
			this->m_checkBoxAssets->CheckedChanged += gcnew System::EventHandler(this, &MainWindowForm::m_checkBoxAssets_CheckedChanged);
			// 
			// m_progressBarConverter
			// 
			this->m_progressBarConverter->Location = System::Drawing::Point(12, 40);
			this->m_progressBarConverter->Name = L"m_progressBarConverter";
			this->m_progressBarConverter->Size = System::Drawing::Size(454, 30);
			this->m_progressBarConverter->TabIndex = 12;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(3) {
				this->fileToolStripMenuItem,
					this->m_helpToolStripMenuItem, this->m_aboutToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(584, 24);
			this->menuStrip1->TabIndex = 13;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(4) {
				this->m_openFileToolStripMenuItem,
					this->m_saveDirectoryToolStripMenuItem, this->toolStripSeparator1, this->m_exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// m_openFileToolStripMenuItem
			// 
			this->m_openFileToolStripMenuItem->Name = L"m_openFileToolStripMenuItem";
			this->m_openFileToolStripMenuItem->Size = System::Drawing::Size(157, 22);
			this->m_openFileToolStripMenuItem->Text = L"Open file...";
			this->m_openFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindowForm::m_openFileToolStripMenuItem_Click);
			// 
			// m_saveDirectoryToolStripMenuItem
			// 
			this->m_saveDirectoryToolStripMenuItem->Name = L"m_saveDirectoryToolStripMenuItem";
			this->m_saveDirectoryToolStripMenuItem->Size = System::Drawing::Size(157, 22);
			this->m_saveDirectoryToolStripMenuItem->Text = L"Save directory...";
			this->m_saveDirectoryToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindowForm::m_saveDirectoryToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(154, 6);
			// 
			// m_exitToolStripMenuItem
			// 
			this->m_exitToolStripMenuItem->Name = L"m_exitToolStripMenuItem";
			this->m_exitToolStripMenuItem->Size = System::Drawing::Size(157, 22);
			this->m_exitToolStripMenuItem->Text = L"Exit";
			this->m_exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindowForm::m_exitToolStripMenuItem_Click);
			// 
			// m_helpToolStripMenuItem
			// 
			this->m_helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(2) {
				this->m_documentationToolStripMenuItem,
					this->m_licenseToolStripMenuItem
			});
			this->m_helpToolStripMenuItem->Name = L"m_helpToolStripMenuItem";
			this->m_helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->m_helpToolStripMenuItem->Text = L"Help";
			this->m_helpToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindowForm::m_helpToolStripMenuItem_Click);
			// 
			// m_documentationToolStripMenuItem
			// 
			this->m_documentationToolStripMenuItem->Name = L"m_documentationToolStripMenuItem";
			this->m_documentationToolStripMenuItem->Size = System::Drawing::Size(157, 22);
			this->m_documentationToolStripMenuItem->Text = L"Documentation";
			this->m_documentationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindowForm::m_documentationToolStripMenuItem_Click);
			// 
			// m_licenseToolStripMenuItem
			// 
			this->m_licenseToolStripMenuItem->Name = L"m_licenseToolStripMenuItem";
			this->m_licenseToolStripMenuItem->Size = System::Drawing::Size(157, 22);
			this->m_licenseToolStripMenuItem->Text = L"License";
			this->m_licenseToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindowForm::m_licenseToolStripMenuItem_Click);
			// 
			// m_aboutToolStripMenuItem
			// 
			this->m_aboutToolStripMenuItem->Name = L"m_aboutToolStripMenuItem";
			this->m_aboutToolStripMenuItem->Size = System::Drawing::Size(52, 20);
			this->m_aboutToolStripMenuItem->Text = L"About";
			this->m_aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainWindowForm::m_aboutToolStripMenuItem_Click);
			// 
			// m_toolStripStatusLabel
			// 
			this->m_toolStripStatusLabel->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->m_toolStripStatusLabel->Name = L"m_toolStripStatusLabel";
			this->m_toolStripStatusLabel->Size = System::Drawing::Size(569, 17);
			this->m_toolStripStatusLabel->Spring = true;
			this->m_toolStripStatusLabel->Text = L"Ready";
			this->m_toolStripStatusLabel->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// m_statusStrip
			// 
			this->m_statusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^  >(1) { this->m_toolStripStatusLabel });
			this->m_statusStrip->Location = System::Drawing::Point(0, 439);
			this->m_statusStrip->Name = L"m_statusStrip";
			this->m_statusStrip->Size = System::Drawing::Size(584, 22);
			this->m_statusStrip->SizingGrip = false;
			this->m_statusStrip->TabIndex = 14;
			this->m_statusStrip->Text = L"statusStrip1";
			// 
			// MainWindowForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(584, 461);
			this->Controls->Add(this->m_statusStrip);
			this->Controls->Add(this->m_progressBarConverter);
			this->Controls->Add(this->m_checkBoxAssets);
			this->Controls->Add(this->m_labelAssets);
			this->Controls->Add(this->m_textBoxPathAssets);
			this->Controls->Add(this->m_buttonBrowseAssets);
			this->Controls->Add(this->m_labelExporter);
			this->Controls->Add(this->m_labelImporter);
			this->Controls->Add(this->m_textBoxPathExport);
			this->Controls->Add(this->m_buttonBrowseExport);
			this->Controls->Add(this->m_buttonBrowseImport);
			this->Controls->Add(this->m_textBoxPathImport);
			this->Controls->Add(this->m_buttonConvert);
			this->Controls->Add(this->m_textBoxConsole);
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->HelpButton = true;
			this->MainMenuStrip = this->menuStrip1;
			this->MaximizeBox = false;
			this->Name = L"MainWindowForm";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->Text = L"Praxis3D converter";
			this->Load += gcnew System::EventHandler(this, &MainWindowForm::MainWindowForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->m_statusStrip->ResumeLayout(false);
			this->m_statusStrip->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		// this->Icon = gcnew System::Drawing::Icon(L"praxis_icon.ico");
		// Used as a function pointer for textBox functions
		delegate void setObjectTextDelegate(System::String ^p_text);

		// Used as a function pointer for progressBar functions
		delegate void setProgressBarValueDelegate(int p_value);
		
		// Used as a function pointer for object->Enabled functions
		delegate void setObjectEnabledDelegate(bool p_enabled);

		// Adds the text to the TextBoxConsole with a NewLine at the start
		// Thread-safe - checks and uses invocation if necessary
		void textBoxConsoleAddTextLineSafe(System::String ^p_text)
		{
			// Check whether the calling thread owns the textBox object
			if(m_textBoxConsole->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectTextDelegate ^textBoxDelegate = gcnew setObjectTextDelegate(this, &MainWindowForm::textBoxConsoleAddTextLineSafe);
				this->Invoke(textBoxDelegate, gcnew array<Object^> { p_text });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_textBoxConsole->AppendText(System::Environment::NewLine + p_text);
				Update();
			}
		}

		// Sets the value of the progress bar used to show the progress of conversion
		// Thread-safe - checks and uses invocation if necessary
		void setProgressBarValueSafe(int p_value)
		{			
			// Check whether the calling thread owns the textBox object
			if(m_progressBarConverter->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setProgressBarValueDelegate ^progressBarDelegate = gcnew setProgressBarValueDelegate(this, &MainWindowForm::setProgressBarValueSafe);
				this->Invoke(progressBarDelegate, gcnew array<Object^> {p_value});
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_progressBarConverter->Value = p_value;
			}
		}
				
		// Sets the text of status label
		// Thread-safe - checks and uses invocation if necessary
		void setLabelStatusTextSafe(System::String ^p_text)
		{			
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectTextDelegate ^buttonConvertDelegate = gcnew setObjectTextDelegate(this, &MainWindowForm::setLabelStatusTextSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_text });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_toolStripStatusLabel->Text = p_text;
			}
		}

		// Sets whether the convert button state is enabled
		// Thread-safe - checks and uses invocation if necessary
		void setButtonConvertEnabledSafe(bool p_enabled)
		{	
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectEnabledDelegate ^buttonConvertDelegate = gcnew setObjectEnabledDelegate(this, &MainWindowForm::setButtonConvertEnabledSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_enabled });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_buttonConvert->Enabled = p_enabled;
			}
		}

		// Sets whether the textBox for import path state is enabled
		// Thread-safe - checks and uses invocation if necessary
		void setTextBoxImportEnabledSafe(bool p_enabled)
		{	
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectEnabledDelegate ^buttonConvertDelegate = gcnew setObjectEnabledDelegate(this, &MainWindowForm::setTextBoxImportEnabledSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_enabled });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_textBoxPathImport->Enabled = p_enabled;
			}
		}

		// Sets whether the browse for import path button state is enabled
		// Thread-safe - checks and uses invocation if necessary
		void setButtonImportEnabledSafe(bool p_enabled)
		{	
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectEnabledDelegate ^buttonConvertDelegate = gcnew setObjectEnabledDelegate(this, &MainWindowForm::setButtonImportEnabledSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_enabled });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_buttonBrowseImport->Enabled = p_enabled;
			}
		}

		// Sets whether the textBox for export path state is enabled
		// Thread-safe - checks and uses invocation if necessary
		void setTextBoxExportEnabledSafe(bool p_enabled)
		{	
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectEnabledDelegate ^buttonConvertDelegate = gcnew setObjectEnabledDelegate(this, &MainWindowForm::setTextBoxExportEnabledSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_enabled });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_textBoxPathExport->Enabled = p_enabled;
			}
		}

		// Sets whether the browse for export path button state is enabled
		// Thread-safe - checks and uses invocation if necessary
		void setButtonExportEnabledSafe(bool p_enabled)
		{	
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectEnabledDelegate ^buttonConvertDelegate = gcnew setObjectEnabledDelegate(this, &MainWindowForm::setButtonExportEnabledSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_enabled });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_buttonBrowseExport->Enabled = p_enabled;
			}
		}

		// Sets whether the convert button state is enabled
		// Thread-safe - checks and uses invocation if necessary
		void setTextboxAssetsEnabledSafe(bool p_enabled)
		{	
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectEnabledDelegate ^buttonConvertDelegate = gcnew setObjectEnabledDelegate(this, &MainWindowForm::setTextboxAssetsEnabledSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_enabled });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_textBoxPathAssets->Enabled = p_enabled;
			}
		}

		// Sets whether the browse for assets path button state is enabled
		// Thread-safe - checks and uses invocation if necessary
		void setButtonAssetsEnabledSafe(bool p_enabled)
		{	
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectEnabledDelegate ^buttonConvertDelegate = gcnew setObjectEnabledDelegate(this, &MainWindowForm::setButtonConvertEnabledSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_enabled });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_buttonBrowseAssets->Enabled = p_enabled;
			}
		}
		
		// Sets whether the check-box (for enabling the assets path selection) state is enabled
		// Thread-safe - checks and uses invocation if necessary
		void setCheckboxAssetsEnabledSafe(bool p_enabled)
		{	
			// Check whether the calling thread owns the button object
			if(m_buttonConvert->InvokeRequired)
			{
				// If it doesn't, invoke the right to the object
				setObjectEnabledDelegate ^buttonConvertDelegate = gcnew setObjectEnabledDelegate(this, &MainWindowForm::setCheckboxAssetsEnabledSafe);
				this->Invoke(buttonConvertDelegate, gcnew array<Object^> { p_enabled });
			}
			else
			{
				// If this thread owns the rights to the object, just perform the task
				m_checkBoxAssets->Enabled = p_enabled;
			}
		}

		// Convert System::string to std::string (using marshaling)
		inline std::string toString(System::String ^p_managedString) { return msclr::interop::marshal_as<std::string>(p_managedString); }

		//Convert std::string to System::String
		inline System::String ^toString(const std::string &p_unmanagedString) { return gcnew System::String(p_unmanagedString.c_str()); }

		// Uses a SceneConverter to perform the conversion
		// Used for multi-threading
		void convertScene()
		{
			setLabelStatusTextSafe("Converting...");

			// Disable GUI elements while converting is in progress
			setButtonConvertEnabledSafe(false);
			setTextBoxImportEnabledSafe(false);
			setButtonImportEnabledSafe(false);
			setTextBoxExportEnabledSafe(false);
			setButtonExportEnabledSafe(false);
			setCheckboxAssetsEnabledSafe(false);

			bool assetPathObjectsEnabledState = false;
			if(m_textBoxPathAssets->Enabled)
			{
				setTextboxAssetsEnabledSafe(false);
				setButtonAssetsEnabledSafe(false);
				assetPathObjectsEnabledState = true;
			}

			// Pass the assets folder location, if the custom assets location checkBox is checked
			if(m_checkBoxAssets->Checked)
				m_converter->convert(toString(m_textBoxPathImport->Text), toString(m_textBoxPathExport->Text), toString(m_textBoxPathAssets->Text));
			else
				m_converter->convert(toString(m_textBoxPathImport->Text), toString(m_textBoxPathExport->Text));
			
			// Enable the GUI elements after the converting has been completed
			setButtonConvertEnabledSafe(true);
			setTextBoxImportEnabledSafe(true);
			setButtonImportEnabledSafe(true);
			setTextBoxExportEnabledSafe(true);
			setButtonExportEnabledSafe(true);
			setCheckboxAssetsEnabledSafe(true);

			if(assetPathObjectsEnabledState)
			{
				setTextboxAssetsEnabledSafe(assetPathObjectsEnabledState);
				setButtonAssetsEnabledSafe(assetPathObjectsEnabledState);
			}
			
			setLabelStatusTextSafe("Done");
		}

		System::Void m_buttonConvert_Click(System::Object ^sender, System::EventArgs ^e)
		{
			// Check if both import and export paths are set
			if(System::String::IsNullOrEmpty(m_textBoxPathImport->Text) || System::String::IsNullOrEmpty(m_textBoxPathExport->Text))
			{
				MessageBox::Show("\"Import file\" and \"Export directory\" must be set before converting.", "Error: insufficient parameters");
			}
			else
			{
				// Check if assets path is set when the asset checkbox is ticked
				if(m_checkBoxAssets->Checked && System::String::IsNullOrEmpty(m_textBoxPathAssets->Text))
				{
					MessageBox::Show("\"Assets directory\" must be set (or disabled via \"Use custom assets location\" checkbox) before converting.", "Error: insufficient parameters");
				}
				else
				{
					// Clear the console before proceeding, as it might not be the first conversion performed during current session
					m_textBoxConsole->Clear();

					// Create a new thread for conversion
					System::Threading::Thread ^backgroundThread = gcnew System::Threading::Thread(
						gcnew System::Threading::ThreadStart(this, &MainWindowForm::convertScene));

					// Set the thread name
					backgroundThread->Name = "Scene convert thread";

					// Start the thread in the background
					backgroundThread->Start();
				}
			}
		}
		System::Void m_buttonBrowseImport_Click(System::Object ^sender, System::EventArgs ^e) 
		{
			if(m_openFileDialogImporter->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				m_textBoxPathImport->Text = m_openFileDialogImporter->FileName;

				m_textBoxPathAssets->Text = toString(m_converter->getAssetDirectory(toString(m_openFileDialogImporter->FileName)));

				// Reset some GUI elements
				setProgressBarValue(0);
				setLabelStatusTextSafe("Ready");
			}
		}
		System::Void m_buttonBrowseExport_Click(System::Object ^sender, System::EventArgs ^e) 
		{
			if(m_folderBrowserDialogExport->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				m_textBoxPathExport->Text = m_folderBrowserDialogExport->SelectedPath;
				
				// Reset some GUI elements
				setProgressBarValue(0);
				setLabelStatusTextSafe("Ready");
			}
		}
		System::Void m_checkBoxAssets_CheckedChanged(System::Object ^sender, System::EventArgs ^e) 
		{
			if(m_checkBoxAssets->Checked)
			{
				m_buttonBrowseAssets->Enabled = true;
				m_textBoxPathAssets->Enabled = true;
			}
			else
			{
				m_buttonBrowseAssets->Enabled = false;
				m_textBoxPathAssets->Enabled = false;
			}
		}
		System::Void m_buttonBrowseAssets_Click(System::Object ^sender, System::EventArgs ^e) 
		{			
			if(m_folderBrowserDialogAssets->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				m_textBoxPathAssets->Text = m_folderBrowserDialogAssets->SelectedPath;
				
				// Reset some GUI elements
				setProgressBarValue(0);
				setLabelStatusTextSafe("Ready");
			}
		}
		System::Void m_openFileToolStripMenuItem_Click(System::Object ^sender, System::EventArgs ^e) 
		{
			m_buttonBrowseImport_Click(sender, e);
		}
		System::Void m_saveDirectoryToolStripMenuItem_Click(System::Object ^sender, System::EventArgs ^e) 
		{
			m_buttonBrowseExport_Click(sender, e);
		}
		System::Void m_exitToolStripMenuItem_Click(System::Object ^sender, System::EventArgs ^e) 
		{
			System::Windows::Forms::Application::Exit();
		}
		System::Void m_aboutToolStripMenuItem_Click(System::Object ^sender, System::EventArgs ^e) 
		{
			MessageBox::Show("Praxis3D converter V1.0" + System::Environment::NewLine + 
							"Converts scenes (maps) to Praxis3D scene format." + System::Environment::NewLine +
							"Currently supports conversions from ProjectExodus exported scenes (in JSON format)." + System::Environment::NewLine +
							System::Environment::NewLine +
							"External libraries used:" + System::Environment::NewLine +
							"Open-Asset-Importer-Lib (ASSIMP)" + System::Environment::NewLine +
							"FreeImage" + System::Environment::NewLine +
							"Nlohmann JSON" + System::Environment::NewLine +
							"(see Help->License for more information)." + System::Environment::NewLine +
							System::Environment::NewLine +
							"Created by Paul Akulavicius." + System::Environment::NewLine + 
							"pauldev.org" + System::Environment::NewLine + 
							"2020-11",
							"About this application...");
		}
		System::Void m_helpToolStripMenuItem_Click(System::Object ^sender, System::EventArgs ^e) 
		{

		}
		System::Void MainWindowForm_Load(System::Object ^sender, System::EventArgs ^e) 
		{
			this->Icon = gcnew System::Drawing::Icon(L"praxis_icon.ico");
			this->PerformLayout();
		}
		System::Void m_documentationToolStripMenuItem_Click(System::Object ^sender, System::EventArgs ^e) 
		{
			// Open URL
			System::Diagnostics::Process::Start("https://bitbucket.org/Paul_ak/praxis3d-converter");
		}
		System::Void m_licenseToolStripMenuItem_Click(System::Object ^sender, System::EventArgs ^e) 
		{
			// Declare mandatory variables
			STARTUPINFO si = { 0 };
			PROCESS_INFORMATION pi = { 0 };
			si.cb = sizeof(si);

			// Second argument of CreateProcess function must be writable memory
			LPWSTR command = L"c:\\windows\\system32\\notepad.exe license.txt";

			// Open the license.txt file with Notepad
			CreateProcess(L"c:\\windows\\system32\\notepad.exe",	// Name of program to execute
				command,											// Command line
				NULL,												// Process handle not inheritable
				NULL,												// Thread handle not inheritable
				FALSE,												// Set handle inheritance to FALSE
				0,													// No creation flags
				NULL,												// Use parent's environment block
				NULL,												// Use parent's starting directory 
				&si,												// Pointer to STARTUPINFO structure
				&pi);												// Pointer to PROCESS_INFORMATION structure
		}
};
}
