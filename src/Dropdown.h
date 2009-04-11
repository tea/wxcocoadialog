#ifndef __DROPDOWN_H__
#define __DROPDOWN_H__

#include "OptionDict.h"

class DropdownBox : public wxDialog {
public:
	DropdownBox(wxWindow* parent, const OptionDict& options, bool doFloat);
	virtual ~DropdownBox();

private:
	void PrintValue() const;

	// Event handlers
	void OnClose(wxCloseEvent& event);
	void OnSelection(wxCommandEvent& event);
	void OnButton1(wxCommandEvent& event);
	void OnButton2(wxCommandEvent& event);
	void OnButton3(wxCommandEvent& event);
	void OnTimeout(wxTimerEvent& event);
	DECLARE_EVENT_TABLE()

	// Member variables
	const OptionDict& m_options;
	wxArrayString m_items;
	wxTimer m_timer;
	
	// Member ctrls
	wxChoice* m_dropdownBox;
	wxButton* m_button1;
	wxButton* m_button2;
	wxButton* m_button3;
};

#endif //__DROPDOWN_H__