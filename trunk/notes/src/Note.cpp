#include "StdAfx.h"
#include "Note.h"

CNote::CNote(void) : m_id(0), m_dtCreated(0)
{
}

CNote::~CNote(void)
{
}

int CNote::GetId() const
{
	return m_id;
}

LPCTSTR CNote::GetText() const
{
	return m_text.c_str();
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

time_t CNote::GetCreated() const
{
	return m_dtCreated;
}

void CNote::SetCreated(time_t dt)
{
	m_dtCreated = dt;
}