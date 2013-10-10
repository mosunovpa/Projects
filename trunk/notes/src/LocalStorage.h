#pragma once

class CAppState;

class CLocalStorage
{
public:
	CLocalStorage(void);
	~CLocalStorage(void);
	void Release();
	void Read(CAppState& cfg);
	void Write(CAppState const& cfg);
private:
	_tstring m_fileName;
	CComPtr<IXMLDOMDocument> s_spDoc;
};
