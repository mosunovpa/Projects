#include "stdafx.h"
#include "TrayWnd.h"
#include "resutils.h"
#include "Application.h"
#include "menuutils.h"

/////////////////////////////////////////////////////////////////////
// helpers

/**/
_tstring GetCaption(_tstring const& text)
{
	_tstring sCaption = strutils::trim_string(text.c_str(), 64);
	std::replace(sCaption.begin(), sCaption.end(), _T('\t'), _T(' '));
	if (sCaption.size() > 0 && sCaption.size() < text.size())
	{
		sCaption += _T("...");
	}
	return sCaption;
}

/**/
CMenuHandle GetMenuNotes(CMenuHandle menuPopup)
{
	return menuPopup.GetSubMenu(0);
}

/**/
CMenuHandle GetUndeleteMenu(CMenuHandle menuNotes)
{
	int nCount = menuNotes.GetMenuItemCount();
	return menuNotes.GetSubMenu(nCount - 8);
}

///////////////////////////////////////////////////////////
// CTrayWnd

/* constructor*/
CTrayWnd::CTrayWnd() : m_nSelectedMenuItemId(0)
{

}

/* destructor */
CTrayWnd::~CTrayWnd()
{
	// Destroy the menu
	if (m_menuPopup.m_hMenu)
		m_menuPopup.DestroyMenu();
}

/* WM_CREATE */
LRESULT CTrayWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_Icon.LoadIcon(IDR_MAINFRAME, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(m_Icon, TRUE);
	m_IconSm.LoadIcon(IDR_MAINFRAME, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(m_IconSm, FALSE);

	// Load the 'Notes' icon from the application's resources
	m_hIconNotes = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	ATLASSERT(m_hIconNotes);

	// Add the icon to the taskbar status area
	_tstring sAppName = resutils::resstring(IDS_APP_NAME);
	if (sAppName.empty() || !m_TrayIcon.AddIconToTaskBar(m_hWnd, IDR_MAINFRAME, m_hIconNotes, &sAppName[0]))
	{
		ATLTRACE(_T("Icon was not added to the taskbar!\n"));
		return 0;
	}

	// adjust system menu
	CMenuHandle menu = GetSystemMenu(FALSE);
	int nCount = menu.GetMenuItemCount();
	for (int i = nCount - 1; i >= 0;  --i)
	{
		int id = menu.GetMenuItemID(i);
		if (id != SC_CLOSE)
		{
			menu.DeleteMenu(id, MF_BYCOMMAND);
		}
	}

	BOOL bAlwaisOnTop = CApplication::Get().GetOptions().GetAlwaysOnTop();
	SetWindowPos(bAlwaisOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, CRect(0,0,0,0), SWP_NOSIZE | SWP_NOMOVE);

	CreateBitmaps();

	return 0;
}

/* WM_DESTROY */
LRESULT CTrayWnd::OnDestroy(void)
{
	CApplication::Get().ReleaseStorage();

	// Delete the icon from the taskbar status area
	if (!m_TrayIcon.RemoveIconFromTaskBar(m_hWnd, IDR_MAINFRAME))
		ATLTRACE(_T("Icon was not removed from the taskbar!\n"));

	::PostQuitMessage(0);
	SetMsgHandled(FALSE);

	return 0;
}

/* WM_QUERYENDSESSION */
LRESULT CTrayWnd::OnQueryEndSession(UINT wParam, UINT lParam)
{
	OnDestroy();
	return TRUE;
}

/* WMU_NOTIFYICON */
LRESULT CTrayWnd::OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Determine the tray message
	switch (lParam)
	{
		// Right mouse button click - display a popup menu
	case WM_RBUTTONUP:
		DisplayShortcutMenu();
		break;
	case WM_LBUTTONDBLCLK:
		break;
	case WM_LBUTTONUP:
		::SetForegroundWindow(m_hWnd);
		PostMessage(WM_COMMAND, ID_POPUP_NEWNOTE);
		break;
	default:
		break;
	}
	return 0;
}


/*
WM_SETFOCUS
*/
void CTrayWnd::OnFocus(HWND hWnd)
{
	CApplication::Get().ActivateTopNote();
	SetMsgHandled(FALSE);
}

