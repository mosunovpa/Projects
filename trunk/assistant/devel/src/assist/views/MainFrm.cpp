/**
 * @file MainFrm.cpp
 *
 * implmentation of the CMainFrame class
 *
 * @author Pavel Mosunov
 * @since 2004-9-13
 */


#include "stdafx.h"
#include "MainFrm.h"
#include "atlwinmisc.h"
#include "guiutils.h"
#include "fileutils.h"
#include "Database.h"
#include "ApplicationImpl.h"
#include "ModuleLoader.h"
#include "Defines.h"
#include "FolderDlg.h"
#include "Items.h"
#include "utils.h"

enum BandsIds 
{ 
	enMenu = 1, 
	enMainToolbar = 2, 
	enModuleToolbar = 3, 
	enAddressBar = 4 
};

//////////////////////////////////////////////////////////////////////////
// CMainFrame

/**
 * Constructor
 */
CMainFrame::CMainFrame(Controllers::CController& controller)
:	m_controller(controller), 
	m_bAppExit(FALSE), 
	m_pLastActiveModule(NULL),
	m_bNoTreeSelProcessing(FALSE)
{
};

/**
 * Destructor.
 */
CMainFrame::~CMainFrame()
{
}

/**
 * 
 */
void CMainFrame::OnFinalMessage(HWND )
{
	delete this;
}

/**
 * CMainFrame::PreTranslateMessage
 */
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
////	if (::IsDialogMessage(m_hWnd, pMsg))
	//if (GuiUtils::IsTabstopProcessing(m_hWnd, pMsg))
	//{
	//	return TRUE;
	//}
	if(CFrameWindowImplEx<CMainFrame>::PreTranslateMessage(pMsg))
	{
		return TRUE;
	}

	return FALSE;
}

/**
 * CMainFrame::OnIdle
 */
BOOL CMainFrame::OnIdle()
{
	UIUpdateMenuBar();
	UIUpdateToolBar();
	return FALSE;
}

/**
 * CMainFrame::OnCreate
 */
LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	try
	{
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

 		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, 
 			ATL_SIMPLE_TOOLBAR_PANE_STYLE | TBSTYLE_LIST );
		
		m_MainToolbar = hWndToolBar;

 		SendMessage(hWndToolBar, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_MIXEDBUTTONS | TBSTYLE_EX_DRAWDDARROWS);

		TBBUTTONINFO bi;
		bi.cbSize = sizeof(TBBUTTONINFO);

		CString sTooltip = GuiUtils::GetTooltip(ID_NEW);
		bi.pszText = const_cast<LPTSTR>((LPCTSTR)sTooltip);
		bi.cchText = sTooltip.GetLength();
		bi.dwMask = TBIF_STYLE | TBIF_TEXT;
		bi.fsStyle = TBSTYLE_BUTTON | BTNS_SHOWTEXT | BTNS_DROPDOWN;
		SendMessage(hWndToolBar, TB_SETBUTTONINFO, (WPARAM)ID_NEW, (LPARAM)&bi);

		sTooltip = GuiUtils::GetTooltip(ID_VIEW_BACK);
		bi.pszText = const_cast<LPTSTR>((LPCTSTR)sTooltip);
		bi.cchText = sTooltip.GetLength();
		bi.dwMask = TBIF_STYLE | TBIF_TEXT;
		bi.fsStyle = TBSTYLE_BUTTON | BTNS_SHOWTEXT /*| BTNS_DROPDOWN*/;
		SendMessage(hWndToolBar, TB_SETBUTTONINFO, (WPARAM)ID_VIEW_BACK, (LPARAM)&bi);

		bi.dwMask = TBIF_STYLE;
		bi.fsStyle = TBSTYLE_BUTTON /*| BTNS_DROPDOWN*/;
		SendMessage(hWndToolBar, TB_SETBUTTONINFO, (WPARAM)ID_VIEW_FORWARD, (LPARAM)&bi);

		sTooltip = GuiUtils::GetTooltip(ID_VIEW_FOLDER_PANE);
		bi.pszText = const_cast<LPTSTR>((LPCTSTR)sTooltip);
		bi.cchText = sTooltip.GetLength();
		bi.dwMask = TBIF_STYLE | TBIF_TEXT;
		bi.fsStyle = TBSTYLE_BUTTON | BTNS_SHOWTEXT;
		SendMessage(hWndToolBar, TB_SETBUTTONINFO, (WPARAM)ID_VIEW_FOLDER_PANE, (LPARAM)&bi);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBandCtrl(m_hWndToolBar, hWndCmdBar, enMenu, NULL, TRUE);
		AddSimpleReBarBandCtrl(m_hWndToolBar, hWndToolBar, enMainToolbar, NULL, TRUE);
		m_AddressCombo.Create(m_hWndToolBar, CRect(0, 0, 0, 400), NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | CBS_DROPDOWNLIST | CCS_NODIVIDER | CCS_NOMOVEY | WS_TABSTOP,  
			WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_TOOLWINDOW | 
			CBES_EX_NOSIZELIMIT | CBES_EX_PATHWORDBREAKPROC,
			IDC_ADDRESS);

		AddSimpleReBarBandCtrl(m_hWndToolBar, m_AddressCombo.m_hWnd, enAddressBar, CResString<32>(IDS_ADDRESS).m_szStr, TRUE);
		CReBarCtrl rebar = m_hWndToolBar;

		m_Options.RestoreBand(rebar, enMenu);
		m_Options.RestoreBand(rebar, enMainToolbar);
		m_Options.RestoreBand(rebar, enAddressBar);

		m_hWndClient = m_splitter.Create(m_hWnd, rcDefault, NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT);
		m_splitter.SetSplitterExtendedStyle(0); // left aligned

		m_TreePane.Create(m_splitter, (LPCTSTR)NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
			WS_EX_CONTROLPARENT);
		m_TreePane.SetTitle(_T("Folders"));
		m_splitter.SetSplitterPane(SPLIT_PANE_LEFT, m_TreePane);

		// Set the tree view in the Pane
		m_tree.Create(m_TreePane, rcDefault, NULL,
			WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | WS_TABSTOP/*| TVS_EDITLABELS*/, 
			WS_EX_STATICEDGE, 
			IDC_FOLDER_TREE);
		m_TreePane.SetClient(m_tree);

		CreateSimpleStatusBar();

 		if (!AddModules()) //create left toolbar 
 		{
 			ThrowError(RESSTR(IDS_NO_MODULES_FOUND));
 		}

		UIAddMenuBar(m_CmdBar);
		UIAddToolBar(hWndToolBar);

		// register object for message filtering and idle updates
 		CMessageLoop* pLoop = _Module.GetMessageLoop();
 		ATLASSERT(pLoop != NULL);
 		pLoop->AddMessageFilter(this);
 		pLoop->AddIdleHandler(this);

		m_splitter.SetSplitterPos(m_Options.TreeWidth);
		m_splitter.SetDefaultActivePane(0);


		ShowFolderPane();
		ShowToolBar();
		ShowStatusBar();
		ShowViewSelector();
		ShowAddressBar();

		CMenuHandle hMruMenu = GetSubMenu(smRecent);

		PostMessage(WM_SETUP, (WPARAM)(HMENU)hMruMenu);

		CApplicationImpl::Get().SetMainView(this);
		bHandled = FALSE;
		return 0;
	}
	CATCH_ALL_ERRORS(m_hWnd)
	return -1;
}

