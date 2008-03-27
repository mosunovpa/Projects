#include "StdAfx.h"
#include "StaticSysIcon.h"
#include "resource.h"
#include "atlwinmisc.h"

CStaticSysIcon::CStaticSysIcon(void)
{
}

CStaticSysIcon::~CStaticSysIcon(void)
{
	// Destroy the menu
	if (m_menuNotes.m_hMenu != NULL)
		m_menuNotes.DestroyMenu();
}

void CStaticSysIcon::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWindowRect rc(m_hWnd);

	// Load the menu resource 
	if (!m_menuNotes.LoadMenu(IDR_NOTEMENU))
	{
		m_menuNotes.DestroyMenu();
		return;
	}
	CMenuHandle menuPopup = m_menuNotes.GetSubMenu(0);
	if (menuPopup.m_hMenu == NULL)
	{
		m_menuNotes.DestroyMenu();
		return;
	}

	MENUITEMINFO mif;
	ZeroMemory(&mif, sizeof(MENUITEMINFO));
	mif.cbSize = sizeof(MENUITEMINFO);
	mif.fMask = MIIM_STATE;
	mif.fState = MFS_DEFAULT;
	::SetMenuItemInfo(menuPopup, ID_CLOSE, FALSE, &mif);

	if (!menuPopup.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON,
		rc.left, rc.bottom, GetParent()))
	{
		m_menuNotes.DestroyMenu();
		return;
	}		

	m_menuNotes.DestroyMenu();
}

void CStaticSysIcon::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	::PostMessage(GetParent(), WM_CLOSE, 0, 0);
}