/*
WM_SYSCOMMAND
*/
void CTrayWnd::OnSysCommand(UINT nID, CPoint pt)
{
	if (nID == SC_CLOSE)
	{
		CApplication::Get().CloseAllNotes();
	}
	else
	{
		SetMsgHandled(FALSE);
	}
}

/**/
LRESULT CTrayWnd::DisplayShortcutMenu()
{
	if (m_menuPopup.m_hMenu)
	{
		return 0;
	}
	// Load the menu resource
	if (!m_menuPopup.LoadMenu(IDR_TRAYMENU))
	{
		ATLTRACE(_T("Menu was not loaded!\n"));
		return 0;
	}
	// Get cursor's position
	POINT pt;
	::GetCursorPos(&pt);

	// Display a shortcut menu at the specified location
	// See "PRB: Menus for Notification Icons Don't Work Correctly" in MSDN
	::SetForegroundWindow(m_hWnd);

	// TrackPopupMenu cannot display the menu bar so get
	// a handle to the first shortcut menu
	CMenuHandle menuTrackPopup = GetMenuNotes(m_menuPopup.m_hMenu);
	if (!menuTrackPopup.m_hMenu)
	{
		ATLTRACE(_T("Handle to the first shortcut menu was not retrieved!\n"));
		m_menuPopup.DestroyMenu();
		m_menuPopup.m_hMenu = NULL;
		return 0;
	}

	// set menu item states
	menuTrackPopup.SetMenuDefaultItem(ID_POPUP_NEWNOTE);

	BOOL bAlwaisOnTop = CApplication::Get().GetOptions().GetAlwaysOnTop();
	menuTrackPopup.CheckMenuItem(ID_POPUP_ALWAYS_ON_TOP, MF_BYCOMMAND | (bAlwaisOnTop ? MF_CHECKED : MF_UNCHECKED));

	COptions::FontSize fs = CApplication::Get().GetOptions().GetFontSize();
	menuTrackPopup.CheckMenuItem(ID_FONTSIZE_SMALL, MF_BYCOMMAND | (fs == COptions::FS_SMALL ? MF_CHECKED : MF_UNCHECKED));
	menuTrackPopup.CheckMenuItem(ID_FONTSIZE_MEDIUM, MF_BYCOMMAND | (fs == COptions::FS_MEDIUM ? MF_CHECKED : MF_UNCHECKED));
	menuTrackPopup.CheckMenuItem(ID_FONTSIZE_LARGE, MF_BYCOMMAND | (fs == COptions::FS_LARGE ? MF_CHECKED : MF_UNCHECKED));

	ModifyNotesMenu(menuTrackPopup);

	// Display the shortcut menu. Track the right mouse button

	if (!menuTrackPopup.TrackPopupMenu(TPM_RIGHTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, m_hWnd, NULL))
	{
		ATLTRACE(_T("Shortcut menu was not displayed!\n"));
		m_menuPopup.DestroyMenu();
		m_menuPopup.m_hMenu = NULL;
		return 0;
	}

	::PostMessage(m_hWnd, WM_NULL, 0, 0);

	// Destroy the menu and free any memory that the menu occupies
	m_menuPopup.DestroyMenu();
	m_menuPopup.m_hMenu = NULL;

	return 0;
}

/* ID_POPUP_NEWNOTE */
void CTrayWnd::OnPopupNewnote(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().CreateNote();
}

/* ID_POPUP_SHOWALLNOTES */
void CTrayWnd::OnPopupShowAllnotes(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().ShowAllNotes();
}

/* ID_POPUP_ABOUT */
void CTrayWnd::OnPopupAbout(UINT uNotifyCode, int nID, CWindow wndCtl)
{
}

/* ID_POPUP_EXIT */
void CTrayWnd::OnPopupExit(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	PostMessage(WM_CLOSE);
}

