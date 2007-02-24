#ifndef __COCOADIALOG_H__
#define __COCOADIALOG_H__

#include "wx/wxprec.h" // For compilers that support precompilation, includes "wx/wx.h".
#include "OptionDict.h"
#include <wx/filename.h>

class CocoaDialogApp : public wxApp {
public:
	virtual bool OnInit(); // { return true; };

	bool OptionError(const wxString& error = wxEmptyString) const;
	bool OptionHelp(const wxString& runmode = wxEmptyString) const;

	// utf8 args (set by main)
	static wxArrayString utf8_args;

private:
	void ShowFileDialog() const;

#ifdef __WXMSW__
	// Utility functions
	static wxString WinPathToCygwin(const wxFileName& path);
	static wxString CygwinPathToWin(const wxString& path);
	static wxString GetCygwinDir();
#endif // __WXMSW__
	
	// Member variables
	OptionDict m_optionDict;
};

#endif //__COCOADIALOG_H__
