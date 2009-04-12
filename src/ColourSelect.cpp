#include "CocoaDialog.h"
#include <wx/colordlg.h>

void CocoaDialogApp::ShowColourDialog() const {
	wxColourData colourData;
	colourData.SetChooseFull(true);

	// Set options
	wxString colourStr = m_optionDict.GetOption(wxT("colour"));
	if (colourStr.empty()) colourStr = m_optionDict.GetOption(wxT("color"));
	if (!colourStr.empty()) {
		// Allow input of hex colours without leading #
		if (colourStr.length() == 6) {
			bool isHex = true;
			for (unsigned int i = 0; i < colourStr.length(); ++i) {
				if (!wxIsxdigit(colourStr[i])) isHex = false;
			}
			if (isHex) colourStr.Prepend(wxT('#'));
		}

		const wxColour defaultColour(colourStr);
		if (defaultColour.IsOk()) {
			colourData.SetColour(defaultColour);
		}
	}

	// Show Dialog
	wxColourDialog dlg(m_parentWnd, &colourData);
	dlg.SetTitle(m_optionDict.GetOption(wxT("title")));
	dlg.Centre();
	const int res = dlg.ShowModal();

	if (m_parentWnd) dlg.Reparent(NULL); // avoid ref to non wxWidgets window
	if (res != wxID_OK) return; // cancel

	// Output result
	const wxColour selectedColour = dlg.GetColourData().GetColour();
	if (m_optionDict.HasOption(wxT("output-rgb"))) {
		printf("%d\n%d\n%d", selectedColour.Red(), selectedColour.Green(), selectedColour.Blue());
	}
	else if (m_optionDict.HasOption(wxT("output-css"))) {
		const wxString outputColour = selectedColour.GetAsString(wxC2S_CSS_SYNTAX);
		printf("%s", outputColour.utf8_str().data());
	}
	else {
		const wxString outputColour = selectedColour.GetAsString(wxC2S_HTML_SYNTAX);
		printf("%s", outputColour.utf8_str().data());
	}

	if (!m_optionDict.HasOption(wxT("no-newline"))) printf("\n");
}

