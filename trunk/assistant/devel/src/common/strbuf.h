// ***************************************************************
//  strbuf   version:  1.0   ·  date: 09/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class CStringBuf
{
public:
	CStringBuf(int nSize) :	m_pData(NULL), m_nSize(nSize + 1), m_nLength(0)
	{
		m_pData = new TCHAR[m_nSize];
		m_pData[0] = 0;
	}

	CStringBuf(CStringBuf const& right) : m_pData(NULL), m_nSize(right.m_nSize), m_nLength(0)
	{
		m_pData = new TCHAR[m_nSize];
		Copy(right.m_pData);
	}

	CStringBuf(LPCTSTR psz) : m_pData(NULL), m_nSize(1), m_nLength(0)
	{
		assert(psz != m_pData && psz != NULL);
		m_nSize = _tcslen(psz) + 1;
		m_pData = new TCHAR[m_nSize];
		Copy(psz);
	}

	~CStringBuf()
	{
		delete[] m_pData;
	}

	CStringBuf const& operator= (CStringBuf const& right)
	{
		Copy(right.m_pData);
		return *this;
	}

	CStringBuf const& operator= (LPCTSTR psz)
	{
		assert(psz != m_pData && psz != NULL);
		Copy(psz);
		return *this;
	}

	operator LPCTSTR() const
	{
		return m_pData;
	}

	size_t GetLength() const
	{
		return m_nLength;
	}

protected:
	void Copy(LPCTSTR psz)
	{
		assert(psz != m_pData && psz != NULL);
		if (psz != m_pData && psz != NULL)
		{
			m_nLength = _tcsnlen(psz, m_nSize - 1);
			if (m_nLength > 0)
			{
				_tcsncpy_s(m_pData, m_nSize, psz, m_nLength);
			}
			m_pData[m_nLength] = 0; // terminated null
		}
	}

private:
	LPTSTR m_pData;
	size_t m_nLength;
	size_t m_nSize;
};

