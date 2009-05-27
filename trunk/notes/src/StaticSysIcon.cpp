#include "StdAfx.h"
#include "StaticSysIcon.h"
#include "resource.h"
#include "atlwinmisc.h"
#include "Application.h"
#include "NoteWnd.h"
#include "menuutils.h"

CStaticSysIcon::CStaticSysIcon(CNoteWnd* pNoteWnd) : m_pNoteWnd(pNoteWnd)
{
}

CStaticSysIcon::~CStaticSysIcon(void)
{
}

void CStaticSysIcon::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWindowRect rc(m_hWnd);
	ShowMenu(CPoint(rc.left, rc.bottom));
}

void CStaticSysIcon::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	::PostMessage(GetParent(), WM_CLOSE, 0, 0);
}

void CStaticSysIcon::ShowMenu( CPoint pt )
{
	m_pNoteWnd->ShowSystemMenu(pt);
}
