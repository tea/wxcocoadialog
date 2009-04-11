#include "CocoaDialog.h"
#include "InputBox.h"
#include "ProgressBar.h"
#include "Dropdown.h"
#include "MessageDialog.h"
#ifdef FEAT_BROWSER
#include "HtmlDialog.h"
#endif

#include <stdio.h>

IMPLEMENT_APP_NO_MAIN(CocoaDialogApp)

wxArrayString CocoaDialogApp::utf8_args;

// 'Main program' equivalent: the program execution "starts" here
bool CocoaDialogApp::OnInit()
{	
	if (utf8_args.GetCount() < 2) {
		return OptionError();
	}

	m_parentWnd = NULL;
	// This code is currently disabled, as it will end up
    // setting focus to a random window if parentwnd is busy.
	// http://blogs.msdn.com/oldnewthing/archive/2004/02/27/81155.aspx
#ifdef __WXMSW__
	// Use currently active window as parent
	HWND wnd = ::GetForegroundWindow();
	if (wnd) {
		m_parentWnd = new wxWindow();
		m_parentWnd->AssociateHandle(wnd);
	}
#endif //__WXMSW__
	
	// Parse the options
	const wxString runmode(utf8_args[1]);
	m_optionDict.SetRunmode(runmode);
	if (!m_optionDict.ParseArgs(utf8_args)) return OptionError();

//	m_optionDict.Print();

	//Initilizing image handlers
	//wxImage::AddHandler(new wxBMPHandler);
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxJPEGHandler);
	wxImage::AddHandler(new wxGIFHandler);
	wxImage::AddHandler(new wxICOHandler);

	// Show the Dialog
	if (runmode == wxT("inputbox")) {
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined"));
		
		InputBox* inputBox = new InputBox(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
		SetTopWindow(inputBox);
	}
	else if (runmode == wxT("secure-inputbox")) {
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined"));
		
		m_optionDict.SetOption(wxT("no-show"));
		
		new InputBox(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("standard-inputbox")) {
		m_optionDict.SetOption(wxT("button1"), wxT("Ok"));
		if (!m_optionDict.HasOption(wxT("no-cancel"))) m_optionDict.SetOption(wxT("button2"), wxT("Cancel"));

		new InputBox(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("secure-standard-inputbox")) {
		m_optionDict.SetOption(wxT("button1"), wxT("Ok"));
		if (!m_optionDict.HasOption(wxT("no-cancel"))) m_optionDict.SetOption(wxT("button2"), wxT("Cancel"));

		m_optionDict.SetOption(wxT("no-show"));

		new InputBox(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("progressbar")) {
		new ProgressBar(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("dropdown")) {
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined."));
		if (!m_optionDict.HasMultiOption(wxT("items"))) return OptionError(wxT("Need to define items for dropdown box."));

		new DropdownBox(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("standard-dropdown")) {
		m_optionDict.SetOption(wxT("button1"), wxT("Ok"));
		m_optionDict.SetOption(wxT("button2"), wxT("Cancel"));
		// Verify mandatory options
		if (!m_optionDict.HasMultiOption(wxT("items"))) return OptionError(wxT("Need to define items for dropdown box."));

		new DropdownBox(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("msgbox")) {
		if (m_optionDict.HasOption(wxT("help"))) return OptionHelp(wxT("msgbox"));
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined"));
		
		new MessageDialog(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("ok-msgbox")) {
		if (m_optionDict.HasOption(wxT("help"))) return OptionHelp(wxT("ok-msgbox"));
		m_optionDict.SetOption(wxT("button1"), wxT("Ok"));
		if (!m_optionDict.HasOption(wxT("no-cancel"))) m_optionDict.SetOption(wxT("button2"), wxT("Cancel"));
		
		new MessageDialog(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("yesno-msgbox")) {
		if (m_optionDict.HasOption(wxT("help"))) return OptionHelp(wxT("yesno-msgbox"));
		m_optionDict.SetOption(wxT("button1"), wxT("Yes"));
		m_optionDict.SetOption(wxT("button2"), wxT("No"));
		
		new MessageDialog(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("textbox")) {
		if (m_optionDict.HasOption(wxT("help"))) return OptionHelp(wxT("textbox"));
		// Verify mandatory options
		if (!m_optionDict.HasOption(wxT("button1"))) return OptionError(wxT("At least one button has to be defined"));
		
		new InputBox(m_parentWnd, m_optionDict, m_optionDict.HasOption(wxT("float")));
	}
	else if (runmode == wxT("fileselect") || runmode == wxT("filesave")) {
		ShowFileDialog();
		return false; // end program
	}
	else if (runmode == wxT("colourselect") || runmode == wxT("colorselect")) {
		ShowColourDialog();
		return false; // end program
	}
	else if (runmode == wxT("menu")) {
		ShowPopupMenu();
		return false; // end program
	}
#ifdef FEAT_BROWSER
	else if (runmode == wxT("html")) {
		new HtmlDialog(m_parentWnd, m_optionDict);
	}
#endif
	else return OptionError(wxT("Unknown runmode."));

	wxLogDebug(wxT("wxCD done"));

	return true;
}

int CocoaDialogApp::OnExit() {
#ifdef __WXMSW__
	if (m_parentWnd) {
		// Activate the parent frame
		HWND hwnd = GetHwndOf(m_parentWnd);
		::SetForegroundWindow(hwnd);
		::SetFocus(hwnd);

		m_parentWnd->DissociateHandle();
		delete m_parentWnd;
	}
#endif //__WXMSW__

	wxLogDebug(wxT("wxCD exit done"));

	return wxApp::OnExit();
}

bool CocoaDialogApp::OptionError(const wxString& error) const {
	wxPrintf(wxT("Invalid options:\n\n"));
	
	if (!error.empty()) wxPrintf(wxT("%s"), error.c_str());
	
	wxPrintf(wxT("\n"));
	OptionHelp();

	return false;
}

bool CocoaDialogApp::OptionHelp(const wxString& runmode) const {
	wxPrintf(wxT("HELP!\n\n"));
	if (!runmode.empty())
	{
		if (runmode == wxT("msgbox"))
		{
			wxPrintf(wxT("All available options:\n"));
			wxPrintf(wxT("\t--icon-file\n"));
			wxPrintf(wxT("\t--title\n"));
			wxPrintf(wxT("\t--button2\n"));
			wxPrintf(wxT("\t--string-output\n"));
			wxPrintf(wxT("\t--help\n"));
			wxPrintf(wxT("\t--debug\n"));
			wxPrintf(wxT("\t--icon\n"));
			wxPrintf(wxT("\t...\n"));
		}
		else if (runmode == wxT("ok-msgbox"))
		{
			wxPrintf(wxT("All available options:\n"));
			wxPrintf(wxT("\t--icon-file\n"));
			wxPrintf(wxT("\t--title\n"));
			wxPrintf(wxT("\t--button2\n"));
			wxPrintf(wxT("\t--string-output\n"));
			wxPrintf(wxT("\t--help\n"));
			wxPrintf(wxT("\t--debug\n"));
			wxPrintf(wxT("\t--icon\n"));
			wxPrintf(wxT("\t...\n"));
		}
		else if (runmode == wxT("yesno-msgbox"))
		{
			wxPrintf(wxT("All available options:\n"));
			wxPrintf(wxT("\t--icon-file\n"));
			wxPrintf(wxT("\t--title\n"));
			wxPrintf(wxT("\t--button2\n"));
			wxPrintf(wxT("\t--string-output\n"));
			wxPrintf(wxT("\t--help\n"));
			wxPrintf(wxT("\t--debug\n"));
			wxPrintf(wxT("\t--icon\n"));
			wxPrintf(wxT("\t...\n"));
		}
		else if (runmode == wxT("textbox"))
		{
			wxPrintf(wxT("\t--text\n"));
			wxPrintf(wxT("\t--text-from-file\n"));
			wxPrintf(wxT("\t--informative-text\n"));
			wxPrintf(wxT("\t--button1\n"));
			wxPrintf(wxT("\t--button2\n"));
			wxPrintf(wxT("\t--button3\n"));
			wxPrintf(wxT("\t--editable\n"));
			wxPrintf(wxT("\t--focus-textbox\n"));
			wxPrintf(wxT("\t--selected\n"));
			wxPrintf(wxT("\t--scroll-to\n"));
			wxPrintf(wxT("\t--float\n"));
			wxPrintf(wxT("\t--timeout\n"));
		}
	}

	wxPrintf(wxT("Usage: Cocoadialog type [options]\n"));
	wxPrintf(wxT("\tAvailable types:\n"));
	wxPrintf(wxT("\t\tinputbox, secure-inputbox, standard-inputbox, secure-standard-inputbox\n"));
	wxPrintf(wxT("\t\tprogressbar, dropdown, standard-dropdown, msgbox\n"));
	wxPrintf(wxT("\tGlobal Options:\n"));
	wxPrintf(wxT("\t\t--help, --debug, --title, --width, --height\n"));
	wxPrintf(wxT("\t\t--string-output, --no-newline\n\n"));
//	wxPrintf(wxT("See ... for detailed documentation\n"));

	return false;
}


int main(int argc, char* argv[]) {
#ifdef _WIN32
	// To be compatible with cygwin, we want to avoid lf->crlf conversion
	_setmode( _fileno( stdout ), _O_BINARY );
#endif

	// We want to keep the ansi versions for the arguments so
	// that we can convert them from utf8.
	for (int i = 0; i < argc; ++i) {
		CocoaDialogApp::utf8_args.Add(wxString(argv[i], wxConvUTF8));
	}

	// Start by initializing wxWidgets.
	// this is needed as we do not have a main wxApp class.
	wxEntry(argc, argv);
	
	return 0;
}

