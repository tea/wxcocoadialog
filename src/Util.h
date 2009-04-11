#ifndef __CD_UTIL_H__
#define __CD_UTIL_H__

#include "wx/wxprec.h" // For compilers that support precompilation, includes "wx/wx.h".
#include <wx/filename.h>

#ifdef __WXMSW__
	// Global Utility functions
	wxString WinPathToCygwin(const wxFileName& path);
	wxString CygwinPathToWin(const wxString& path);
	wxString GetCygwinDir();
#endif // __WXMSW__

#endif //__CD_UTIL_H__

