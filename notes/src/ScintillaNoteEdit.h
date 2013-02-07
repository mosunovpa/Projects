#pragma once

#include "atlscintilla.h"
#include <boost/algorithm/string/trim.hpp>

class CScintillaNoteEdit : public CScintillaWindowImpl<CScintillaNoteEdit>,
                public CEditCommands<CScintillaNoteEdit>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CScintilla::GetWndClassName())
	BEGIN_MSG_MAP(CScintillaNoteEdit)
		MESSAGE_HANDLER_EX(WM_KEYDOWN, OnKeyDown)
		CHAIN_MSG_MAP(CScintillaWindowImpl<CScintillaNoteEdit>)
		CHAIN_MSG_MAP_ALT(CEditCommands<CScintillaNoteEdit>, 1)
		COMMAND_ID_HANDLER(ID_EDIT_REDO, OnEditRedo)
	END_MSG_MAP()

    void Create(HWND hWndParent) 
	{
		CScintillaWindowImpl<CScintillaNoteEdit>::Create(hWndParent, NULL, NULL, 
			WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL, 
			WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY, 20000);
	}

	void Init(COLORREF bgColor)
	{
		SetCodePage(SC_CP_UTF8);//SC_CP_UTF8
		SetWrapMode(SC_WRAP_WORD);
		SetWrapIndentMode(SC_WRAPINDENT_SAME);
		SetWrapVisualFlags(SC_WRAPVISUALFLAG_END);
		COptions::FontSize fs = CApplication::Get().GetOptions().GetFontSize();
		SetAStyle(STYLE_DEFAULT, bgColor, RGB(0,0,0), 
			(fs == COptions::FS_SMALL ? 8 : (fs == COptions::FS_MEDIUM ? 10 : 12))
			, NULL/*"MS Shell Dlg"*/);
		StyleClearAll();
		SetMarginTypeN(1, SC_MARGIN_BACK);

		AssignCmdKey(SCK_HOME, SCI_VCHOMEWRAP);
		AssignCmdKey(SCK_END, SCI_LINEENDWRAP);
		AssignCmdKey(SCK_HOME + (SCMOD_SHIFT << 16), SCI_VCHOMEWRAPEXTEND);
		AssignCmdKey(SCK_END + (SCMOD_SHIFT << 16), SCI_LINEENDWRAPEXTEND);

		DWORD spec_chars[] = 
		{
			66, //  Ctrl+B
			69, //  Ctrl+E
			70, //  Ctrl+F
			71, //  Ctrl+G
			72, //  Ctrl+H
			75, //  Ctrl+K
			78, //  Ctrl+N
			79, //  Ctrl+O
			80, //  Ctrl+P
			81, //  Ctrl+Q
			82, //  Ctrl+R
			83, //  Ctrl+S
			87  //  Ctrl+W
		};
		for (int i = 0; i < sizeof(spec_chars) / sizeof(DWORD); ++i)
		{
			AssignCmdKey(spec_chars[i] + (SCMOD_CTRL << 16), SCI_NULL); 
		}

		UsePopUp(false);

	}

	void SetFontSize(LONG lHeight)
	{
	}

	void GetSel(long& nStartChar, long& nEndChar)
	{
		nStartChar = GetSelectionStart();
		nEndChar = GetSelectionEnd();
	}

	_tstring GetText()
	{
		string s;
		int len = GetLength();

		s.resize(len + 1);
		CScintillaWindowImpl<CScintillaNoteEdit>::GetText(s.size(), &s[0]);
		string trimed(s);
		boost::trim(trimed);
		if (trimed.empty())
		{
			return _tstring();
		}
		
		_tstring ws = ATOT(s.c_str(), s.size());
		return ws;
	}

	void SetText(_tstring const& text)
	{
		string s;
		if (!text.empty())
		{
			s = TTOA(text.c_str(), text.size());
		}
		CScintillaWindowImpl<CScintillaNoteEdit>::SetText(s.c_str());
	}

private:

	void SetAStyle(int style, COLORREF back, COLORREF fore = RGB(0,0,0), int size = 0, const char *face = NULL) {
		StyleSetBack(style, back);
		StyleSetFore(style, fore);
		if (size >= 1)
			StyleSetSize(style, size);
		if (face) 
			StyleSetFont(style, face);
	}

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (wParam == VK_ESCAPE)
		{
			GetParent().PostMessage(WM_CLOSE/*WMU_ESCAPEFOCUS*/);
		}
		SetMsgHandled(FALSE);
		return 0;
	}

	LRESULT OnEditRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		Redo();
		return 0;
	}
	
};
