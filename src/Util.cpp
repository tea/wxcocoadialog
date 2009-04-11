#include "Util.h"

#ifdef __WXMSW__

wxString WinPathToCygwin(const wxFileName& path) { // static
	wxASSERT(path.IsOk() && path.IsAbsolute());

	// Drive
	wxString unixPath = wxT("/cygdrive/");
	unixPath += path.GetVolume().Lower();

	// Dirs
	const wxArrayString& dirs = path.GetDirs();
	for (unsigned int i = 0; i < dirs.GetCount(); ++i) {
		unixPath += wxT('/') + dirs[i];
	}

	// Filename
	if (path.HasName()) {
		unixPath += wxT('/') + path.GetFullName();
	}

	return unixPath;
}

wxString CygwinPathToWin(const wxString& path) { // static
	if (path.empty()) {
		wxASSERT(false);
		return wxEmptyString;
	}
	wxString newpath;

	if (path.StartsWith(wxT("/cygdrive/"))) {

		// Get drive letter
		const wxChar drive = wxToupper(path[10]);
		if (drive < wxT('A') || drive > wxT('Z')) {
			wxASSERT(false);
			return wxEmptyString;
		}

		// Build new path
		newpath += drive;
		newpath += wxT(':');
		if (path.size() > 11) newpath += path.substr(11);
		else newpath += wxT('\\');
	}
	else if (path.StartsWith(wxT("/usr/bin/"))) {
		newpath = GetCygwinDir() + wxT("\\bin\\");
		newpath += path.substr(9);
	}
	else if (path.GetChar(0) == wxT('/')) {
		newpath = GetCygwinDir();
		newpath += path;
	}
	else return path; // no conversion

	// Convert path seperators
	for (unsigned int i = 0; i < newpath.size(); ++i) {
		if (newpath[i] == wxT('/')) newpath[i] = wxT('\\');
	}

	return newpath;
}

#include <wx/msw/registry.h>

wxString GetCygwinDir() { // static
	wxString cygPath;

	// Check if we have a cygwin installation
	wxRegKey cygKey(wxT("HKEY_LOCAL_MACHINE\\SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2\\/"));
	if( cygKey.Exists() ) {
		if (cygKey.HasValue(wxT("native"))) {
			cygKey.QueryValue(wxT("native"), cygPath);
		}
	}

	// Also check "current user" (might be needed if user did not have admin rights during install)
	if (cygPath.empty()) {
		wxRegKey cygKey2(wxT("HKEY_CURRENT_USER\\SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2\\/"));
		if( cygKey2.Exists() ) {
			if (cygKey2.HasValue(wxT("native"))) {
				cygKey2.QueryValue(wxT("native"), cygPath);
			}
		}
	}

	return cygPath;
}

#endif // __WXMSW__