/**
 * WM_DESTROY handler
 */
LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// destroy image lists
	m_MainToolbar.GetImageList().Destroy();
	CToolBarCtrl(m_hwndModulesToolbar).GetImageList().Destroy();
	CTreeViewCtrlEx(m_tree).GetImageList().Destroy();

	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	// save position only if window is in normal state
	if (!IsIconic() && !IsZoomed())
		m_Options.FramePos = CWindowRect(m_hWnd);

	if (!IsIconic())
		m_Options.Styles = GetStyle();

	m_Options.TreeWidth = m_splitter.GetSplitterPos();

	// store bands positions.

	m_Options.Bands.clear();
	m_Options.StoreBand(CReBarCtrl(m_hWndToolBar), enMenu);
	m_Options.StoreBand(CReBarCtrl(m_hWndToolBar), enMainToolbar);
	m_Options.StoreBand(CReBarCtrl(m_hWndToolBar), enAddressBar);

	CApplicationImpl::Get().CloseAllItemWindows();

	// default handler
	CApplicationImpl::Get().SetMainView(NULL);
	bHandled = FALSE;
	return 1;
}

/**
* Add modules to main application.
*/
BOOL CMainFrame::AddModules()
{
	CImageList ilist, ilist_sm;
	HBITMAP bm;

	std::vector<TBBUTTON> vTBBtn;
	int nNumber = 0;
	int nMewItemPos = 2;
	for (CMainFrmModules::iterator it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		IAssistView* pView = new CModuleView(&(*it));
		if (pView != NULL)
		{
			CModule& module = *(it->m_pModule);
			DWORD mod_id = module.pModuleInfo->GetId();
			LPCTSTR mod_name = module.pModuleInfo->GetName();
			AddChildView(pView);

			int module_cmd = m_modules.GetModuleCmd(mod_id);
			int item_module_cmd = m_modules.GetModuleItemCmd(mod_id);

			// insert item to the view menu
			bm = ::LoadBitmap(module.hModule, module.pModuleInfo->GetSmallBitmapRes());
			GetSubMenu(smView).AppendMenu(MF_STRING, module_cmd, mod_name);
			m_CmdBar.AddBitmap(bm, module_cmd);
			::DeleteObject(bm);

			// insert item to the new menu
			if (!CString(module.pModuleInfo->GetItemName()).IsEmpty())
			{
				bm = ::LoadBitmap(module.hModule, module.pModuleInfo->GetSmallBitmapRes());
				GetSubMenu(smNewItem).InsertMenu(
					nMewItemPos++, MF_BYPOSITION | MF_STRING, item_module_cmd, module.pModuleInfo->GetItemName());
				m_CmdBar.AddBitmap(bm, item_module_cmd);
				::DeleteObject(bm);
			}

			if (ilist.IsNull())
				ilist.Create(24, 24, ILC_COLOR24 | ILC_MASK, 4, 4);

			if (ilist_sm.IsNull())
				ilist_sm.Create(16, 16, ILC_COLOR24 | ILC_MASK, 4, 4);

			// add button to the document toolbar
			if (module.pModuleInfo->GetBitmapRes())
			{
				TBBUTTON tbb;
				tbb.iBitmap = nNumber++;
				tbb.idCommand = module_cmd;
				tbb.fsState = TBSTATE_ENABLED;
				tbb.fsStyle = TBSTYLE_BUTTON;
				tbb.dwData = 0;
				tbb.iString = (INT_PTR)mod_name;
				vTBBtn.push_back(tbb);

				// add bitmaps to image lists

				bm = ::LoadBitmap(module.hModule, module.pModuleInfo->GetBitmapRes());
				ilist.Add(bm, RGB(192, 192, 192));
				::DeleteObject(bm);
			}

			bm = ::LoadBitmap(module.hModule, module.pModuleInfo->GetSmallBitmapRes());
			ilist_sm.Add(bm, RGB(192, 192, 192));
			::DeleteObject(bm);
		}
	}

	if (!vTBBtn.empty())
	{
		m_hwndModulesToolbar = ::CreateWindowEx(0, TOOLBARCLASSNAME, NULL, ATL_SIMPLE_LEFT_TOOLBAR_PANE_STYLE, 0,0,100,100,
			m_hWnd, (HMENU)LongToHandle(ATL_IDW_TOOLBAR), ATL::_AtlBaseModule.GetModuleInstance(), NULL);
		if(m_hwndModulesToolbar == NULL)
		{
			ATLASSERT(FALSE);
			return FALSE;
		}
		::SendMessage(m_hwndModulesToolbar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0L);
		::SendMessage(m_hwndModulesToolbar, TB_SETIMAGELIST, 0, (LPARAM)(HIMAGELIST)ilist);
		::SendMessage(m_hwndModulesToolbar, TB_ADDBUTTONS, vTBBtn.size(), (LPARAM)&vTBBtn[0]);

		m_hWndLeftToolBar = CreateSimpleReBarCtrl(m_hWnd, ATL_SIMPLE_LEFT_REBAR_STYLE, ATL_IDW_TOOLBAR);
		::SendMessage(m_hwndModulesToolbar, TB_SETBITMAPSIZE, 0, MAKELONG(24, 24));
		::SendMessage(m_hwndModulesToolbar, TB_SETBUTTONSIZE, 0, MAKELONG(50, 50));
		AddSimpleReBarBandCtrl(m_hWndLeftToolBar, m_hwndModulesToolbar, 0);

		m_AddressCombo.SetImageList(ilist_sm);
		m_tree.SetImageList(ilist_sm);

		return TRUE;
	}
return FALSE;
}

