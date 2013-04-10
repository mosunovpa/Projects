#pragma once

class CConfig;

class CLocalStorage
{
public:
	CLocalStorage(void);
	~CLocalStorage(void);

	void SetDataFile(LPCTSTR fileName);
	void Read(CConfig& cfg);
private:
	_tstring m_fileName;
	CComPtr<IXMLDOMDocument> s_spDoc;
};
