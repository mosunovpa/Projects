#include "StdAfx.h"
#include "Note.h"
#include "dateutils.h"

using namespace dateutils;

CNote::CNote(void) : m_id(0)
{
	timebn::clear(m_dtCreated);
	timebn::clear(m_dtModified);
	timebn::clear(m_dtDeleted);
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

_timeb CNote::GetCreatedDate() const
{
	return m_dtCreated;
}

_timeb CNote::GetDeletedDate() const
{
	return m_dtDeleted;
}

_timeb CNote::GetModifiedDate() const
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

void CNote::SetCreatedDate(_timeb dt)
{
	m_dtCreated = dt;
}

void CNote::SetModifiedDate(_timeb dt)
{
	m_dtModified = dt;
}

void CNote::SetLabel(LPCTSTR label)
{
	m_label = label;
}

void CNote::SetDeletedDate( _timeb dt )
{
	m_dtDeleted = dt;
}