/**
 * CMainFrame::ShowToolBar()
 */
void CMainFrame::ShowToolBar()
{
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(enMainToolbar);
	rebar.ShowBand(nBandIndex, m_Options.ToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, m_Options.ToolBar);
}

/**
* CMainFrame::ShowStatusBar()
*/
void CMainFrame::ShowStatusBar()
{
	::ShowWindow(m_hWndStatusBar, m_Options.StatusBar ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, m_Options.StatusBar);
	UpdateLayout();
}


/**
 * CMainFrame::ShowFolderPane()
 */
void CMainFrame::ShowFolderPane()
{
	m_splitter.SetSinglePaneMode(m_Options.FolderState ? SPLIT_PANE_NONE : SPLIT_PANE_RIGHT);
	UISetCheck(ID_VIEW_FOLDER_PANE, m_Options.FolderState);
}

/**
 * CMainFrame::OnCommand
 */
 LRESULT CMainFrame::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
 {
 	// if command comes from the close button, substitute HWND of the pane container instead
 	if(m_TreePane.m_hWnd != NULL && (HWND)lParam == m_TreePane.m_hWnd)
 		return SendMessage(WM_COMMAND, ID_VIEW_FOLDER_PANE);
 
 	bHandled = FALSE;
 	return 1;
 }

 LRESULT CMainFrame::OnMenuSelect(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
 {
	 WORD wFlags = HIWORD(wParam);
	 WORD wID = LOWORD(wParam);

	 if(m_hWndStatusBar == NULL/* || !m_modules.IsModuleCmd(wID)*/)
		 return 1;

	 if(wFlags == 0xFFFF && lParam == NULL)   // menu closing
	 {
		 ::SendMessage(m_hWndStatusBar, SB_SIMPLE, FALSE, 0L);
	 }
	 else
	 {
		 if (m_modules.IsModuleCmd(wID))
		 {
			 CString csPrompt;
			 if (!(wFlags & MF_POPUP))
			 {
				 CModuleView* pView = GetModuleView(m_modules.GetModuleId(LOWORD(wParam)));
				 Module::IModuleInfo* pModuleInfo = pView->GetModule()->m_pModule->pModuleInfo;
				 csPrompt.Format(IDS_ACTIVATE_MODULE, pModuleInfo->GetName());
			 }
			 ::SendMessage(m_hWndStatusBar, SB_SIMPLE, TRUE, 0L);
			 ::SendMessage(m_hWndStatusBar, SB_SETTEXT, (255 | SBT_NOBORDERS), (LPARAM)(LPCTSTR)csPrompt);
		 }
		 if (m_modules.IsModuleItemCmd(wID))
		 {
			 CString csPrompt;
			 if (!(wFlags & MF_POPUP))
			 {
				 CModuleView* pView = GetModuleView(m_modules.GetModuleItemId(LOWORD(wParam)));
				 Module::IModuleInfo* pModuleInfo = pView->GetModule()->m_pModule->pModuleInfo;
				 csPrompt.Format(IDS_CREATE_NEW_MODULE_ITEM, pModuleInfo->GetItemName());
			 }
			 ::SendMessage(m_hWndStatusBar, SB_SIMPLE, TRUE, 0L);
			 ::SendMessage(m_hWndStatusBar, SB_SETTEXT, (255 | SBT_NOBORDERS), (LPARAM)(LPCTSTR)csPrompt);
		 }
	 }
	return 1;
 }

/**
 * Notification from tree view. Tree view kyboard input processing.
 */
LRESULT CMainFrame::OnTreeKey(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN) pnmh;
// 	if (ptvkd->wVKey == VK_ESCAPE || ptvkd->wVKey == VK_RETURN) // escape || enter
// 	{
// 		PostMessage(WM_U_SET_VIEW_FOCUS, 0, 0);
// 		return 1;
// 	}

	if (ptvkd->wVKey == VK_DELETE)
	{
		SendMessage(WM_COMMAND, ID_DELETE);
		return 1;
	}
	if (ptvkd->wVKey == VK_TAB)
	{
		m_splitter.ActivateNextPane();
	}

	bHandled = FALSE;
	return 0;
}

