#pragma once

class CConfig;

class CLocalStorage
{
public:
	CLocalStorage(void);
	~CLocalStorage(void);
	void Release();
	void Read(CConfig& cfg);
	void Write(CConfig const& cfg);
private:
	_tstring m_fileName;
	CComPtr<IXMLDOMDocument> s_spDoc;
};
