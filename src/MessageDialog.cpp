#include "MessageDialog.h"
#include "images/atom.xpm"
#include "images/cocodialog.xpm"
#include "images/computer.xpm"
#include "images/document.xpm"
#include "images/find.xpm"
#include "images/finder.xpm"
#include "images/firewire.xpm"
#include "images/folder.xpm"
#include "images/gear.xpm"
#include "images/globe.xpm"
#include "images/hazard.xpm"
#include "images/hourglass.xpm"
#include "images/info.xpm"
#include "images/ipod.xpm"
#include "images/person.xpm"
#include "images/sound.xpm"
#include "images/test.xpm"
#include "images/x.xpm"


// Ctrl id's
enum {
	CTRL_BUTTON1,
	CTRL_BUTTON2,
	CTRL_BUTTON3,
	TIMEOUT_TIMER
};

BEGIN_EVENT_TABLE(MessageDialog, wxDialog)
	EVT_BUTTON(CTRL_BUTTON1, MessageDialog::OnButton1)
	EVT_BUTTON(CTRL_BUTTON2, MessageDialog::OnButton2)
	EVT_BUTTON(wxID_CANCEL, MessageDialog::OnButton2)
	EVT_BUTTON(CTRL_BUTTON3, MessageDialog::OnButton3)
	EVT_TIMER(TIMEOUT_TIMER, MessageDialog::OnTimeout)
END_EVENT_TABLE()


MessageDialog::MessageDialog(const OptionDict& options, bool doFloat) 
: wxDialog(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, doFloat ? wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP : wxDEFAULT_DIALOG_STYLE), m_options(options) {
	// Set the dialog title
	const wxString title = options.GetOption(wxT("title"));
	SetTitle(options.GetOption(wxT("title")));

	int textStyle = 0;
	if (options.HasOption(wxT("no-show"))) textStyle |= wxTE_PASSWORD;

    wxBitmap bmp;
	if (options.HasOption(wxT("icon")))
	{
		const wxString icon = options.GetOption(wxT("icon"));
		if (icon == wxT("atom")) bmp = wxBitmap(atom_xpm);
		else if (icon == wxT("cocodialog")) bmp = wxBitmap(cocodialog_xpm);
		else if (icon == wxT("computer")) bmp = wxBitmap(computer_xpm);
		else if (icon == wxT("document")) bmp = wxBitmap(document_xpm);
		else if (icon == wxT("find")) bmp = wxBitmap(find_xpm);
		else if (icon == wxT("finder")) bmp = wxBitmap(finder_xpm);
		else if (icon == wxT("firewire")) bmp = wxBitmap(firewire_xpm);
		else if (icon == wxT("folder")) bmp = wxBitmap(folder_xpm);
		else if (icon == wxT("gear")) bmp = wxBitmap(gear_xpm);
		else if (icon == wxT("globe")) bmp = wxBitmap(globe_xpm);
		else if (icon == wxT("hazard")) bmp = wxBitmap(hazard_xpm);
		else if (icon == wxT("hourglass")) bmp = wxBitmap(hourglass_xpm);
		else if (icon == wxT("info")) bmp = wxBitmap(info_xpm);
		else if (icon == wxT("ipod")) bmp = wxBitmap(ipod_xpm);
		else if (icon == wxT("person")) bmp = wxBitmap(person_xpm);
		else if (icon == wxT("sound")) bmp = wxBitmap(sound_xpm);
		else if (icon == wxT("test")) bmp = wxBitmap(test_xpm);
		else if (icon == wxT("x")) bmp = wxBitmap(x_xpm);
	}
	else if (options.HasOption(wxT("icon-file"))){
		const wxString imageFile = options.GetOption(wxT("icon-file"));
		//load image
		wxImage image(imageFile);
		bmp = wxBitmap(image);
		if (!bmp.IsOk())
		{
			printf("The image could not be loaded...\n");
		}
	}



	// Create controls
	wxStaticText* informativeText = new wxStaticText(this, wxID_ANY, options.GetOption(wxT("informative-text")));
	wxStaticText* mainText = new wxStaticText(this, wxID_ANY, options.GetOption(wxT("text")));
	wxFont font = mainText->GetFont();
	font.SetWeight(wxFONTWEIGHT_BOLD);
	mainText->SetFont(font);

	wxStaticBitmap* staticBitmap = new wxStaticBitmap(this, wxID_ANY, bmp);

	m_button3 = new wxButton(this, CTRL_BUTTON3, options.GetOption(wxT("button3")));
	m_button2 = new wxButton(this, CTRL_BUTTON2, options.GetOption(wxT("button2")));
	m_button1 = new wxButton(this, CTRL_BUTTON1, options.GetOption(wxT("button1")));
	SetDefaultItem(m_button1);

	// Create layout
	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
		mainSizer->Add(mainText, 0, wxALIGN_CENTER|wxALL, 5);
		wxSizer* textSizer = new wxBoxSizer(wxHORIZONTAL);
			textSizer->Add(staticBitmap, 0, wxALL, 5);
			textSizer->Add(informativeText, 0, wxALL, 5);
			mainSizer->Add(textSizer, 0, wxALL, 5);
		wxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
			buttonSizer->Add(m_button3, 0, wxALL, 5);
			buttonSizer->Add(m_button2, 0, wxALL, 5);
			buttonSizer->Add(m_button1, 0, wxALL, 5);
			mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

	// Hide unneeded controls
	if (!options.HasOption(wxT("icon")) && !options.HasOption(wxT("icon-file"))) textSizer->Hide(staticBitmap);
	if (!options.HasOption(wxT("text"))) textSizer->Hide(mainText);
	if (!options.HasOption(wxT("informative-text"))) textSizer->Hide(informativeText);
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

void MessageDialog::OnButton1(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s", m_button1->GetLabel().mb_str(wxConvUTF8));
	else printf("1");

	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	
	Destroy(); // Dlg is top window, so this ends the app.
}

void MessageDialog::OnButton2(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s", m_button3->GetLabel().mb_str(wxConvUTF8));
	else printf("2");

	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	
	Destroy(); // Dlg is top window, so this ends the app.
}

void MessageDialog::OnButton3(wxCommandEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("%s", m_button3->GetLabel().mb_str(wxConvUTF8));
	else printf("3");

	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	Destroy(); // Dlg is top window, so this ends the app.
}

void MessageDialog::OnTimeout(wxTimerEvent& WXUNUSED(event)) {
	if (m_options.HasOption(wxT("string-output"))) printf("timeout");
	else printf("0");

	if (!m_options.HasOption(wxT("no-newline"))) printf("\n");
	Destroy(); // Dlg is top window, so this ends the app.
}