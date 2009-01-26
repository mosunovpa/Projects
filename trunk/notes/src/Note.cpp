#include "StdAfx.h"
#include "Note.h"

CNote::CNote(void) : m_id(0), m_dtCreated(0), m_dtModified(0), m_dtDeleted(0)
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

int CNote::GetTextLen() const
{
	return m_text.size();
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

time_t CNote::GetCreatedDate() const
{
	return m_dtCreated;
}

void CNote::SetCreatedDate(time_t dt)
{
	m_dtCreated = dt;
}

time_t CNote::GetModifiedDate() const
{
	return m_dtModified;
}

void CNote::SetModifiedDate(time_t dt)
{
	m_dtModified = dt;
}

time_t CNote::GetDeletedDate() const
{
	return m_dtDeleted;
}

void CNote::SetDeletedDate( time_t dt )
{
	m_dtDeleted = dt;
}