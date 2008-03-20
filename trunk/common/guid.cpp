// Guid.cpp: implementation of the CGuid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guid.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma comment(lib, "rpcrt4.lib")

#define GUID_STRING_LEN	40
 
CGuid::CGuid()
{
	m_guid = GUID_NULL;
}

CGuid::CGuid(BSTR guid)
{
	operator=(guid);
}

CGuid::CGuid(const CString& csGuid)
{
	operator=(csGuid);
}

CGuid::CGuid(LPCTSTR lpszGuid)
{
	operator=(lpszGuid);
}

CGuid::~CGuid()
{

}

CGuid::CGuid(const CGuid& g)
{
	m_guid = g.m_guid;	
}

CGuid::CGuid(const GUID& g)
{
	m_guid = g;
}

bool CGuid::operator==(const GUID& g)
{
	return !::memcmp(&m_guid, &g, sizeof(GUID));
}

bool CGuid::operator==(const CGuid& g)
{	
	return operator==(g.m_guid);
}

CGuid::operator GUID&()
{ 
	return m_guid; 
}

CGuid::operator GUID*()
{ 
	return &m_guid; 
}

CGuid::operator CComBSTR()
{ 
	CComBSTR bstrGuid;
	Convert(m_guid, bstrGuid);
	return bstrGuid; 
}

CGuid::operator CString()
{ 
	CString csGuid;
	Convert(m_guid, csGuid);
	return csGuid; 
}

bool CGuid::operator!=(const GUID& g)
{
	return ::memcmp(&m_guid, &g, sizeof(GUID)) != 0;
}

bool CGuid::operator!=(const CGuid& g)
{
	return operator!=(g.m_guid);
}

CGuid& CGuid::operator=(const GUID& g)
{
	if( ::memcmp(&m_guid, &g, sizeof(GUID)) != 0 )
	{
		copy(g);
	}

	return *this;
}

CGuid& CGuid::operator=(const CComBSTR& g)
{
	ATLASSERT(g.m_str);
	Convert(g, m_guid);
	return *this;
}

CGuid& CGuid::operator=(BSTR g)
{
	ATLASSERT(g);
	Convert(g, m_guid);
	return *this;
}

CGuid& CGuid::operator=(LPCTSTR g)
{
	ATLASSERT(g);
	Convert(g, m_guid);
	return *this;
}

CGuid& CGuid::operator=(const CString& g)
{
	Convert(g, m_guid);
	return *this;
}

CGuid& CGuid::operator=(const CGuid& g)
{
	if(this != &g )
	{
		copy(g.m_guid);
	}
	
	return *this;
}

inline void CGuid::copy(const CGuid& g)
{
	copy(g.m_guid);
}

inline void CGuid::copy(const GUID& g)
{
	::memcpy(&m_guid, (void*)&g, sizeof(GUID));
}

bool CGuid::operator<(const CGuid& g1) const
{
	RPC_STATUS status;
	return ::UuidCompare(const_cast<GUID*>(&m_guid), const_cast<GUID*>(&g1.m_guid), &status)==-1;
}

bool CGuid::operator>(const CGuid& g1) const
{
	return !this->operator<(g1) && ::memcmp(&g1.m_guid, &m_guid, sizeof(GUID));
}

bool CGuid::Convert(GUID& guid, CComBSTR& bstrGuid)
{
	OLECHAR szGuid[GUID_STRING_LEN]={0};
	int nCount = ::StringFromGUID2(guid, szGuid, GUID_STRING_LEN);
	bstrGuid = szGuid;
	
	return nCount !=0;
}

bool CGuid::Convert(GUID& guid, CString& csGuid)
{
	OLECHAR szGuid[GUID_STRING_LEN]={0};
	int nCount = ::StringFromGUID2(guid, szGuid, GUID_STRING_LEN);
	csGuid = szGuid;
	
	return nCount !=0;
}

bool CGuid::Convert(const CComBSTR& bstrGuid, GUID& guid)
{	
	return Convert(bstrGuid.m_str, guid);
}

bool CGuid::Convert(BSTR bstrGuid, GUID& guid)
{
	if( bstrGuid == NULL )
	{
		return false;
	}
	
	UINT nLen = ::SysStringLen(bstrGuid);
	if( nLen < GUID_STRING_LEN - 4 )
	{
		return false;
	}

	CString csguid=bstrGuid;
	if( csguid.GetAt(0) == TCHAR('{') )
	{
		ATLASSERT(csguid.Find(TCHAR('}'))!=-1);
		return Convert(csguid.Mid(1, csguid.GetLength()-2), guid);
	}
	else
	{
		return Convert(csguid, guid);
	}
}

bool CGuid::Convert(const CString& csGuid, GUID& guid)
{
	if( csGuid.GetLength() < GUID_STRING_LEN - 4 )
	{
		return false;
	}

	return Convert((LPCTSTR)csGuid, guid);
}

bool CGuid::Convert(LPCTSTR lpszGuid, GUID& guid)
{
	ATLASSERT(lpszGuid);
	return ::UuidFromString((unsigned char*)lpszGuid, &guid) == RPC_S_OK;
}

bool CGuid::Create(CComBSTR& bstrGuid)
{
	GUID guid;
	if( Create(guid) )
	{
		return Convert(guid, bstrGuid);
	}

	return false;
}

bool CGuid::Create(GUID& guid)
{
	return ::UuidCreate(&guid) == RPC_S_OK;
}

long CGuid::HashKey(GUID& guid)
{
	RPC_STATUS status=RPC_S_OK;
	return ::UuidHash(&guid, &status);
}

BOOL CGuid::ProgID(CString& csProgID)
{
	CComBSTR bstrTmp;
	BOOL bRet = ProgID(bstrTmp);
	csProgID = bstrTmp;
	return bRet;
}
	
BOOL CGuid::ProgID(CComBSTR& bstrProgID)
{
	BOOL bRet=FALSE;
	if( *this == GUID_NULL )
	{
		return bRet;
	}

	LPOLESTR psz = NULL;
	if( ::ProgIDFromCLSID(m_guid, &psz) == S_OK )
	{
		bstrProgID=psz;
		::CoTaskMemFree(psz);
		psz=NULL;
		bRet=TRUE;
	}
	return bRet;
}
