/**
 * \file HiddenWnd.h
 *
 * \since 2006/10/05
 * \author pavel
 */

#pragma once

#include "TrayIcon.h"
#include "guiutils.h"
#include "MainController.h"

class CHiddenWnd : public CWindowImpl<CHiddenWnd> 
{
public:
	CHiddenWnd(void);
	~CHiddenWnd(void);

private:
	CTrayIcon m_TrayIcon;
	HICON m_hIconNotes;
	CMenu m_menuPopup;				// top-level popup menu
	CMainController m_controller;

	BEGIN_MSG_MAP(CHiddenWnd)
		try
		{
			MESSAGE_HANDLER(WM_CREATE, OnCreate)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
			MESSAGE_HANDLER(WMU_NOTIFYICON, OnNotifyIcon)
			MESSAGE_HANDLER(WM_QUERYENDSESSION, OnEndSession)
			COMMAND_ID_HANDLER(ID_EXPLORE, OnExplore)
			COMMAND_ID_HANDLER(ID_APP_EXIT, OnAppExit)
		} 
		CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAppExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnExplore(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT DisplayShortcutMenu();
	void ShowExplorer();

};
