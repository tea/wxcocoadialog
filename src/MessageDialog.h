#ifndef __MESSAGEDIALOG_H__
#define __MESSAGEDIALOG_H__

#include "OptionDict.h"

class MessageDialog : public wxDialog {
public:
	MessageDialog(wxWindow* parent, const OptionDict& options, bool doFloat);

private:
	// Event handlers
	void OnClose(wxCloseEvent& event);
	void OnButton1(wxCommandEvent& event);
	void OnButton2(wxCommandEvent& event);
	void OnButton3(wxCommandEvent& event);
	void OnTimeout(wxTimerEvent& event);
	DECLARE_EVENT_TABLE()

	// Member variables
	const OptionDict& m_options;
	wxTimer m_timer;
	
	// Member ctrls
//	wxTextCtrl* m_messageDialog;
	wxButton* m_button1;
	wxButton* m_button2;
	wxButton* m_button3;
};

#endif //__MESSAGEDIALOG_H__

