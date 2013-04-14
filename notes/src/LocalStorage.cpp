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
}