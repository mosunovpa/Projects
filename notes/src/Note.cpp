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

_tstring CNote::GetText() const
{
	return m_text;
}

int CNote::GetTextLen() const
{
	return m_text.size();
}

CRect CNote::GetPos() const
{
	return m_pos;
}

time_t CNote::GetCreatedDate() const
{
	return m_dtCreated;
}

time_t CNote::GetDeletedDate() const
{
	return m_dtDeleted;
}

time_t CNote::GetModifiedDate() const
{
	return m_dtModified;
}

_tstring CNote::GetLabel() const
{
	return m_label;
}

void CNote::SetId( int id )
{
	m_id = id;
}

void CNote::SetText(_tstring const& text)
{
	m_text = text;
}

void CNote::SetPos( CRect const& pos )
{
	m_pos = pos;
}

void CNote::SetCreatedDate(time_t dt)
{
	m_dtCreated = dt;
}

void CNote::SetModifiedDate(time_t dt)
{
	m_dtModified = dt;
}

void CNote::SetLabel(_tstring const& text)
{
	m_label = text;
}

void CNote::SetDeletedDate( time_t dt )
{
	m_dtDeleted = dt;
}