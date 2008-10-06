
/**
 * @file guiutils.h
 *
 * guiutils interface.
 *
 * @author Pavel Mosunov
 * @since 2004-9-22
 */

#ifndef __GUIUTILS_H__
#define __GUIUTILS_H__

#include <vector>
#include <map>
#include "errors.h"
#include "resource.h"
#include "resutils.h"
#include "atlctrls.h"
#include "rsettings.h"
#include "strutils.h"

#define CATCH_ALL_ERRORS(hWnd) \
	catch (IExeption* e) { GuiUtils::ErrorMessage(e->What(), hWnd); e->Delete(); } \
	catch (wexception e) { GuiUtils::ErrorMessage(e.what(), hWnd); } \
	catch (std::runtime_error e) { GuiUtils::ErrorMessage(ATOT(e.what(), strlen(e.what())).c_str(), hWnd); } \
	catch (std::exception e) { GuiUtils::ErrorMessage(ATOT(e.what(), strlen(e.what())).c_str(), hWnd); } \
	catch (...) { GuiUtils::ErrorMessage(_T("Unknown error"), hWnd); } \

#define BEGIN_MSG_MAP_TRY(theClass) BEGIN_MSG_MAP(theClass) try {

#define END_MSG_MAP_CATCH() } CATCH_ALL_ERRORS(m_hWnd) END_MSG_MAP()

namespace GuiUtils
{

int ErrorMessage(LPCTSTR msg, HWND hWnd = NULL);

BOOL WinPosIsValid(const CRect& WinPos);
BOOL GetIconSize(HICON icon, CSize& size);
CString GetTooltip(UINT nID);
_tstring GetSelectedTreeItemPath(CTreeViewCtrl& tree);
void GetSelectedTreeItemPath(CTreeViewCtrl& tree, OUT std::vector<HTREEITEM>& Ids);
BOOL SelectListViewItem(CListViewCtrl& listCtrl, int nItemPos);
bool IsParent(HWND hParent, HWND hChild);
HWND GetNextTabstopWindow(HWND hDlg, HWND hCtrl);
HWND GetPrevTabstopWindow(HWND hDlg, HWND hCtrl);
bool IsTabstopProcessing(HWND hDlg, MSG* pMsg);

//////////////////////////////////////////////////////////////////////////
// CTreeCtrlEnumerator

class CTreeCtrlEnumerator
{
public:
	static HTREEITEM FindByData(CTreeViewCtrl& tree, int nData);
	void EnumTree(CTreeViewCtrl& tree);
	virtual BOOL OnTreeItem(CTreeViewCtrl& tree, HTREEITEM hItem, int level) { return TRUE; }
	virtual BOOL OnIsTreeItemChildProcessing(CTreeViewCtrl& tree, HTREEITEM hItem, int level) { return TRUE; }
};


//////////////////////////////////////////////////////////////////////////
// CListCtrlOptions

class CListCtrlOptions : public CRegSettings
{
public:
	struct COLUMN_INFO : public CRegSettings
	{
		int Width;
		BEGIN_REG_MAP(COLUMN_INFO)
			REG_ITEM(Width, 100);
		END_REG_MAP()
	};
	typedef std::list<COLUMN_INFO> CColumnList;

	CColumnList Columns;

	BEGIN_HKCU_REG_MAP(CListCtrlOptions, _T(""))
		REG_ITEM_VECTOR(Columns)
	END_REG_MAP()

	void StoreColumns(CListViewCtrl& listCtrl);
	void RestoreColumns(CListViewCtrl& listCtrl);
};

//////////////////////////////////////////////////////////////////////////
// CListCtrlState

class CListCtrlState
{
public: 
	CListCtrlState(CListViewCtrl& listCtrl);
	~CListCtrlState();
protected:
	void StoreState();
	void RestoreState();
private:
	std::map<DWORD, UINT> m_mapIdToState;
	int m_nTop;
	CListViewCtrl& m_listCtrl;
};

} // namespace GuiUtils

#endif // __GUIUTILS_H__
