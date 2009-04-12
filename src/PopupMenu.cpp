#include "CocoaDialog.h"

void CocoaDialogApp::ShowPopupMenu() const {
	wxArrayString items;
	if (!m_optionDict.GetMultiOption(wxT("items"), items)) return;

	// Build menu
	wxMenu menu;
	unsigned int id = 1;
	for (unsigned int i = 0; i < items.GetCount(); ++i) {
		if (items[i] == wxT("-")) {
			menu.AppendSeparator();
		}
		else {
			wxString item = items[i];

			if (item[0] == wxT('#')) {
				item.Remove(0, 1);
				menu.Append(id, item);
				menu.Enable(id, false);
			}
			else if (item[0] == wxT('+')) {
				item.Remove(0, 1);
				
				menu.AppendCheckItem(id, item);
				menu.Check(id, true);
			}
			else {
				menu.Append(id, item);
			}
		}

		++id;
	}

	// We need a hidden window to be parent of the menu
	// to enable keyboard input in the menu, it has to be shown. but since
	// we don't want it to be visible, we show it outside the screen
	wxDialog dlg(m_parentWnd, wxID_ANY, wxEmptyString, wxPoint(-1000, -1000));
	dlg.Show();

	// Set menu position
	wxPoint point = wxDefaultPosition;
	int x;
	int y;
	if (m_optionDict.GetIntegerOption(wxT("xpos"), x)) point.x = x;
	if (m_optionDict.GetIntegerOption(wxT("ypos"), y)) point.y = y;
	if (point == wxDefaultPosition)
	{
		point = wxGetMousePosition();
	}

	int res = 0;
#ifdef __WXMSW__
	HMENU hMenu = (HMENU)menu.GetHMenu();
	res = ::TrackPopupMenu(hMenu, TPM_RETURNCMD|TPM_LEFTALIGN, point.x, point.y, 0, (HWND)dlg.GetHWND(), NULL);
#endif //__WXMSW__
	// TODO: Implement for other platforms than windows

	if (res != 0) {
		if (m_optionDict.HasOption(wxT("string-output"))) printf("%s", menu.GetLabel(res).utf8_str().data());
		else printf("%d", res);

		if (!m_optionDict.HasOption(wxT("no-newline"))) printf("\n");
	}
	
	// If we have a parent, we want to pass focus to it before closing
	// (otherwise the system may activate a random window)
	if (m_parentWnd) {
#ifdef __WXMSW__
		// Activate the parent frame
		HWND hwnd = GetHwndOf(m_parentWnd);
		::SetForegroundWindow(hwnd);
		::SetFocus(hwnd);
#endif //__WXMSW__
	}
}