/**
 * WM_U_SET_VIEW_FOCUS handler. Set focus to current view.
 */
LRESULT CMainFrame::OnSetViewFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	GetCurrentChildView()->SetFocus();
	return 0;
}

/**
 * CMainFrame::OnChildViewActivated()
 */
void CMainFrame::OnChildViewActivated(BOOL bActive, IAssistView* pView)
{
	CModuleView* pModuleView = dynamic_cast<CModuleView*>(pView);
	MODULE* pModule = pModuleView->GetModule();

	if (bActive)
	{
		if (!::IsWindow(pView->GetHwnd()))
		{
			pView->CreateView(m_splitter.m_hWnd);
		}
		::ShowWindow(pView->GetHwnd(), SW_SHOW);
		m_splitter.SetSplitterPane(SPLIT_PANE_RIGHT, pView->GetHwnd());

		SynchronizeFolders();
		if (pModule->m_pModule->pModuleInfo->GetId() != ID_TRASH)
			m_Options.ActiveModule = pModule->m_pModule->pModuleInfo->GetId();
	}
	else
	{
		::ShowWindow(pView->GetHwnd(), SW_HIDE);
		m_splitter.SetSplitterPane(SPLIT_PANE_RIGHT, NULL);
	}
	

	// select document type in toolbar
	::CheckMenuItem(GetSubMenu(smView), pModule->m_nCmd, 
		MF_BYCOMMAND | (bActive ? MF_CHECKED : MF_UNCHECKED));

	// select document type in menu
	::SendMessage(m_hwndModulesToolbar, TB_CHECKBUTTON, pModule->m_nCmd, MAKELPARAM(bActive, 0));

	if (bActive)
	{
		// change bitmap in toolbar
		ReplaceNewButtonBitmap(*pModule);
	}
}

