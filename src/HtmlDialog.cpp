#include "HtmlDialog.h"
#include <wx/filename.h>
#include <wx/ffile.h>
#include "Util.h"

#if defined(__WXMSW__)
	#include "IEHtmlWin.h"
#elif defined(__WXGTK__)
	#include "WebKitHtmlWnd.h"
#endif

// control id's
enum
{
	ID_MSHTML
};

BEGIN_EVENT_TABLE(HtmlDialog, wxDialog)
	EVT_CLOSE(HtmlDialog::OnClose)
	EVT_HTMLWND_BEFORE_LOAD(ID_MSHTML, HtmlDialog::OnBeforeLoad)
END_EVENT_TABLE()

HtmlDialog::HtmlDialog(wxWindow* parent, const OptionDict& options) 
: wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER),
  m_optionDict(options) {
	// Set the dialog title
	const wxString title = options.GetOption(wxT("title"));
	SetTitle(options.GetOption(wxT("title")));

#if defined (__WXMSW__)
	// IE Control
	m_browser = new wxIEHtmlWin(this, ID_MSHTML);
#elif defined (__WXGTK__)
	// WebKit control
	m_browser = new wxBrowser(this, ID_MSHTML);
#endif

	// Load html from file
	if (options.HasOption(wxT("html-from-file")))
	{
		wxString htmlFile = options.GetOption(wxT("html-from-file"));
#ifdef __WXMSW__
		if (!htmlFile.empty()) {
			htmlFile = CygwinPathToWin(htmlFile);
		}
#endif // __WXMSW__
		
		// We need an absolute path
		wxFileName path(htmlFile);
		path.MakeAbsolute();

		m_browser->LoadUrl(path.GetFullPath());
	}
	else { // Load html for stdin
		// Create temp file
		m_tempPath = wxFileName::CreateTempFileName(wxEmptyString) + wxT(".html");
		wxFFile tempFile(m_tempPath, wxT("wb"));
		if (!tempFile.IsOpened()) {
			Close();
			return;
		}
		
		// Load html from stdin to temp file
		int c;
		FILE* fp = tempFile.fp();
		while ((c = getc(stdin)) != EOF) putc(c, fp);
		tempFile.Close();

		m_browser->LoadUrl(m_tempPath);
	}

	// Create layout
	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(m_browser->GetWindow(), 1, wxEXPAND);
	SetSizer(mainSizer);

	int width;
	int height;
	if (options.GetIntegerOption(wxT("width"), width) &&
		options.GetIntegerOption(wxT("height"), height)) {
		SetSize(width, height);
	}

	Centre();
	Show();
}

void HtmlDialog::OnClose(wxCloseEvent& WXUNUSED(event)) {
	// Clean up
	if (!m_tempPath.empty() && wxFileExists(m_tempPath)) {
		wxRemoveFile(m_tempPath);
	}
	
	// If we have a parent, we want to pass focus to it before closing
	// (otherwise the system may activate a random window)
	wxWindow* parent = GetParent();
	if (parent) {
#ifdef __WXMSW__
		// Activate the parent frame
		HWND hwnd = GetHwndOf(parent);
		::SetForegroundWindow(hwnd);
		::SetFocus(hwnd);
#endif //__WXMSW__
	}
	
	Destroy(); // Dlg is top window, so this ends the app.
}

void HtmlDialog::OnBeforeLoad(IHtmlWndBeforeLoadEvent& event) {
 	const wxString url = event.GetURL();
	if (url == wxT("about:blank"))
		return;

	if (url.StartsWith(wxT("cocoadialog://"))) {
		wxString output = URLDecode(url.substr(14));

		// Remove trailing '/' (added by ie)
		if (output.EndsWith(wxT("/"))) output.RemoveLast();

		printf("%s", output.utf8_str().data());
		if (!m_optionDict.HasOption(wxT("no-newline"))) printf("\n");

		// Don't try to open it in browser
		event.Cancel(true);

		Close();
	}
}

// URL Decode a string.
wxString HtmlDialog::URLDecode(const wxString &value) { // static
	wxString szDecoded;
	wxString szEncoded = value;

	unsigned int nEncodedPos = 0;

	// eliminiate + and replace with spaces...
	szEncoded.Replace(wxT("+"), wxT(" "));

	while( nEncodedPos < szEncoded.length() ) {
		if(szEncoded.GetChar(nEncodedPos) == wxT('%')) {
			nEncodedPos++;
			if( isxdigit(szEncoded.GetChar(nEncodedPos)) && isxdigit(szEncoded.GetChar(nEncodedPos+1)) ) {
				wxString szIntFromHex;
				szIntFromHex.Append( szEncoded.GetChar(nEncodedPos) );
				szIntFromHex.Append( szEncoded.GetChar(nEncodedPos+1) );
				szDecoded.Append( (wxChar) ParseHex(szIntFromHex) );
				nEncodedPos += 2;
			}
		} 
		else {
			szDecoded.Append(  szEncoded.GetChar(nEncodedPos++) );
		}
	}

	return szDecoded;
}

int HtmlDialog::ParseHex(const wxString& hexStr) { // static
	wxASSERT(hexStr.size() == 2);

	const int n1 = HexToNumber(hexStr[0]);
	const int n2 = HexToNumber(hexStr[1]);

	return ((n1 << 4) | n2);
}

int HtmlDialog::HexToNumber(char hex) { // static
	if (hex >= '0' && hex <= '9') return hex - '0';
	if (hex >= 'a' && hex <= 'f') return hex - 'a' + 10;

	return -1; // invalid hex character
}
