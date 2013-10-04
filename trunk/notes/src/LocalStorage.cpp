#include "StdAfx.h"
#include "Config.h"
#include "LocalStorage.h"
#include "Application.h"

#define LOCALSTORAGE_DATAFILE _T("notes.cfg")

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
		CComBSTR sXml(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?><config><last-file></last-file></config>"));
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

void CLocalStorage::Read(CConfig& cfg)
{
	CComPtr<IXMLDOMNode> spNode;
	CHECK_HR(s_spDoc->selectSingleNode(CComBSTR(_T("config/last-file")),&spNode));
	if (spNode)
	{
		CComBSTR s;
		spNode->get_text(&s);
		cfg.SetLastDataFileName((LPCTSTR)s);
	}
}

void CLocalStorage::Write(CConfig const& cfg)
{
	CComPtr<IXMLDOMNode> spNode;
	CHECK_HR(s_spDoc->selectSingleNode(CComBSTR(_T("config/last-file")),&spNode));
	if (spNode)
	{
		spNode->put_text(CComBSTR( cfg.GetLastDataFileName().c_str() ));
	}
	CHECK_HR(s_spDoc->save(CComVariant(m_fileName.c_str())));
}
