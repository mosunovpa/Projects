#pragma once
#include <boost/algorithm/string/trim.hpp>

class CRichNoteEdit : public CWindowImpl<CRichNoteEdit, CRichEditCtrl>,
                    public CRichEditCommands<CRichNoteEdit>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())
    BEGIN_MSG_MAP(CRichNoteEdit)
		MESSAGE_HANDLER_EX(WM_KEYDOWN, OnKeyDown)
        CHAIN_MSG_MAP_ALT(CRichEditCommands<CRichNoteEdit>, 1)
    END_MSG_MAP()

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (wParam == VK_ESCAPE)
		{
			GetParent().PostMessage(WM_CLOSE);
		}
		if (wParam == 68 && GetKeyState(VK_CONTROL) & 0x8000) // Ctrl+D
		{
			GetParent().PostMessage(WM_COMMAND, ID_REMOVE);
		}
		SetMsgHandled(FALSE);
		return 0;
	}

	class CRichEditOleCallback : public IRichEditOleCallback
	{
	public:
		STDMETHOD_(ULONG, AddRef)() 
		{ 
			return 1; 
		}
		STDMETHOD_(ULONG, Release)() 
		{ 
			return 1; 
		}
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObj)
		{
			if (riid == IID_IUnknown || riid == IID_IRichEditOleCallback)  
			{  
				*ppvObj = (void*)this;  
				return S_OK;  
			}  
			else  
			{  
				*ppvObj   =   NULL;  
				return   E_NOINTERFACE;  
			} 
		}
		STDMETHOD(GetNewStorage)(LPSTORAGE* lplpstg)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(GetInPlaceContext)(LPOLEINPLACEFRAME*, LPOLEINPLACEUIWINDOW*, LPOLEINPLACEFRAMEINFO)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(ShowContainerUI)(BOOL)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(QueryInsertObject)(LPCLSID, LPSTORAGE, LONG)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(DeleteObject)(LPOLEOBJECT)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(QueryAcceptData)(LPDATAOBJECT, CLIPFORMAT* pcfFormat, DWORD,BOOL, HGLOBAL)
		{
			*pcfFormat = CF_TEXT;
			return S_OK;  
		}
		STDMETHOD(ContextSensitiveHelp)(BOOL)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(GetClipboardData)(CHARRANGE*, DWORD, LPDATAOBJECT*)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(GetDragDropEffect)(BOOL, DWORD, LPDWORD)
		{
			return E_NOTIMPL;  
		}
		STDMETHOD(GetContextMenu)(WORD, LPOLEOBJECT, CHARRANGE*, HMENU* hMenu)
		{
			CMenuHandle hEditMenu;
			hEditMenu.LoadMenu(IDR_EDIT);
			CMenuHandle menuPopup = hEditMenu.GetSubMenu(0);
			*hMenu = menuPopup.m_hMenu;
			return S_OK;  
		}
	} m_OleCallback;


	void Create(HWND hWndParent) 
	{
		CWindowImpl<CRichNoteEdit, CRichEditCtrl>::Create(hWndParent, NULL, NULL, 
			WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL, 
			WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY, 20000);
	}

	void Init()
	{
		SetBackgroundColor(RGB(255, 255, 204));

		COptions::FontSize fs = CApplication::Get().GetOptions().GetFontSize();
		CHARFORMAT cf;
		ZeroMemory(&cf, sizeof(CHARFORMAT));
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_SIZE | CFM_BOLD | CFM_FACE;
		GetDefaultCharFormat(cf);
		cf.yHeight = (fs == COptions::FS_SMALL ? 160 : (fs == COptions::FS_MEDIUM ? 200 : 240));
		cf.dwEffects = 0;
		lstrcpy(cf.szFaceName, _T("MS Shell Dlg"));
		SetDefaultCharFormat(cf);

		// format changing lead to increase undo queue

		PARAFORMAT pf;
		ZeroMemory(&pf, sizeof(PARAFORMAT));
		pf.cbSize = sizeof(PARAFORMAT);
		pf.dwMask = PFM_OFFSETINDENT;
		pf.dxStartIndent = 100;
		SetParaFormat(pf);

		SetOleCallback(&m_OleCallback);
		SetEventMask(ENM_LINK);
		SetAutoURLDetect();
	}

	void SetFontSize(LONG lHeight)
	{
		CHARFORMAT cf;
		ZeroMemory(&cf, sizeof(CHARFORMAT));
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_SIZE | CFM_BOLD | CFM_FACE;
		GetDefaultCharFormat(cf);
		cf.yHeight = lHeight;
		cf.dwEffects = 0;
		lstrcpy(cf.szFaceName, _T("MS Shell Dlg"));
		SetDefaultCharFormat(cf);
	}

	_tstring GetText()
	{
		_tstring s;
		int len = GetWindowTextLength() + 1;
		s.resize(len);
		GetWindowText(&s[0],len);
		boost::trim(s);
		if (s[0] == 0)
		{
			s.clear();
		}
		return _tstring(s.c_str());
	}

	void SetText(_tstring const& text)
	{
		SetWindowText(text.c_str());
	}

	void SetSavePoint()
	{
		SetModify(FALSE);
	}
};