void CMainFrame::ReplaceNewButtonBitmap(const MODULE& module)
{
	DWORD nModuleId = module.m_pModule->pModuleInfo->GetId();
	if (nModuleId == ID_TRASH)
		return;

	std::map<DWORD, int>::iterator it = m_mapNewButtonImages.find(nModuleId);
	int nImage = 0;
	if (it == m_mapNewButtonImages.end())
	{
		HBITMAP bm = ::LoadBitmap(module.m_pModule->hModule, module.m_pModule->pModuleInfo->GetSmallBitmapRes());
		nImage = m_MainToolbar.GetImageList().Add(bm, RGB(192, 192, 192));
		::DeleteObject(bm);
		m_mapNewButtonImages[nModuleId] = nImage;
	}
	else
	{
		nImage = it->second;
	}
	m_MainToolbar.ChangeBitmap(ID_NEW, nImage);


	HBITMAP bm = ::LoadBitmap(module.m_pModule->hModule, module.m_pModule->pModuleInfo->GetSmallBitmapRes());
	m_CmdBar.ReplaceBitmap(bm, ID_NEW);
	::DeleteObject(bm);

	m_pLastActiveModule = const_cast<MODULE*>(&module);
}


/**
 * 
 */
void CMainFrame::SynchronizeFolders()
{
	if (GetCurrentModule() == 0)
		return;

	int nCurrFolder = m_modules.GetModule(GetCurrentModule()->m_pModule->pModuleInfo->GetId())->m_nCurrentFolder;
	if (nCurrFolder == 0)
		return;
		
	CTreeItem ti = m_tree.GetSelectedItem();
	if (!ti.IsNull() && ti.GetData() == nCurrFolder)
	{
		return;
	}

	class CTreeEnumerator : public GuiUtils::CTreeCtrlEnumerator
	{
	public:
		CTreeEnumerator(int nCurrFolder)
		:	m_nCurrFolder(nCurrFolder) {}

		virtual BOOL OnTreeItem(CTreeViewCtrl& tree, HTREEITEM hItem, int level)
		{
			if (tree.GetItemData(hItem) == m_nCurrFolder && tree.GetSelectedItem() != hItem)
			{
				tree.SelectItem(hItem);
				return FALSE;
			}
			return TRUE;
		}
	private:
		int m_nCurrFolder;

	} enumerator(nCurrFolder);

	enumerator.EnumTree(m_tree);
}

/**
* CMainFrame::ShowViewSelector()
*/
void CMainFrame::ShowViewSelector()
{
	::ShowWindow(m_hWndLeftToolBar, m_Options.ViewSelector ? SW_SHOW : SW_HIDE);
 	UISetCheck(ID_VIEW_VIEW_SELECTOR, m_Options.ViewSelector);
	UpdateLayout();
}

/**
 * 
 */
void CMainFrame::ShowAddressBar()
{
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(enAddressBar);
	rebar.ShowBand(nBandIndex, m_Options.ViewAddressBar);
	UISetCheck(ID_VIEW_ADDRESS, m_Options.ViewAddressBar);
}

/**
* 
*/
HWND CMainFrame::GetHwnd() const
{
	return m_hWnd;
}

/**
* 
*/
HWND CMainFrame::CreateView(HWND parent)
{
	HWND frame = CreateEx(parent, m_Options.FramePos, m_Options.Styles | WS_OVERLAPPEDWINDOW, WS_EX_CONTROLPARENT);
	if (frame != NULL)
	{
		int nCmdShow = SW_SHOWDEFAULT;
		if ((m_Options.Styles & WS_MAXIMIZE) == WS_MAXIMIZE)
			nCmdShow = SW_SHOWMAXIMIZED;

		ShowWindow(nCmdShow);
	}
	return frame;
}


/**
* CMainFrame::OnViewFolderPane
*/
LRESULT CMainFrame::OnViewFolderPane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Options.FolderState = !m_Options.FolderState;
	ShowFolderPane();
	return 0;
}


/**
* CMainFrame::OnViewViewSelector()
*/
LRESULT CMainFrame::OnViewViewSelector(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Options.ViewSelector = !m_Options.ViewSelector;
	ShowViewSelector();
	return 0;
}

/**
* 
*/
LRESULT CMainFrame::OnViewAddressBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Options.ViewAddressBar = !m_Options.ViewAddressBar;
	ShowAddressBar();
	return 0;
}

/**
* CMainFrame::OnViewStatusBar
*/
LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Options.StatusBar = !m_Options.StatusBar;
	ShowStatusBar();
	return 0;
}

/**
* CMainFrame::OnViewToolBar
*/
LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Options.ToolBar = !m_Options.ToolBar;
	ShowToolBar();
	return 0;
}

