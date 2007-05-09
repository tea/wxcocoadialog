#include "InputBox.h"

// Ctrl id's
enum {
	CTRL_BUTTON1,
	CTRL_BUTTON2,
	CTRL_BUTTON3,
	TIMEOUT_TIMER
};

BEGIN_EVENT_TABLE(InputBox, wxDialog)
	EVT_CLOSE(InputBox::OnClose)
	EVT_BUTTON(CTRL_BUTTON1, InputBox::OnButton1)
	EVT_BUTTON(CTRL_BUTTON2, InputBox::OnButton2)
	EVT_BUTTON(wxID_CANCEL, InputBox::OnButton2)
	EVT_BUTTON(CTRL_BUTTON3, InputBox::OnButton3)
	EVT_TIMER(TIMEOUT_TIMER, InputBox::OnTimeout)
END_EVENT_TABLE()

InputBox::InputBox(wxWindow* parent, const OptionDict& options, bool doFloat) 
: wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, doFloat ? wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP : wxDEFAULT_DIALOG_STYLE), m_options(options) {
	// Set the dialog title
	const wxString title = options.GetOption(wxT("title"));
	SetTitle(options.GetOption(wxT("title")));

	wxSize textSize = wxDefaultSize;

	int textStyle = 0;
	if (options.HasOption(wxT("no-show"))) textStyle |= wxTE_PASSWORD;

	//Set textStyle to multiline, in case runmode is textbox
	const wxString runmode = options.GetRunmode();
	if (runmode == wxT("textbox")) 
	{
		textSize = wxSize(-1,250);
		textStyle |= wxTE_MULTILINE|wxTE_READONLY;
	}
	
	// Create controls
	wxStaticText* informativeText = new wxStaticText(this, wxID_ANY, options.GetOption(wxT("informative-text")));
	m_inputBox = new wxTextCtrl(this, wxID_ANY, options.GetOption(wxT("text")), wxDefaultPosition, textSize, textStyle);

	//Set text selected
	m_inputBox->SetSelection(5,7);

	//Set cursor at last position
	if (options.GetOption(wxT("scroll-to")) == wxT("bottom")) m_inputBox->ShowPosition(m_inputBox->GetLastPosition());
	
	//Set text editable
	if (options.HasOption(wxT("editable"))) m_inputBox->SetEditable(true);

	//Load text from file
	if (options.HasOption(wxT("text-from-file")))
	{
		const wxString textFromFile = options.GetOption(wxT("text-from-file"));
		m_inputBox->LoadFile(textFromFile);
	}

	m_inputBox->SetInsertionPoint(0);

	m_button3 = new wxButton(this, CTRL_BUTTON3, options.GetOption(wxT("button3")));
	m_button2 = new wxButton(this, CTRL_BUTTON2, options.GetOption(wxT("button2")));
	m_button1 = new wxButton(this, CTRL_BUTTON1, options.GetOption(wxT("button1")));
	SetDefaultItem(m_button1);

	// Create layout
	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
		mainSizer->Add(informativeText, 0, wxALL, 5);
		mainSizer->Add(m_inputBox, 0, wxEXPAND|wxALL, 5);
		wxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
			buttonSizer->Add(m_button3, 0, wxALL, 5);
			buttonSizer->Add(m_button2, 0, wxALL, 5);
			buttonSizer->Add(m_button1, 0, wxALL, 5);
			mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	// Hide unneeded controls
	if (!options.HasOption(wxT("informative-text"))) mainSizer->Hide(informativeText);
	if (!options.HasOption(wxT("button2"))) buttonSizer->Hide(m_button2);
	if (!options.HasOption(wxT("button3"))) buttonSizer->Hide(m_button3);

	// Set timeout timer
	int interval;
	if (options.GetIntegerOption(wxT("timeout"), interval)) {
		m_timer.SetOwner(this, TIMEOUT_TIMER);
		m_timer.Start(interval*1000, true);
	}
	
	SetSizerAndFit(mainSizer);
	
	// Make sure dialog does not get too small
	const wxSize size = GetSize();
	if (size.x < 300) SetSize(300, -1);

	Centre();
	Show();
}

void InputBox::OnClose(wxCloseEvent& WXUNUSED(event)) {
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

void InputBox::OnButton1(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s\n", m_button1->GetLabel().mb_str(wxConvUTF8));
	else printf("1\n");

	printf(m_inputBox->GetValue().mb_str(wxConvUTF8));
	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	
	Close(); // Dlg is top window, so this ends the app.
}

void InputBox::OnButton2(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s\n", m_button3->GetLabel().mb_str(wxConvUTF8));
	else printf("2\n");

	printf(m_inputBox->GetValue().mb_str(wxConvUTF8));
	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	
	Close(); // Dlg is top window, so this ends the app.
}

void InputBox::OnButton3(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s\n", m_button3->GetLabel().mb_str(wxConvUTF8));
	else printf("3\n");

	printf(m_inputBox->GetValue().mb_str(wxConvUTF8));

	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	Close(); // Dlg is top window, so this ends the app.
}

void InputBox::OnTimeout(wxTimerEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("timeout");
	else printf("0");

	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	Close(); // Dlg is top window, so this ends the app.
}