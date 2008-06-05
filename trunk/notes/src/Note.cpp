#include "StdAfx.h"
#include "Note.h"

CNote::CNote(void) : m_id(0)
{
}

CNote::~CNote(void)
{
}

int CNote::GetId() const
{
	return m_id;
}

_tstring CNote::GetText() const
{
	return m_text;
}

CRect CNote::GetPos() const
{
	return m_pos;
}

void CNote::SetId( int id )
{
	m_id = id;
}

void CNote::SetText(LPCTSTR text)
{
	m_text = text;
}

void CNote::SetPos( CRect const& pos )
{
	m_pos = pos;
}