// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __MAINFRM_H__
#define __MAINFRM_H__

#include <atlcrack.h>
#include "FrameWindowImplEx.h"
#include "FrameOptions.h"
#include "GuiUtils.h"
#include "Application.h"
#include "AssistFrame.h"
#include "controller.h"
#include "ModuleView.h"
#include "MainFrmModules.h"
#include "assistres.h"

const int WM_U_SET_VIEW_FOCUS	= WM_USER + 200;
const int WM_APP_EXIT			= WM_USER + 201;

class CModuleView;

class CMainFrame : public CFrameWindowImplEx<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler, 
		public CAssistFrame
{
public:
	CMainFrame(Controllers::CController& controller);
	virtual ~CMainFrame();

	DECLARE_FRAME_WND_CLASS(_T("Developer Assistant"), IDR_MAINFRAME)

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_FOLDER_PANE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_VIEW_DETAILS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_VIEW_VIEW_SELECTOR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ADDRESS, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		try
		{
			MESSAGE_HANDLER(WM_CREATE, OnCreate)
			MSG_WM_CLOSE(OnClose)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
			MESSAGE_HANDLER(WM_APP_EXIT, OnAppExit)
			MSG_WM_KEYDOWN(OnKeyDown)
			CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
			CHAIN_MSG_MAP(CFrameWindowImplEx<CMainFrame>)
			MESSAGE_HANDLER(WM_COMMAND, OnCommand)
			MESSAGE_HANDLER(WM_MENUSELECT, OnMenuSelect)
			MESSAGE_HANDLER(WM_SETUP, OnSetup)
			COMMAND_ID_HANDLER(ID_NEW_FOLDER, OnNewFolder)

			COMMAND_ID_HANDLER(ID_CLOSE, OnFileExit)
			COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
			COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
			COMMAND_ID_HANDLER(ID_VIEW_FOLDER_PANE, OnViewFolderPane)
			COMMAND_ID_HANDLER(ID_VIEW_VIEW_SELECTOR, OnViewViewSelector)
			COMMAND_ID_HANDLER(ID_VIEW_ADDRESS, OnViewAddressBar)

			NOTIFY_HANDLER(IDC_FOLDER_TREE, TVN_SELCHANGED, OnTreeSelChanged)
			COMMAND_HANDLER_EX(IDC_ADDRESS, CBN_DROPDOWN, OnCbnDropDown)
			COMMAND_HANDLER_EX(IDC_ADDRESS, CBN_SELENDOK, OnCbnSelEndOk)
			COMMAND_HANDLER_EX(IDC_ADDRESS, CBN_SELENDCANCEL, OnCbnSelEndCancel)
			COMMAND_ID_HANDLER_EX(ID_DELETE, OnItemsDelete)
			COMMAND_ID_HANDLER(ID_FILE_CLOSEALLDOCUMENTS, OnFileClosealldocuments)

			// tree view notifiers
			NOTIFY_HANDLER(IDC_FOLDER_TREE, TVN_KEYDOWN, OnTreeKey)
			NOTIFY_HANDLER_EX(IDC_FOLDER_TREE, TVN_ENDLABELEDIT, OnTvnEndLabelEdit)
			NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnTbBtnDropDown)

			//user messages
			MESSAGE_HANDLER(WM_U_SET_VIEW_FOCUS, OnSetViewFocus)
			MSG_WM_SIZE(OnSize)

			// switch current view
			if (uMsg == WM_COMMAND)
			{
				if (ProcessModuleCmd(wParam))
					return TRUE;
				if (ProcessModuleNewItemCmd(wParam))
					return TRUE;
			}
			COMMAND_ID_HANDLER(ID_NEW, OnNew)


			CHAIN_MSG_MAP_MEMBER(m_controller)
			// delegate command to active child view
			CHAIN_MSG_MAP(CAssistFrame)
		} 
		CATCH_ALL_ERRORS(m_hWnd)
	END_MSG_MAP()

// interface 

	virtual HWND GetHwnd() const;
	virtual HWND CreateView(HWND parent);
	virtual void Refresh(int code = 0, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual void OnRefreshed(IAssistView* pView, int code, WPARAM wParam = 0, LPARAM lParam = 0);

	virtual Module::IModuleInfo* GetCurModule();
	virtual DWORD GetCurFolder();
	virtual DWORD GetRootFolder(DWORD nModuleId);

private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	
	// events
	virtual void OnChildViewActivated(BOOL bActive, IAssistView* pView);

	// handlers
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnAppExit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMenuSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetup(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnNewFolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled);

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewFolderPane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewViewSelector(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewAddressBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	// tree view notifiers
	LRESULT OnTreeKey(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnTreeSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	void OnFolderChanged(DWORD nNewFolder);

	CModuleView* GetModuleView(DWORD mod_id);
	BOOL ActivateModuleView(DWORD mod_id);
	MODULE* GetCurrentModule();
	BOOL ProcessModuleCmd(DWORD mod_cmd);
	BOOL ProcessModuleNewItemCmd(DWORD mod_cmd);


	// user messages
	LRESULT OnSetViewFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	
	virtual void OnFinalMessage(HWND );

	// implementation
	BOOL AddModules();
	void RefreshFolders();
	void SwitchDetailsPane();

	void ShowFolderPane();
	void ShowToolBar();
	void ShowStatusBar();
	void ShowViewSelector();
	void ShowAddressBar();
	void SynchronizeFolders();
	void AddFolders(MODULE& module, HTREEITEM hParent = TVI_ROOT);
	void SetAddress();
	void ReplaceNewButtonBitmap(const MODULE& module);

	enum TSubMenus
	{
		smNewItem,
		smView,
		smRecent
	};
	CMenuHandle GetSubMenu(TSubMenus menu);

	// controls
	/*CFoldersTreeCtrl*/CTreeViewCtrlEx m_tree;
	CCommandBarCtrl m_CmdBar;
	CToolBarCtrl m_MainToolbar;
	CSplitterWindow m_splitter;
	CPaneContainer m_TreePane;
	CFrameOptions m_Options;
	CComboBoxEx m_AddressCombo;
	HWND m_hwndModulesToolbar;
	Controllers::CController& m_controller;
	CMainFrmModules m_modules;
	BOOL m_bAppExit;
	std::map<DWORD, int> m_mapNewButtonImages;
	MODULE* m_pLastActiveModule;
	BOOL m_bNoTreeSelProcessing;

public:
	LRESULT OnTvnEndLabelEdit(LPNMHDR pnmh);
	LRESULT OnTbBtnDropDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnClose(void);
	LRESULT OnSize(UINT state, CSize Size);
	LRESULT OnCbnDropDown(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnCbnSelEndOk(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnCbnSelEndCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnItemsDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl);
public:
	LRESULT OnFileClosealldocuments(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif //__MAINFRM_H__
