/**
 * \file ListCtrlWindowImpl.h
 *
 * \since 2006/12/20
 * \author pavel
 */

#pragma once

#include <atlcrack.h>
#include "AssistViewImpl.h"
#include "guiutils.h"
#include "assistres.h"
#include "controls.h"
#include "ModuleInfo.h"

#define ID_LISTCTRL 100

namespace Views
{
template <class TBase>
class CListCtrlWindowImpl : public CWindowImpl<TBase>
{
public:
	CListCtrlWindowImpl(LPCTSTR szKey, Module::IModuleInfo* pModule) 
		:	m_listCtrlOptions(szKey),
			m_spModule(pModule)
	{
	}

	typedef CWindowImpl<TBase>	thisClass;
	BEGIN_MSG_MAP(thisClass)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SIZE(OnSize)
		MSG_WM_ERASEBKGND(OnErasebkgnd)
		MSG_WM_SETFOCUS(OnSetFocus)
		COMMAND_ID_HANDLER(ID_NEW, OnNew)
		COMMAND_ID_HANDLER(ID_OPEN, OnOpen)
		COMMAND_ID_HANDLER(ID_DELETE, OnDelete)
		NOTIFY_CODE_HANDLER_EX(LVN_KEYDOWN, OnLvnKeyDown)
	END_MSG_MAP()

protected:
	virtual void AddColumns() = 0;
	virtual void AddBitmaps() = 0;


	LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		m_listCtrl.Create(m_hWnd, NULL, NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN 
				| LVS_REPORT | LVS_SHOWSELALWAYS | WS_TABSTOP, 
			WS_EX_CLIENTEDGE | LVS_EX_DOUBLEBUFFER, 
			ID_LISTCTRL);

		AddColumns();

		CImageList ilist;
		ilist.Create(16, 16, ILC_COLOR24 | ILC_MASK, 4, 4);
		m_listCtrl.SetImageList(ilist, LVSIL_SMALL);

		AddBitmaps();

		// restore columns
		m_listCtrlOptions.RestoreColumns(m_listCtrl);

		SetMsgHandled(FALSE);
		return 0;
	}

	LRESULT OnDestroy(void)
	{
		// store columns
		m_listCtrlOptions.StoreColumns(m_listCtrl);

		m_listCtrl.GetImageList(LVSIL_SMALL).Destroy();

		SetMsgHandled(FALSE);
		return 0;
	}

	LRESULT OnSize(UINT state, CSize size)
	{
		CRect rc;
		GetClientRect(&rc);
		m_listCtrl.MoveWindow(rc, TRUE);
		SetMsgHandled(FALSE);
		return 0;
	}

	LRESULT OnErasebkgnd(HDC hdc)
	{
		return TRUE;
	}

	void OnSetFocus(HWND wParam)
	{
		::SetFocus(m_listCtrl.m_hWnd);
	}

	LRESULT OnNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_spModule->NewItem(0);
		return 0;
	}

	LRESULT OnOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		std::vector<int> vSelectedItems;
		GetSelectedItems(vSelectedItems);
		for (int i = 0; i != vSelectedItems.size(); ++i)
		{
			m_spModule->OpenItem(vSelectedItems[0]);
		}
		return 0;
	}

	LRESULT OnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		std::vector<int> vSelectedItems;
		GetSelectedItems(vSelectedItems);
		if (!vSelectedItems.empty() && !m_spModule.is_null())
		{
			m_spModule->DeleteItems(&vSelectedItems[0], vSelectedItems.size());
		}
		return 0;	
	}

	LRESULT OnLvnKeyDown(LPNMHDR pnmh)
	{
		LPNMLVKEYDOWN pnkd = (LPNMLVKEYDOWN)pnmh;
		if (pnkd->wVKey == VK_DELETE)
		{
			SendMessage(WM_COMMAND, ID_DELETE);
		}
		else if (pnkd->wVKey == VK_RETURN)
		{
			SendMessage(WM_COMMAND, ID_OPEN);
		}
		else if (pnkd->wVKey == VK_TAB)
		{
			GetParent().SendMessage(WM_KEYDOWN, VK_TAB); // forward to parent for tab navigation
		}
		return 0;
	}

	void GetSelectedItems(OUT std::vector<int>& vSelectedItems)
	{
		// clear all selections
		for (int i = 0; i < m_listCtrl.GetItemCount(); ++i)
		{
			if ((m_listCtrl.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED) == LVIS_SELECTED)
			{
				vSelectedItems.push_back(m_listCtrl.GetItemData(i));
			}
		}
	}

	CListViewCtrlEx m_listCtrl;
	GuiUtils::CListCtrlOptions m_listCtrlOptions;
	safe_ptr<Module::IModuleInfo> m_spModule;
};

class CListCtrlWindow : public CListCtrlWindowImpl<CListCtrlWindow>, public CAssistViewImpl
{
public:
	DECLARE_WND_CLASS(NULL)

	CListCtrlWindow(LPCTSTR szKey, Module::IModuleInfo* pModule) 
		: CListCtrlWindowImpl<CListCtrlWindow>(szKey, pModule) {}

	BEGIN_MSG_MAP(CListCtrlWindow)
		CHAIN_MSG_MAP(CListCtrlWindowImpl<CListCtrlWindow>)
		CHAIN_MSG_MAP(CAssistViewImpl)
	END_MSG_MAP()

	virtual HWND GetHwnd() const { return m_hWnd; }
	virtual HWND CreateView(HWND parent)
	{
		return Create(parent, rcDefault,_T(""), WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);
	}
};

} // namespace Views
