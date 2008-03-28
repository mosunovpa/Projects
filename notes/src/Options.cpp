#include "StdAfx.h"
#include "Options.h"

COptions::COptions(void) : m_bAlwaysOnTop(FALSE)
{
}

COptions::~COptions(void)
{
}

BOOL COptions::GetAlwaysOnTop() const
{
	return m_bAlwaysOnTop;
}

void COptions::SetAlwaysOnTop( BOOL bVal )
{
	m_bAlwaysOnTop = bVal;
}