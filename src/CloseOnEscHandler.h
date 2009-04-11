#ifndef __CLOSEONESCHANDLER_H__
#define __CLOSEONESCHANDLER_H__

#include "wx/wxprec.h" // For compilers that support precompilation, includes "wx/wx.h".

class CloseOnEscHandler :
	public wxEvtHandler
{
	public:
		CloseOnEscHandler(wxWindow* parent);
	private:
		// Event handlers
		void OnChar(wxKeyEvent &evt);
		DECLARE_EVENT_TABLE();
		// Member variables
		wxWindow* parent;
};

#endif __CLOSEONESCHANDLER_H__