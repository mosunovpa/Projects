// Guid.h: interface for the CGuid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUID_H__BBBFB961_9EC0_4414_8B69_067CDD2FFB85__INCLUDED_)
#define AFX_GUID_H__BBBFB961_9EC0_4414_8B69_067CDD2FFB85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuid  
{
public:
	CGuid();
	CGuid(BSTR guid);
	CGuid(const CString& csGuid);
	CGuid(LPCTSTR lpszGuid);
	CGuid(const CGuid& g);
	CGuid(const GUID& g);
	virtual ~CGuid();
 
	CGuid& operator=(const GUID& g);
	CGuid& operator=(const CGuid& g);
	CGuid& operator=(BSTR g);
	CGuid& operator=(const CComBSTR& g);
	CGuid& operator=(const CString& g);
	CGuid& operator=(LPCTSTR g);

	bool operator==(const GUID& g);
	bool operator==(const CGuid& g);

	bool operator!=(const GUID& g);
	bool operator!=(const CGuid& g);

	operator GUID&();
	operator GUID*();
	operator CComBSTR();
	operator CString();

	bool operator<(const CGuid& g1) const;
	bool operator>(const CGuid& g1) const;

	long HasKey(){ return HashKey(*this); };

	BOOL ProgID(CString& csProgID);
	BOOL ProgID(CComBSTR& bstrProgID);
	
	static bool Create(GUID& guid);
	static bool Create(CComBSTR& guid);

	static bool Convert(GUID& guid, CComBSTR& bstrGuid);
	static bool Convert(GUID& guid, CString& csGuid);

	static bool Convert(BSTR bstrGuid, GUID& guid);
	static bool Convert(const CComBSTR& bstrGuid, GUID& guid);
	static bool Convert(const CString& bstrGuid, GUID& guid);
	static bool Convert(LPCTSTR lpszGuid, GUID& guid);
	
	static long HashKey(GUID& guid);
	

protected:
	GUID m_guid;
	void copy(const CGuid& g);	
	void copy(const GUID& g);

};

#endif // !defined(AFX_GUID_H__BBBFB961_9EC0_4414_8B69_067CDD2FFB85__INCLUDED_)