/**
* 
*/
void CMainFrame::Refresh(int code, WPARAM wParam, LPARAM lParam)
{
	switch (code/*wParam*/)
	{
	case UNM_FILE_OPENED:
		{
			_tstring file_name = fileutils::file_name((LPCTSTR)wParam/*lParam*/) 
				+ _T(" - ") + RESSTR(IDR_MAINFRAME);
			SetWindowText(file_name.c_str());

			RefreshFolders();

			// if module has not been selected yet
			if (GetCurrentModule() == 0)
			{
				if (!ActivateModuleView(m_Options.ActiveModule))
				{
					ActivateChildView(m_Views.front());
				}
			}
			else
			{
				// if module selected, only synchronize folders
				SynchronizeFolders();
			}
		}
		break;

	case UNM_ITEM_ADDED:
		if (lParam == GetCurrentModule()->m_nCurrentFolder) // item in current folder
		{
			GetCurrentChildView()->Refresh(UNM_ITEM_ADDED, wParam, lParam);
		}
		break;

	case UNM_ITEM_DELETED:
		GetCurrentChildView()->Refresh(UNM_ITEM_DELETED, wParam, lParam);
		break;

	case UNM_FOLDER_ADDED:
		{
			Assist::IItemIteratorPtr items = CItems::GetItem(wParam);
			int mod_id = items->GetModuleId();
			HTREEITEM hItem = GuiUtils::CTreeCtrlEnumerator::FindByData(m_tree, lParam);
			assert(hItem);
			if (hItem)
			{
				int nImage = m_modules.GetModule(mod_id)->m_nImage;
				HTREEITEM hNewItem = m_tree.InsertItem(items->GetName(), nImage, nImage, hItem, TVI_LAST);
				if (hNewItem)
				{
					m_tree.SetItemData(hNewItem, wParam);
					m_tree.SelectItem(hNewItem);
					m_tree.SetFocus();
				}
			}
		}
		break;

	case UNM_FOLDER_DELETED:
		{
			HTREEITEM hItem = GuiUtils::CTreeCtrlEnumerator::FindByData(m_tree, wParam);
			if (hItem)
			{
				m_tree.DeleteItem(hItem);
			}
			if (GetCurrentModule()->m_pModule->pModuleInfo->GetId() == ID_TRASH) // refresh trash
			{
				GetCurrentChildView()->Refresh(UNM_ITEM_DELETED, wParam, lParam);
			}
		}
		break;
	}
}

/**
 * 
 */
void CMainFrame::OnRefreshed(IAssistView* pView, int code, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/) 
{
	switch (code)
	{
	case UNM_DETAILS_SHOWN:
		UISetCheck(ID_VIEW_DETAILS, (BOOL)wParam);
		break;
	}
}

/**
 * 
 */
void CMainFrame::AddFolders(MODULE& module, HTREEITEM hParent /*= TVI_ROOT*/)
{
 	Assist::IItemIteratorPtr folder = CItems::SelectFolders(module.m_pModule->pModuleInfo->GetId(),
 		hParent == TVI_ROOT ? 0 : m_tree.GetItemData(hParent));

	while (folder->exec())
	{
		// insert item to the tree
		HTREEITEM hTreeItem = m_tree.InsertItem(folder->GetName(), module.m_nImage, module.m_nImage, hParent, TVI_LAST);
		m_tree.SetItemData(hTreeItem, folder->GetId());

 		if (module.m_nCurrentFolder == 0)
 			module.m_nCurrentFolder = folder->GetId();

		AddFolders(module, hTreeItem);
	}
}

/**
 * 
 */
void CMainFrame::RefreshFolders()
{
	Utils::CRestoredValueT<BOOL> var(m_bNoTreeSelProcessing, TRUE);
	m_modules.Reset();
	m_tree.DeleteAllItems();
	for (CMainFrmModules::iterator it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		AddFolders(*it);
	}
}

/**
 * 
 */
LRESULT CMainFrame::OnTreeSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if (m_bNoTreeSelProcessing)
	{
		return 0;
	}
	CTreeItem ti = m_tree.GetSelectedItem();
	DWORD nFolder = m_tree.GetItemData(ti);
	if (nFolder)
	{
		OnFolderChanged(nFolder);
		ti.Expand();
		SetAddress();
		GetCurrentChildView()->Refresh(UNM_FOLDER_CHANGED, nFolder);
	}
	return 0;
}

/**
 * 
 */
void CMainFrame::OnFolderChanged(DWORD nNewFolder)
{
	MODULE* pCurModule = GetCurrentModule();
	if (pCurModule == NULL)
		return;

	if (nNewFolder == pCurModule->m_nCurrentFolder)
		return;

	Assist::IItemIteratorPtr items = CItems::GetItem(nNewFolder);
	int nNewFolderModId = items->GetModuleId();
	m_modules.GetModule(nNewFolderModId)->m_nCurrentFolder = nNewFolder;

	if (pCurModule->m_pModule->pModuleInfo->GetId() != nNewFolderModId)
	{
		ActivateModuleView(nNewFolderModId);
	}
	else
	{
		SynchronizeFolders();
	}	
	return;
}

/**
* Returns first view from module view list.
*/
CModuleView* CMainFrame::GetModuleView(DWORD mod_id)
{
	MODULE* pModule = m_modules.GetModule(mod_id);
	if (!pModule)
		return NULL;

	if (!pModule->m_pModule)
		return NULL;

	for (Views::CViewList::const_iterator it = m_Views.begin(); it != m_Views.end(); ++it)
	{
		CModuleView* pView = dynamic_cast<CModuleView*>(*it);
		if (pView != NULL && pView->GetModule() == pModule)
		{
			return pView;
		}
	}

	return NULL;
}

