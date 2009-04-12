#include "OptionDict.h"

const wxString OptionDict::s_emptyString;

bool OptionDict::ParseArgs(const wxArrayString& args) {
	// 0 is app name and 1 is runmode so both are ignored
	for (unsigned int i = 2; i < args.GetCount(); ++i) {
		// Get the option name
		wxString option(args[i]);
		if (!option.StartsWith(wxT("--"))) return false;
		option = option.substr(2); // strip initial dashes

		// Get Value(s)
		wxArrayString values;
		unsigned int i2 = i+1;
		while (i2 < args.GetCount()) {
			const wxString tmp(args[i2]);
			
			if (!tmp.StartsWith(wxT("--"))) values.Add(tmp);
			else break;	// We have reached the next option
			
			++i2;
		}
		i = i2-1; // adjust for ++ in for loop

		// Add to dict
		if (values.IsEmpty()) m_optionDict[option] = wxEmptyString;
		else if (values.GetCount() == 1) m_optionDict[option] = values[0];
		else m_multiDict[option] = values;
	}

	return true;
}

void OptionDict::SetOption(const wxString& key, const wxString& value) {
	m_optionDict[key] = value;
}

bool OptionDict::HasOption(const wxString& key) const {
	Dict::const_iterator valP = m_optionDict.find(key);

	return valP != m_optionDict.end();
}

bool OptionDict::HasMultiOption(const wxString& key) const {
	MultiDict::const_iterator valP = m_multiDict.find(key);
	
	return (valP != m_multiDict.end() || HasOption(key));
}

const wxString& OptionDict::GetOption(const wxString& key) const {
	Dict::const_iterator valP = m_optionDict.find(key);

	if (valP != m_optionDict.end()) return valP->second;
	else return s_emptyString;
}

bool OptionDict::GetIntegerOption(const wxString& key, int& value) const {
	Dict::const_iterator valP = m_optionDict.find(key);

	if (valP != m_optionDict.end()) {
		long val;
		if (valP->second.ToLong(&val)) {
			value = val;
			return true;
		}
	}

	return false;
}

bool OptionDict::GetMultiOption(const wxString& key, wxArrayString& values) const {
	MultiDict::const_iterator valP = m_multiDict.find(key);

	if (valP != m_multiDict.end()) {
		values = valP->second;
		return true;
	}
	else {
		// Check if there is a matching option with a single value
		Dict::const_iterator singleValP = m_optionDict.find(key);

		if (singleValP != m_optionDict.end()){
			values.Add(singleValP->second);
			return true;
		}
	}

	return false;
}

void OptionDict::Print() const {
	for (Dict::const_iterator p = m_optionDict.begin(); p != m_optionDict.end(); ++p) {
		printf("%s: %s\n", p->first.utf8_str().data(), p->second.utf8_str().data());
	}
	
	for (MultiDict::const_iterator m = m_multiDict.begin(); m != m_multiDict.end(); ++m) {
		printf("%s:\n", m->first.utf8_str().data());

		const wxArrayString& opts = m->second;
		for (unsigned int i = 0; i < opts.GetCount(); ++i) {
			printf("  %s\n", opts[i].utf8_str().data());
		}
	}
}

