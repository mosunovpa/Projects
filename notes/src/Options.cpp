#include "StdAfx.h"
#include "Options.h"

COptions::COptions(void) : m_bAlwaysOnTop(FALSE), m_fs(FS_SMALL), m_newnote_size(200, 160)
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

COptions::FontSize COptions::GetFontSize() const
{
	return m_fs;
}

void COptions::SetFontSize( FontSize fs )
{
	m_fs = fs;
}

SIZE COptions::GetNewNoteSize() const
{
	return m_newnote_size;
}

void COptions::SetNewNoteSize(SIZE const& newnote_size)
{
	m_newnote_size = newnote_size;
}