/**
 * 
 */
BOOL CMainFrame::ActivateModuleView(DWORD mod_id)
{
	CModuleView* pView = GetModuleView(mod_id);
	if (pView != NULL)
		return ActivateChildView(pView);

	return FALSE;
}

/**
 * 
 */
MODULE* CMainFrame::GetCurrentModule()
{
	CModuleView* pModuleView = dynamic_cast<CModuleView*>(GetCurrentChildView());
	if (pModuleView)
		return pModuleView->GetModule();

	return NULL;
}

/**
 * 
 */
BOOL CMainFrame::ProcessModuleCmd(DWORD mod_cmd)
{
	DWORD mod_id = m_modules.GetModuleId(mod_cmd);
	if (mod_id && ActivateModuleView(mod_id))
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * 
 */
BOOL CMainFrame::ProcessModuleNewItemCmd(DWORD mod_cmd)
{
	DWORD mod_id = m_modules.GetModuleItemId(mod_cmd);
	if (mod_id > 0)
	{
		MODULE* pModule = m_modules.GetModule(mod_id);
		int nParentId = pModule->m_nCurrentFolder;
		if (pModule != m_pLastActiveModule)
		{
			nParentId = GetRootFolder(pModule->m_pModule->pModuleInfo->GetId());
		}
		pModule->m_pModule->pModuleInfo->NewItem(nParentId);
		return TRUE;
	}
	return FALSE;
}

/**
 * WM_SETUP
 */
LRESULT CMainFrame::OnSetup(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	CApplicationImpl::Get().m_mru.SetMenuHandle((HMENU)wParam);
	CApplicationImpl::Get().m_mru.UpdateMenu();

	if (!CApplicationImpl::Get().IsDataFileOpened())
	{
		DestroyWindow();
	}
	m_tree.SetFocus();
	return 0;
}

/**
 * 
 */
LRESULT CMainFrame::OnNewFolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CTreeItem ti = m_tree.GetSelectedItem();
	CString csFolderName = RESSTR(IDS_NEW_FOLDER);
	if (!ti.IsNull() && CFolderDlg::GetFolderName(csFolderName) == IDOK)
	{
		int nNewFolder = CApplicationImpl::Get().AddFolder(csFolderName, ti.GetData());
	}
	return 0;
}

/**
 
 */
LRESULT CMainFrame::OnNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
	assert(m_pLastActiveModule);
	int nParentId = m_pLastActiveModule->m_nCurrentFolder;
	if (GetCurrentModule() != m_pLastActiveModule)
	{
		nParentId = GetRootFolder(m_pLastActiveModule->m_pModule->pModuleInfo->GetId());
	}
	m_pLastActiveModule->m_pModule->pModuleInfo->NewItem(nParentId);
	return 0;
}

/**
 * 
 */
LRESULT CMainFrame::OnTvnEndLabelEdit(LPNMHDR pnmh)
{
	LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pnmh;
	TVITEM tvi = ptvdi->item;

	if (!CString(tvi.pszText).Trim().IsEmpty())
	{
		CApplicationImpl::Get().RenameFolder(tvi.pszText, m_tree.GetItemData(tvi.hItem));
		m_tree.SetItemText(tvi.hItem, tvi.pszText);
	}

	return FALSE; // Reject the editing text and revert to the original label
}

/**
 * 
 */
LRESULT CMainFrame::OnTbBtnDropDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMTOOLBAR lpnmtb = (LPNMTOOLBAR)pnmh;	
	if (lpnmtb->iItem == ID_NEW)
	{
		CMenuHandle hNewMenu = GetSubMenu(smNewItem);
		CPoint pt(lpnmtb->rcButton.left, lpnmtb->rcButton.bottom);
		::ClientToScreen(lpnmtb->hdr.hwndFrom, &pt);
		m_CmdBar.TrackPopupMenu(hNewMenu, 
			TPM_LEFTBUTTON | TPM_VERTICAL | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NOANIMATION, 
			pt.x, pt.y);
	}
	return TBDDRET_DEFAULT;
}

/**
 * 
 */
CMenuHandle CMainFrame::GetSubMenu(TSubMenus menu)
{
	CMenuHandle hMenu;
	switch (menu)
	{
	case smNewItem: 
		hMenu = m_CmdBar.GetMenu().GetSubMenu(0).GetSubMenu(0); 
		break;
	case smView:
		hMenu = m_CmdBar.GetMenu().GetSubMenu(2);
		break;
	case smRecent:
		hMenu = m_CmdBar.GetMenu().GetSubMenu(0).GetSubMenu(4);
		break;
	default:
		ThrowError(_T("Unknown submenu"));
	}
	return hMenu;
}

