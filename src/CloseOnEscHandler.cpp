#include "CloseOnEscHandler.h"

BEGIN_EVENT_TABLE(CloseOnEscHandler, wxEvtHandler)
	EVT_CHAR(CloseOnEscHandler::OnChar)
END_EVENT_TABLE()

CloseOnEscHandler::CloseOnEscHandler(wxWindow* parent)
: parent(parent) {}

void CloseOnEscHandler::OnChar(wxKeyEvent &evt) {
	if (evt.GetKeyCode() == WXK_ESCAPE) {
		parent->Destroy();
		return;
	}

	evt.Skip();
}
