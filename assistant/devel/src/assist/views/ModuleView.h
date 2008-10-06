/**
 * \file ModuleView.h
 *
 * \since 2006/02/06
 * \author pavel
 */
#ifndef ModuleViewImpl_h__
#define ModuleViewImpl_h__

#include "rsettings.h"
#include "resource.h"
#include "AssistViewImpl.h"
#include "MainFrmModules.h"
#include "Application.h"

//////////////////////////////////////////////////////////////////////////
// CModuleViewOptions

class CModuleViewOptions : public CRegSettings
{
public:
	DWORD SplitterPos;
	BOOL DetailsState;
	BEGIN_HKCU_REG_MAP(CModuleViewOptions, _T(""))
		REG_ITEM(SplitterPos, 0)
		REG_ITEM(DetailsState, TRUE)
	END_REG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// CModuleView

class CModuleView : public CHorSplitterWindow, public Views::CAssistViewImpl
{
public:
	DECLARE_WND_CLASS(_T("ModuleView"))

	CModuleView(MODULE* pModule);
	virtual ~CModuleView() {}

	BEGIN_MSG_MAP(CModuleView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SETUP, OnInit)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_WM_SETFOCUS(OnSetFocus)
		COMMAND_ID_HANDLER(ID_VIEW_DETAILS, OnViewDetailsPane)
		COMMAND_ID_HANDLER(ID_PANE_CLOSE, OnViewDetailsPane)

		CHAIN_MSG_MAP(Views::CAssistViewImpl)

		// filtering WM_COMMAND to avoid sending WM_COMMAND message back to the parent
		if (uMsg != WM_COMMAND)
			CHAIN_MSG_MAP(CHorSplitterWindow)

	END_MSG_MAP()

	virtual HWND SetFocus();
	virtual HWND GetHwnd() const;
	virtual HWND CreateView(HWND parent);
	virtual BOOL OnActivate(BOOL bActive);
	virtual void Refresh(int code = 0, WPARAM wParam = 0, LPARAM lParam = 0);

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnInit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnSetFocus(HWND wParam);
	LRESULT OnViewDetailsPane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	MODULE* GetModule() const { return m_pModule; }

protected:
	MODULE* m_pModule;

private:
	CModuleViewOptions m_Options;
//	CPaneContainer m_DetailsPane;
};

#endif // ModuleViewImpl_h__