#include "Dropdown.h"
#include "CloseOnEscHandler.h"

// Ctrl id's
enum {
	CTRL_DROPDOWN,
	CTRL_BUTTON1,
	CTRL_BUTTON2,
	CTRL_BUTTON3,
	TIMEOUT_TIMER
};

BEGIN_EVENT_TABLE(DropdownBox, wxDialog)
	EVT_CLOSE(DropdownBox::OnClose)
	EVT_CHOICE(CTRL_DROPDOWN, DropdownBox::OnSelection)
	EVT_BUTTON(CTRL_BUTTON1, DropdownBox::OnButton1)
	EVT_BUTTON(CTRL_BUTTON2, DropdownBox::OnButton2)
	EVT_BUTTON(wxID_CANCEL, DropdownBox::OnButton2)
	EVT_BUTTON(CTRL_BUTTON3, DropdownBox::OnButton3)
	EVT_TIMER(TIMEOUT_TIMER, DropdownBox::OnTimeout)
END_EVENT_TABLE()

DropdownBox::DropdownBox(wxWindow* parent, const OptionDict& options, bool doFloat) 
: wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, doFloat ? wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP : wxDEFAULT_DIALOG_STYLE), m_options(options) {
	// Set the dialog title
	const wxString title = options.GetOption(wxT("title"));
	SetTitle(options.GetOption(wxT("title")));

	int textStyle = 0;
	if (options.HasOption(wxT("no-show"))) textStyle |= wxTE_PASSWORD;
	
	// Get dropdown contents
	options.GetMultiOption(wxT("items"), m_items);
	
	// Create controls
	wxStaticText* informativeText = new wxStaticText(this, wxID_ANY, options.GetOption(wxT("text")));
	m_dropdownBox = new wxChoice(this, CTRL_DROPDOWN, wxDefaultPosition, wxDefaultSize, m_items);
	m_button3 = new wxButton(this, CTRL_BUTTON3, options.GetOption(wxT("button3")));
	m_button2 = new wxButton(this, CTRL_BUTTON2, options.GetOption(wxT("button2")));
	m_button1 = new wxButton(this, CTRL_BUTTON1, options.GetOption(wxT("button1")));
	SetDefaultItem(m_button1);

	// Initial state
	m_dropdownBox->SetSelection(0);
	m_dropdownBox->PushEventHandler(new CloseOnEscHandler(this));
	
	// Create layout
	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
		mainSizer->Add(informativeText, 0, wxALL, 5);
		mainSizer->Add(m_dropdownBox, 0, wxEXPAND|wxALL, 5);
		wxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
			buttonSizer->Add(m_button3, 0, wxALL, 5);
			buttonSizer->Add(m_button2, 0, wxALL, 5);
			buttonSizer->Add(m_button1, 0, wxALL, 5);
			mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	// Hide unneeded controls
	if (!options.HasOption(wxT("text"))) mainSizer->Hide(informativeText);
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

DropdownBox::~DropdownBox(){
	m_dropdownBox->PopEventHandler(true);
}

void DropdownBox::PrintValue() const {
	const int ndx = m_dropdownBox->GetSelection();

	if (m_options.HasOption(wxT("string-output"))) {
		printf("%s", m_items[ndx].mb_str(wxConvUTF8));
	}
	else printf("%d", ndx);

	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
}

void DropdownBox::OnClose(wxCloseEvent& WXUNUSED(event)) {
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

void DropdownBox::OnSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("exit-onchange"))) {
		printf("4\n");

		PrintValue();

		Close(); // Dlg is top window, so this ends the app.
	}
}

void DropdownBox::OnButton1(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s\n", m_button1->GetLabel().mb_str(wxConvUTF8));
	else printf("1\n");

	PrintValue();
	
	Close(); // Dlg is top window, so this ends the app.
}

void DropdownBox::OnButton2(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s\n", m_button2->GetLabel().mb_str(wxConvUTF8));
	else printf("2\n");

	PrintValue();
	
	Close(); // Dlg is top window, so this ends the app.
}

void DropdownBox::OnButton3(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s\n", m_button3->GetLabel().mb_str(wxConvUTF8));
	else printf("3\n");

	PrintValue();
	
	Close(); // Dlg is top window, so this ends the app.
}

void DropdownBox::OnTimeout(wxTimerEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("timeout");
	else printf("0");

	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	Close(); // Dlg is top window, so this ends the app.
}