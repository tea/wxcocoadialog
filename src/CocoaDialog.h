#ifndef __COCOADIALOG_H__
#define __COCOADIALOG_H__

#include "wx/wxprec.h" // For compilers that support precompilation, includes "wx/wx.h".
#include "OptionDict.h"

class CocoaDialogApp : public wxApp {
public:
	virtual bool OnInit(); // { return true; };

	bool OptionError(const wxString& error = wxEmptyString) const;
	bool OptionHelp(const wxString& runmode = wxEmptyString) const;

private:
	OptionDict m_optionDict;
};

#endif //__COCOADIALOG_H__
