#include "CocoaDialog.h"
#include "InputBox.h"
#include "ProgressBar.h"
#include "Dropdown.h"
#include "MessageDialog.h"

IMPLEMENT_APP_NO_MAIN(CocoaDialogApp)

wxArrayString CocoaDialogApp::utf8_args;

// 'Main program' equivalent: the program execution "starts" here
bool CocoaDialogApp::OnInit()
{	
	if (utf8_args.GetCount() < 2) {
		return OptionError();
	}

	m_parentWnd = NULL;
/*  This code is currently disabled, as it will end up
    setting focus to a random window if parentwnd is busy.
	http://blogs.msdn.com/oldnewthing/archive/2004/02/27/81155.aspx
#ifdef __WXMSW__
	// Use currently active window as parent
	HWND wnd = ::GetForegroundWindow();
	if (wnd) {
		m_parentWnd = new wxWindow();
		m_parentWnd->AssociateHandle(wnd);
	}
#endif //__WXMSW__*/
	
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
	else return OptionError(wxT("Unknown runmode."));

	wxLogDebug(wxT("wxCD done"));

	return true;
}

int CocoaDialogApp::OnExit() {
	if (m_parentWnd) {
		m_parentWnd->DissociateHandle();
		delete m_parentWnd;
	}
	wxLogDebug(wxT("wxCD exit done"));

	return wxApp::OnExit();
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


main(int argc, char* argv[]) {
	// To be compatible with cygwin, we want to avoid lf->crlf conversion
	_setmode( _fileno( stdout ), _O_BINARY );

	// We want to keep the ansi versions for the arguments so
	// that we can convert them from utf8.
	for (int i = 0; i < argc; ++i) {
		CocoaDialogApp::utf8_args.Add(wxString(argv[i], wxConvUTF8));
	}

	// Start by initializing wxWidgets.
	// this is needed as we do not have a main wxApp class.
	wxEntry(::GetModuleHandle(NULL));
	
	return 0;
}

#ifdef __WXMSW__

wxString CocoaDialogApp::WinPathToCygwin(const wxFileName& path) { // static
	wxASSERT(path.IsOk() && path.IsAbsolute());

	// Drive
	wxString unixPath = wxT("/cygdrive/");
	unixPath += path.GetVolume().Lower();

	// Dirs
	const wxArrayString& dirs = path.GetDirs();
	for (unsigned int i = 0; i < dirs.GetCount(); ++i) {
		unixPath += wxT('/') + dirs[i];
	}

	// Filename
	if (path.HasName()) {
		unixPath += wxT('/') + path.GetFullName();
	}

	return unixPath;
}

wxString CocoaDialogApp::CygwinPathToWin(const wxString& path) { // static
	if (path.empty()) {
		wxASSERT(false);
		return wxEmptyString;
	}
	wxString newpath;

	if (path.StartsWith(wxT("/cygdrive/"))) {

		// Get drive letter
		const wxChar drive = wxToupper(path[10]);
		if (drive < wxT('A') || drive > wxT('Z')) {
			wxASSERT(false);
			return wxEmptyString;
		}

		// Build new path
		newpath += drive;
		newpath += wxT(':');
		if (path.size() > 11) newpath += path.substr(11);
		else newpath += wxT('\\');
	}
	else if (path.StartsWith(wxT("/usr/bin/"))) {
		newpath = GetCygwinDir() + wxT("\\bin\\");
		newpath += path.substr(9);
	}
	else if (path.GetChar(0) == wxT('/')) {
		newpath = GetCygwinDir();
		newpath += path;
	}
	else return path; // no conversion

	// Convert path seperators
	for (unsigned int i = 0; i < newpath.size(); ++i) {
		if (newpath[i] == wxT('/')) newpath[i] = wxT('\\');
	}

	return newpath;
}

#include <wx/msw/registry.h>

wxString CocoaDialogApp::GetCygwinDir() { // static
	wxString cygPath;

	// Check if we have a cygwin installation
	wxRegKey cygKey(wxT("HKEY_LOCAL_MACHINE\\SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2\\/"));
	if( cygKey.Exists() ) {
		if (cygKey.HasValue(wxT("native"))) {
			cygKey.QueryValue(wxT("native"), cygPath);
		}
	}

	// Also check "current user" (might be needed if user did not have admin rights during install)
	if (cygPath.empty()) {
		wxRegKey cygKey2(wxT("HKEY_CURRENT_USER\\SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2\\/"));
		if( cygKey2.Exists() ) {
			if (cygKey2.HasValue(wxT("native"))) {
				cygKey2.QueryValue(wxT("native"), cygPath);
			}
		}
	}

	return cygPath;
}

#endif // __WXMSW__
