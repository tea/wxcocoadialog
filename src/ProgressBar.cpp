#include "ProgressBar.h"
#include <wx/file.h>

#define LINEBUFSIZE 250

// Ctrl id's
enum {
	LINEREADER
};

BEGIN_EVENT_TABLE(ProgressBar, wxDialog)
	EVT_CLOSE(ProgressBar::OnClose)
	EVT_TEXT(LINEREADER, ProgressBar::OnLineRead)
	EVT_BUTTON(wxID_CANCEL, ProgressBar::OnCancel)
END_EVENT_TABLE()

ProgressBar::ProgressBar(wxWindow* parent, const OptionDict& options, bool doFloat) 
: wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, doFloat ? wxCAPTION|wxSTAY_ON_TOP : wxCAPTION),
  m_options(options) {
	// Set the dialog title
	const wxString title = options.GetOption(wxT("title"));
	SetTitle(options.GetOption(wxT("title")));

	// Create controls
	m_infoText = new wxStaticText(this, wxID_ANY, options.GetOption(wxT("text")));
	m_progressBar = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH);

	// Create layout
	m_mainSizer = new wxBoxSizer(wxVERTICAL);
		wxSizer* spacer = new wxBoxSizer(wxVERTICAL);
			spacer->Add(m_infoText, 0, wxALL, 5);
			spacer->Add(m_progressBar, 0, wxEXPAND|wxALL, 5);
			m_mainSizer->Add(spacer, 0, wxEXPAND|wxALL, 5);

	// Set initial position
	int percent;
	if (options.GetIntegerOption(wxT("percent"), percent)) {
		m_progressBar->SetValue(percent);
	}

	SetSizerAndFit(m_mainSizer);

	// Make sure dialog does not get too small
	const wxSize size = GetSize();
	if (size.x < 300) SetSize(300, -1);

	Centre();
	Show();

	// Start a thread to wait for input on stdin
	// It will delete itself when getting EOF
	// Note that the dialog does not have any close button
	// this is because the thread blocks on stdin
	// so we cannot just kill it.
	new LineReaderThread(*this);
}

void ProgressBar::OnClose(wxCloseEvent& WXUNUSED(event)) {
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

void ProgressBar::OnLineRead(wxCommandEvent& event) {
	const wxString& line = event.GetString();

	// Update progress
	long percent = -1;
	line.ToLong(&percent);
	if (percent != -1) { // ToLong returns false if string is longer than number
		m_progressBar->SetValue(percent);
	}

	// Update info text
	const wxString text = line.AfterFirst(wxT(' '));
	if (!text.empty()) m_infoText->SetLabel(text);
	
	m_mainSizer->Layout();
	Fit();

	// Make sure dialog does not get too small
	const wxSize size = GetSize();
	if (size.x < 300) SetSize(300, -1);
}

void ProgressBar::OnCancel(wxCommandEvent& WXUNUSED(event)) {
	Close();
}

// ---- LineReaderThread -----------------------------------------------

ProgressBar::LineReaderThread::LineReaderThread(wxEvtHandler& evtHandler)
: m_evtHandler(evtHandler) {
	// Start the thread
	Create();
	Run();
}

void* ProgressBar::LineReaderThread::Entry() {
	char linebuf[LINEBUFSIZE];

	while (fgets(linebuf, LINEBUFSIZE, stdin) != NULL) {
		wxString line(linebuf, wxConvUTF8);
		line.RemoveLast(); // remove trailing newline
			
		// Notify parent that we have a new line
		wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, LINEREADER);
		event.SetString(line);
		m_evtHandler.AddPendingEvent(event);
	}
	
	// Notify parent that that there is no more input
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, wxID_CANCEL);
	m_evtHandler.AddPendingEvent(event);

	return NULL;
}

