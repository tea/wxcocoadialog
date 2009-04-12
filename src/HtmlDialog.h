#ifndef __HTMLDIALOG_H__
#define __HTMLDIALOG_H__

#include "OptionDict.h"
#include "IHtmlWnd.h"

class HtmlDialog : public wxDialog {
public:
	HtmlDialog(wxWindow* parent, const OptionDict& options);

private:
	// Event handlers
	void OnClose(wxCloseEvent& event);
	void OnBeforeLoad(IHtmlWndBeforeLoadEvent& event);
	DECLARE_EVENT_TABLE()

	// Support functions
	static wxString URLDecode(const wxString &value);
	static int ParseHex(const wxString& hexStr);
	static int HexToNumber(char hex);

	// Member Ctrls
	IHtmlWnd* m_browser;

	// Member variables
	const OptionDict& m_optionDict;
	wxString m_tempPath;

};

#endif //__HTMLDIALOG_H__
