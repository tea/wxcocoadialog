#ifndef __INPUTBOX_H__
#define __INPUTBOX_H__

#include "OptionDict.h"

class InputBox : public wxDialog {
public:
	InputBox(const OptionDict& options, bool doFloat);

private:
	// Event handlers
	void OnButton1(wxCommandEvent& event);
	void OnButton2(wxCommandEvent& event);
	void OnButton3(wxCommandEvent& event);
	void OnTimeout(wxTimerEvent& event);
	DECLARE_EVENT_TABLE()

	// Member variables
	const OptionDict& m_options;
	wxTimer m_timer;
	
	// Member ctrls
	wxTextCtrl* m_inputBox;
	wxButton* m_button1;
	wxButton* m_button2;
	wxButton* m_button3;
};

#endif //__INPUTBOX_H__