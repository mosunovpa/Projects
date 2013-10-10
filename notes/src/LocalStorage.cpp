#include "StdAfx.h"
#include "AppState.h"
#include "LocalStorage.h"
#include "Application.h"

#define LOCALSTORAGE_DATAFILE _T("notes.cfg")
#define DATAFILE _T("notes.dat")

CLocalStorage::CLocalStorage(void)
{
	TCHAR destBuf[MAX_PATH] = _T("");
	::PathCombine(destBuf, CApplication::Get().GetAppFolder().c_str(), LOCALSTORAGE_DATAFILE);
	m_fileName = destBuf;
	if (s_spDoc == NULL)
	{
		CHECK_HR_MSG(s_spDoc.CoCreateInstance(__uuidof(DOMDocument)), _T("Create document error"));
	}
	LPCTSTR sFileName = m_fileName.c_str();
	VARIANT_BOOL bSuccess = false;
	if (::PathFileExists(sFileName))
	{
		CHECK_HR_MSG(s_spDoc->load(CComVariant(sFileName), &bSuccess), _T("Load file error"));
	}
	else
	{
		CComBSTR sXml(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><config><recent-files></recent-files></config>"));
		CHECK_HR_MSG(s_spDoc->loadXML(sXml, &bSuccess), _T("Load xml error"));
	}
}

CLocalStorage::~CLocalStorage(void)
{
}

void CLocalStorage::Release()
{
	s_spDoc.Release();
}

void CLocalStorage::Read(CAppState& cfg)
{
	long len = 0;
	CComPtr<IXMLDOMNodeList> spFiles;

	CHECK_HR(s_spDoc->getElementsByTagName(L"config/recent-files/file", &spFiles));
	CHECK_HR(spFiles->get_length(&len));
	if (len == 0)
	{
		_tstring last_datafile_name;
		last_datafile_name.resize(MAX_PATH);
		_tstring sAppFolder = CApplication::Get().GetAppFolder();
		::PathCombine(&last_datafile_name[0], sAppFolder.c_str(), DATAFILE);

		cfg.SetLastDataFile(last_datafile_name.c_str());
	}
	for (int i = len - 1; i >= 0; --i)
	{
		CComPtr<IXMLDOMNode> spChild;
		CHECK_HR(spFiles->get_item(i, &spChild));

		CComPtr<IXMLDOMElement> spElement;
		CHECK_HR(spChild.QueryInterface(&spElement));

		CComVariant val;
		CHECK_HR(spElement->getAttribute(L"name", &val));

		if(::PathFileExists((LPCTSTR)val.bstrVal))
		{
			cfg.SetLastDataFile((LPCTSTR)val.bstrVal);
		}
	}
}

void CLocalStorage::Write(CAppState const& cfg)
{
	long len = 0;
	CComPtr<IXMLDOMNode> spRecentFiles;
	CComPtr<IXMLDOMNodeList> spFiles;
	CHECK_HR(s_spDoc->selectSingleNode(CComBSTR(_T("config/recent-files")),&spRecentFiles));
	if (spRecentFiles)
	{
		// удалить узлы <file>
		CHECK_HR(spRecentFiles->get_childNodes(&spFiles));
		CHECK_HR(spFiles->get_length(&len));
		for (int i = len - 1; i>=0; --i)
		{
			CComPtr<IXMLDOMNode> spChild;
			CHECK_HR(spFiles->get_item(i, &spChild));
			CHECK_HR(spRecentFiles->removeChild(spChild, NULL));
		}

		// добавить узлы <file>
		const std::list<CAppState::CRecentFile>& rf = cfg.GetRecentFiles();
		std::list<CAppState::CRecentFile>::const_iterator it;
		for (it = rf.begin(); it != rf.end(); ++it)
		{
			CComPtr<IXMLDOMElement> spChildElem;
			CHECK_HR(s_spDoc->createElement(_T("file"), &spChildElem));
			CHECK_HR(spChildElem->setAttribute(L"name", CComVariant(it->GetName().c_str())));
			CHECK_HR(spRecentFiles->appendChild(spChildElem, NULL));
		}
	}
	CHECK_HR(s_spDoc->save(CComVariant(m_fileName.c_str())));
}
