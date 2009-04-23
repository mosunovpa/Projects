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
	CMenu menuNotes;

	// Load the menu resource 
	if (!menuNotes.LoadMenu(IDR_NOTEMENU))
	{
		return;
	}
	CMenuHandle menuPopup = menuNotes.GetSubMenu(0);
	if (menuPopup.m_hMenu == NULL)
	{
		return;
	}

	menuPopup.SetMenuDefaultItem(ID_CLOSE);

	if (CApplication::Get().GetOpenedNotesCount() == 1)
	{
		menuutils::SetMenuItemEnable(menuPopup, ID_CLOSEALL, CApplication::Get().GetOpenedNotesCount() > 1);
		menuutils::SetMenuItemEnable(menuPopup, ID_CLOSEALLBUTTHIS, CApplication::Get().GetOpenedNotesCount() > 1);
	}
	if (m_pNoteWnd->GetDeletedDate() == 0)
	{
		menuPopup.DeleteMenu(ID_RESTORE, MF_BYCOMMAND);
	}
	menuPopup.DeleteMenu(m_pNoteWnd->IsMinimized() ? ID_ROLLUP : ID_UNROLL, MF_BYCOMMAND);
	
	if (!menuPopup.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,
		pt.x, pt.y, GetParent()))
	{
		return;
	}
}