/* ID_POPUP_ALWAYS_ON_TOP */
void CTrayWnd::OnAlwaysOnTop( UINT uNotifyCode, int nID, CWindow wndCtl )
{
	BOOL bAlwaisOnTop = CApplication::Get().GetOptions().GetAlwaysOnTop();
	bAlwaisOnTop = !bAlwaisOnTop;
	SetWindowPos(bAlwaisOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, CRect(0,0,0,0), SWP_NOSIZE | SWP_NOMOVE);
	CApplication::Get().GetOptions().SetAlwaysOnTop(bAlwaisOnTop);
	CApplication::Get().SaveOptions();
}

/**/
void CTrayWnd::OnFontSizeSmall(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().GetOptions().SetFontSize(COptions::FS_SMALL);
	CApplication::Get().SaveOptions();
}

/**/
void CTrayWnd::OnFontSizeMedium(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().GetOptions().SetFontSize(COptions::FS_MEDIUM);
	CApplication::Get().SaveOptions();
}

/**/
void CTrayWnd::OnFontSizeLarge(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().GetOptions().SetFontSize(COptions::FS_LARGE);
	CApplication::Get().SaveOptions();
}

/* ID_OPTIONS_FONT */
void CTrayWnd::OnOptionsFont(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CFontDialog dlg(NULL, CF_EFFECTS | CF_SCREENFONTS, NULL, m_hWnd);
	dlg.DoModal();
}

/* NOTE_CMD_FIRST - NOTE_CMD_LAST */
void CTrayWnd::OnNoteSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().ShowNote(GET_NOTE_ID_FROM_CMD(nID));
}

/* DELETED_CMD_FIRST - DELETED_CMD_LAST */
void CTrayWnd::OnDeletedNoteSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().UndeleteNote(GET_DELETED_ID_FROM_CMD(nID));
}

/* WM_MENURBUTTONUP */
void CTrayWnd::OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu)
{
	m_nSelectedMenuItemId = menu.GetMenuItemID(wParam);
	if (IS_NOTE_CMD(m_nSelectedMenuItemId))
	{
		POINT pt;
		::GetCursorPos(&pt);
		CMenu menuNoteContext;
		menuNoteContext.LoadMenu(IDR_TRAY_NOTE_MENU);
		menuNoteContext.GetSubMenu(0).TrackPopupMenu(TPM_LEFTALIGN | TPM_RECURSE, pt.x, pt.y, m_hWnd, NULL);
	}
}

/* ID_TNM_COPYALLTOCLIPBOARD */
void CTrayWnd::OnCopyAllToClipboard(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedMenuItemId))
	{
		CApplication::Get().NoteTextToClipboard(GET_NOTE_ID_FROM_CMD(m_nSelectedMenuItemId));
		m_nSelectedMenuItemId = 0;
		EndMenu();
	}
}

/* ID_TNM_DELETE */
void CTrayWnd::OnNoteDelete(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (IS_NOTE_CMD(m_nSelectedMenuItemId))
	{
		// delete note
		int nNoteId = GET_NOTE_ID_FROM_CMD(m_nSelectedMenuItemId);
		CApplication::Get().DeleteNote(nNoteId);

		CMenuHandle menuTrackPopup = GetMenuNotes(m_menuPopup.m_hMenu);
		menuTrackPopup.CheckMenuItem(m_nSelectedMenuItemId, MF_BYCOMMAND | MF_CHECKED);
//		menuutils::SetMenuItemEnable(menuTrackPopup, m_nSelectedMenuItemId, FALSE);

//		EndMenu();
		m_nSelectedMenuItemId = 0;
	}
}

/* ID_POPUP_NEWANDPASTE */
void CTrayWnd::OnNewAndPaste(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	CApplication::Get().Command(ID_PASTE, CApplication::Get().CreateNote());
}

/* Create bitmaps for menu*/
void CTrayWnd::CreateBitmaps()
{
	HWND hwndDesktop = GetDesktopWindow(); 
	HDC hdcDesktop = ::GetDC(hwndDesktop); 
	HDC hdcMem = CreateCompatibleDC(hdcDesktop); 

	// Determine the required bitmap size. 

	SIZE size = { GetSystemMetrics(SM_CXMENUCHECK), 
		GetSystemMetrics(SM_CYMENUCHECK) }; 

	// Create a monochrome bitmap and select it. 

	m_bmpDeleted.CreateBitmap(size.cx, size.cy, 1, 1, NULL); 
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, m_bmpDeleted); 

	// Erase the background and call the drawing function. 

	PatBlt(hdcMem, 0, 0, size.cx, size.cy, WHITENESS); 


	HBRUSH hbrOld; 
	hbrOld = (HBRUSH)SelectObject(hdcMem, GetStockObject(NULL_BRUSH)); 

