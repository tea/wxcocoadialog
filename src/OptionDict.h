#ifndef __OPTIONDICT_H__
#define __OPTIONDICT_H__

#include "wx/wxprec.h" // For compilers that support precompilation, includes "wx/wx.h".

#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

WX_DECLARE_STRING_HASH_MAP( wxString, Dict );
WX_DECLARE_STRING_HASH_MAP( wxArrayString, MultiDict );

class OptionDict {
public:
	bool ParseArgs(const wxArrayString& args);
	
	bool HasOption(const wxString& key) const;
	const wxString& GetOption(const wxString& key) const;
	bool GetIntegerOption(const wxString& key, int& value) const;
	bool GetMultiOption(const wxString& key, wxArrayString& values) const;
	bool HasMultiOption(const wxString& key) const;

	void SetOption(const wxString& key, const wxString& value = wxEmptyString);

	void SetRunmode(const wxString& runmode) {m_runmode = runmode;};
	const wxString& GetRunmode() const {return m_runmode;};

	void Print() const;

private:
	
	// Member variables
	Dict m_optionDict;
	MultiDict m_multiDict;
	wxString m_runmode;
	static const wxString s_emptyString;
};

#endif //__OPTIONDICT_H__