/**
 * 
 */
LRESULT CMainFrame::OnClose(void)
{
	if (m_bAppExit)
		SetMsgHandled(FALSE);
	else
#if 1
		CApplicationImpl::Get().PostCloseAppWindow();
#else
		ShowWindow(SW_MINIMIZE);
#endif
	return 0;
}

/**
 * 
 */
LRESULT CMainFrame::OnAppExit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_bAppExit = TRUE;
	SendMessage(WM_CLOSE);
	return 0;
}

/**/
void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB)
	{
		m_splitter.ActivateNextPane();
	}
}

/**
* CMainController::OnFileExit()
*/
LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

/**
 * 
 */
LRESULT CMainFrame::OnSize(UINT state, CSize Size)
{
	if (state == SIZE_MINIMIZED)
		ShowWindow(SW_HIDE);
	else if (state == SIZE_RESTORED)
		ShowWindow(SW_SHOW);
	return 0;
}

/**
 * 
 */
void CMainFrame::SetAddress()
{
	MODULE* pModule = GetCurrentModule();
	CTreeItem ti = m_tree.GetSelectedItem();
	m_AddressCombo.ResetContent();
	int nIndex = m_AddressCombo.InsertItem(0, GuiUtils::GetSelectedTreeItemPath(m_tree).c_str(), pModule->m_nImage, pModule->m_nImage, 0);
	m_AddressCombo.SetCurSel(nIndex);
}

/**
 * 
 */
LRESULT CMainFrame::OnCbnDropDown(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	m_AddressCombo.ResetContent();
	std::vector<HTREEITEM> Ids;
	GuiUtils::GetSelectedTreeItemPath(m_tree, Ids);

	class CTreeEnumerator : public GuiUtils::CTreeCtrlEnumerator
	{
	public:
		CTreeEnumerator(CComboBoxEx& Combo, std::vector<HTREEITEM>& Ids)
			:	m_Combo(Combo), m_Ids(Ids)
		{
		}
		virtual BOOL OnTreeItem(CTreeViewCtrl& tree, HTREEITEM hItem, int level)
		{
			_tstring sItemText;
			sItemText.resize(256);
			tree.GetItemText(hItem, &sItemText[0], 255);
			int nImage, nSelectedImage;
			tree.GetItemImage(hItem, nImage, nSelectedImage);
			int nIndex = m_Combo.InsertItem(m_Combo.GetCount(), sItemText.c_str(), nImage, nSelectedImage, level);
			m_Combo.SetItemData(nIndex, (DWORD)hItem);
			if (tree.GetSelectedItem() == hItem)
			{
				m_Combo.SetCurSel(nIndex);
			}
			return TRUE;
		}
		virtual BOOL OnIsTreeItemChildProcessing(CTreeViewCtrl& tree, HTREEITEM hItem, int level)
		{
			return (std::find(m_Ids.begin(), m_Ids.end(), hItem) != m_Ids.end()) || 
				!tree.GetParentItem(hItem);
		}
	private:
		CComboBoxEx& m_Combo;
		std::vector<HTREEITEM>& m_Ids;

	} enumerator(m_AddressCombo, Ids);

	enumerator.EnumTree(m_tree);
	return 0;
}

LRESULT CMainFrame::OnCbnSelEndOk(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	m_tree.SelectItem((HTREEITEM)m_AddressCombo.GetItemData(m_AddressCombo.GetCurSel()));
	SetAddress();
	return 0;
}

LRESULT CMainFrame::OnCbnSelEndCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	SetAddress();
	return 0;
}

LRESULT CMainFrame::OnItemsDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	HWND focused_hwnd = GetFocus();
	if (focused_hwnd == m_tree.m_hWnd || focused_hwnd == m_AddressCombo.GetComboCtrl().m_hWnd)
	{
		CTreeItem sel_item = m_tree.GetSelectedItem();
		if (!sel_item.IsNull())
		{
			int nId = (int)sel_item.GetData();
			CApplicationImpl::Get().DeleteItems(&nId, 1);
		}
	}
	else
	{
		SetMsgHandled(FALSE);
	}

	return 0;
}

LRESULT CMainFrame::OnFileClosealldocuments(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CApplicationImpl::Get().CloseAllItemWindows();
	return 0;
}

Module::IModuleInfo* CMainFrame::GetCurModule()
{
	return GetCurrentModule()->m_pModule->pModuleInfo;
}

DWORD CMainFrame::GetCurFolder()
{
	return GetCurrentModule()->m_nCurrentFolder;
}

DWORD CMainFrame::GetRootFolder(DWORD nModuleId)
{
	MODULE* pModule = m_modules.GetModule(nModuleId);
	Assist::IItemIteratorPtr item = CItems::SelectFolders(nModuleId, 0);
	if (item->exec())
	{
		return item->GetId();
	}
	ThrowError(_T("Root folder is not found"));
	return -1;
}
