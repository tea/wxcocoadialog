#include "CocoaDialog.h"
#include "InputBox.h"
#include "ProgressBar.h"
#include "Dropdown.h"
#include "MessageDialog.h"

IMPLEMENT_APP_NO_MAIN(CocoaDialogApp)

// 'Main program' equivalent: the program execution "starts" here
bool CocoaDialogApp::OnInit()
{	
	if (argc < 2) {
		return OptionError();
	}
	
	// Parse the options
	const wxString runmode(argv[1], wxConvUTF8);
	m_optionDict.SetRunmode(runmode);
	if (!m_optionDict.ParseArgs(argc, argv)) return OptionError();

//	m_optionDict.Print();

	//Initilizing image handlers
	wxImage::AddHandler(new wxBMPHandler);
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxJPEGHandler);
	wxImage::AddHandler(new wxGIFHandler);
	wxImage::AddHandler(new wxICOHandler);
//	wxInitAllImageHandlers();

	// Show the Dialog
	if (runmode == wxT("inputbox")) {
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined"));
		
		InputBox* dlg = new InputBox(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("secure-inputbox")) {
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined"));
		
		m_optionDict.SetOption(wxT("no-show"));
		
		InputBox* dlg = new InputBox(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("standard-inputbox")) {
		m_optionDict.SetOption(wxT("button1"), wxT("Ok"));
		if (!m_optionDict.HasOption(wxT("no-cancel"))) m_optionDict.SetOption(wxT("button2"), wxT("Cancel"));

		InputBox* dlg = new InputBox(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("secure-standard-inputbox")) {
		m_optionDict.SetOption(wxT("button1"), wxT("Ok"));
		if (!m_optionDict.HasOption(wxT("no-cancel"))) m_optionDict.SetOption(wxT("button2"), wxT("Cancel"));

		m_optionDict.SetOption(wxT("no-show"));

		InputBox* dlg = new InputBox(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("progressbar")) {
		ProgressBar* dlg = new ProgressBar(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("dropdown")) {
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined."));
		if (!m_optionDict.HasMultiOption(wxT("items"))) return OptionError(wxT("Need to define items for dropdown box."));

		DropdownBox* dlg = new DropdownBox(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("standard-dropdown")) {
		m_optionDict.SetOption(wxT("button1"), wxT("Ok"));
		m_optionDict.SetOption(wxT("button2"), wxT("Cancel"));
		// Verify mandatory options
		if (!m_optionDict.HasMultiOption(wxT("items"))) return OptionError(wxT("Need to define items for dropdown box."));

		DropdownBox* dlg = new DropdownBox(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	
	}
	else if (runmode == wxT("msgbox")) {
		if (m_optionDict.HasOption(wxT("help"))) return OptionHelp(wxT("msgbox"));
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined"));
		
		MessageDialog* dlg = new MessageDialog(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("ok-msgbox")) {
		if (m_optionDict.HasOption(wxT("help"))) return OptionHelp(wxT("ok-msgbox"));
		m_optionDict.SetOption(wxT("button1"), wxT("Ok"));
		if (!m_optionDict.HasOption(wxT("no-cancel"))) m_optionDict.SetOption(wxT("button2"), wxT("Cancel"));
		
		MessageDialog* dlg = new MessageDialog(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("yesno-msgbox")) {
		if (m_optionDict.HasOption(wxT("help"))) return OptionHelp(wxT("yesno-msgbox"));
		m_optionDict.SetOption(wxT("button1"), wxT("Yes"));
		m_optionDict.SetOption(wxT("button2"), wxT("No"));
		
		MessageDialog* dlg = new MessageDialog(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else if (runmode == wxT("textbox")) {
		if (m_optionDict.HasOption(wxT("help"))) return OptionHelp(wxT("textbox"));
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined"));
		
		InputBox* dlg = new InputBox(m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(dlg);
	}
	else return OptionError(wxT("Unknown runmode."));

	return true;
}

bool CocoaDialogApp::OptionError(const wxString& error) const {
	printf("Invalid options:\n\n");
	
	if (!error.empty()) printf(error.mb_str(wxConvUTF8));
	
	printf("\n");
	OptionHelp();

	return false;
}

bool CocoaDialogApp::OptionHelp(const wxString& runmode) const {
	printf("HELP!\n\n");
	if (!runmode.empty())
	{
		if (runmode == wxT("msgbox"))
		{
			printf("All available options:\n");
			printf("\t--icon-file\n");
			printf("\t--title\n");
			printf("\t--button2\n");
			printf("\t--string-output\n");
			printf("\t--help\n");
			printf("\t--debug\n");
			printf("\t--icon\n");
			printf("\t...\n");
		}
		else if (runmode == wxT("ok-msgbox"))
		{
			printf("All available options:\n");
			printf("\t--icon-file\n");
			printf("\t--title\n");
			printf("\t--button2\n");
			printf("\t--string-output\n");
			printf("\t--help\n");
			printf("\t--debug\n");
			printf("\t--icon\n");
			printf("\t...\n");
		}
		else if (runmode == wxT("yesno-msgbox"))
		{
			printf("All available options:\n");
			printf("\t--icon-file\n");
			printf("\t--title\n");
			printf("\t--button2\n");
			printf("\t--string-output\n");
			printf("\t--help\n");
			printf("\t--debug\n");
			printf("\t--icon\n");
			printf("\t...\n");
		}
		else if (runmode == wxT("textbox"))
		{
			printf("\t--text\n");
			printf("\t--text-from-file\n");
			printf("\t--informative-text\n");
			printf("\t--button1\n");
			printf("\t--button2\n");
			printf("\t--button3\n");
			printf("\t--editable\n");
			printf("\t--focus-textbox\n");
			printf("\t--selected\n");
			printf("\t--scroll-to\n");
			printf("\t--float\n");
			printf("\t--timeout\n");
		}
	}

	printf("Usage: Cocoadialog type [options]\n");
	printf("\tAvailable types:\n");
	printf("\t\tinputbox, secure-inputbox, standard-inputbox, secure-standard-inputbox\n");
	printf("\t\tprogressbar, dropdown, standard-dropdown, msgbox\n");
	printf("\tGlobal Options:\n");
	printf("\t\t--help, --debug, --title, --width, --height\n");
	printf("\t\t--string-output, --no-newline\n\n");
//	printf("See ... for detailed documentation\n");

	return false;
}


main() {
	// Start by initializing wxWidgets.
	// this is needed as we do not have a main wxApp class.
	wxEntry(::GetModuleHandle(NULL));
	
	return 0;
}
