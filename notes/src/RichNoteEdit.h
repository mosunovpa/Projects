#pragma once

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
};
