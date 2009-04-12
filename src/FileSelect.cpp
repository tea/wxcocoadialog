#include "CocoaDialog.h"
#include "Util.h"

void CocoaDialogApp::ShowFileDialog() const {

	const wxString msg = m_optionDict.GetOption(wxT("text"));
	
	// Extensions
	wxString wildcard;
	wxArrayString exts;
	if (m_optionDict.GetMultiOption(wxT("with-extensions"), exts)) {
		for (unsigned int i = 0; i < exts.GetCount(); ++i) {
			const wxString& ext = exts[i];
			
			if (!ext.empty()) {
				if (i) wildcard += wxT(';');

				if (ext[0] == wxT('.')) wildcard += wxT("*") + ext;
				else wildcard += wxT("*.") + ext;
			}
		}
	}
	else wildcard = wxT("*.*"); // default
	
	// Default file and dir
	const wxString defaultfile = m_optionDict.GetOption(wxT("file"));
	wxString defaultdir = m_optionDict.GetOption(wxT("with-directory"));
#ifdef __WXMSW__
	if (!defaultdir.empty()) {
		defaultdir = CygwinPathToWin(defaultdir);
	}
#endif // __WXMSW__

	// Show Dialog
	wxArrayString paths;
	if (m_optionDict.GetRunmode() == wxT("fileselect")) {
		if (m_optionDict.HasOption(wxT("select-only-directories"))) {
			// OpenDirectory Dialog
			wxDirDialog dlg(m_parentWnd, msg, defaultdir);
			if (dlg.ShowModal() == wxID_OK) {
				paths.Add(dlg.GetPath());
			}

			if (m_parentWnd) dlg.Reparent(NULL); // avoid ref to non wxWidgets window
		}
		else {
			// OpenFile Dialog
			int style = wxFD_OPEN;
			if (m_optionDict.HasOption(wxT("select-multiple"))) style |= wxFD_MULTIPLE;

			wxFileDialog dlg(m_parentWnd, msg, defaultdir, defaultfile, wildcard, style);
			if (dlg.ShowModal() == wxID_OK) {
				if (style & wxFD_MULTIPLE) dlg.GetPaths(paths);
				else paths.Add(dlg.GetPath());
			}

			if (m_parentWnd) dlg.Reparent(NULL); // avoid ref to non wxWidgets window
		}
	}
	else if (m_optionDict.GetRunmode() == wxT("filesave")) {
		// FileSave Dialog
		const int style = wxFD_SAVE;

		wxFileDialog dlg(m_parentWnd, msg, defaultdir, defaultfile, wildcard, style);
		if (dlg.ShowModal() == wxID_OK) {
			paths.Add(dlg.GetPath());
		}

		if (m_parentWnd) dlg.Reparent(NULL); // avoid ref to non wxWidgets window
	}

	// Print paths to stdout
	if(!paths.empty()) {
		for (unsigned int i = 0; i < paths.GetCount(); ++i) {
			wxString& path = paths[i];

#ifdef __WXMSW__
			const wxFileName fn(path);
			path = WinPathToCygwin(fn);
#endif // __WXMSW__
			
			printf("%s", path.utf8_str().data());
		}
	}
}