/*
	MoveToEx(hdcMem, 2, 5, NULL); 
	LineTo(hdcMem, size.cx - 4, 5); 
	MoveToEx(hdcMem, 2, 6, NULL); 
	LineTo(hdcMem, size.cx - 4, 6); 
	MoveToEx(hdcMem, 2, 7, NULL); 
	LineTo(hdcMem, size.cx - 4, 7); 
*/
	MoveToEx(hdcMem, 3, 3, NULL); 
	LineTo(hdcMem, 10, 10); 
	MoveToEx(hdcMem, 4, 3, NULL); 
	LineTo(hdcMem, 10, 9); 
	MoveToEx(hdcMem, 3, 4, NULL); 
	LineTo(hdcMem, 9, 10); 

	MoveToEx(hdcMem, 3, 8, NULL); 
	LineTo(hdcMem, 9, 2); 
	MoveToEx(hdcMem, 3, 9, NULL); 
	LineTo(hdcMem, 10, 2); 
	MoveToEx(hdcMem, 4, 9, NULL); 
	LineTo(hdcMem, 10, 3); 

	SelectObject(hdcMem, hbrOld); 

	// Clean up. 

	SelectObject(hdcMem, hbmOld); 
	DeleteDC(hdcMem); 
	::ReleaseDC(hwndDesktop, hdcDesktop); 

}


/**/
bool compare_by_modify_date(CNote const& left, CNote const& right)
{
	return left.GetModifiedDate() < right.GetModifiedDate();
}

/**/
void CTrayWnd::ModifyNotesMenu(CMenuHandle menuNotes)
{
	int nHiddenNotes = CApplication::Get().GetHiddenNotesCount();
	menuutils::SetMenuItemEnable(menuNotes, ID_POPUP_SHOWALLNOTES, nHiddenNotes > 0);
	CNote::List notes;
	CApplication::Get().GetAllNotes(notes, CApplication::NM_ID | CApplication::NM_TEXT | CApplication::NM_MODIFIED);
	std::sort(notes.begin(), notes.end(), compare_by_modify_date);

	/* check separator */
	int nState = menuNotes.GetMenuState(0, MF_BYPOSITION);
	if (notes.empty() && (nState & MF_SEPARATOR) == MF_SEPARATOR)
	{
		menuNotes.DeleteMenu(0, MF_BYPOSITION);
	}
	if (!notes.empty() && (nState & MF_SEPARATOR) != MF_SEPARATOR)
	{
		menuNotes.InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);
	}

	// show all notes
	for (int i = 0; i < notes.size(); ++i)
	{
		_tstring sCaption = GetCaption(notes[i].GetText());
		int nId = CREATE_NOTE_CMD(notes[i].GetId());
		menuNotes.InsertMenu(0, MF_BYPOSITION, nId, sCaption.c_str());
		menuNotes.SetMenuItemBitmaps(nId, MF_BYCOMMAND, 
			NULL, m_bmpDeleted); 
	}

	// show deleted notes
	CMenuHandle menuDeleted = GetUndeleteMenu(menuNotes);
	std::list<CNote> const& listDeleted = CApplication::Get().GetUndeleteList();
	if (!listDeleted.empty())
	{
		menuDeleted.DeleteMenu(ID_UNDELETE_EMPTY, MF_BYCOMMAND);
	}
	for(std::list<CNote>::const_iterator it = listDeleted.begin(); it != listDeleted.end(); ++it)
	{
		CNote const& note = *it;
		_tstring sCaption = GetCaption(note.GetText());
		menuDeleted.AppendMenu(MF_STRING, CREATE_DELETED_CMD(note.GetId()), sCaption.c_str());
	}
}
