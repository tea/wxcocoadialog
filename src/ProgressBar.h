#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

#include "OptionDict.h"

class ProgressBar : public wxDialog {
public:
	ProgressBar(wxWindow* parent, const OptionDict& options, bool doFloat);

private:
	// Event handlers
	void OnClose(wxCloseEvent& event);
	void OnLineRead(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()

	class LineReaderThread : public wxThread {
	public:
		LineReaderThread(wxEvtHandler& evtHandler);
		virtual void *Entry();
	private:
		wxEvtHandler& m_evtHandler;
	};

	// Member variables
	const OptionDict& m_options;

	// Member controls
	wxSizer* m_mainSizer;
	wxStaticText* m_infoText;
	wxGauge* m_progressBar;
};

#endif //__PROGRESSBAR_H__
