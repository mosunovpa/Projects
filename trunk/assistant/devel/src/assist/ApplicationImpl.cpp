/*!
 *	\file ApplicationImpl.cpp
 *
 *	\author Pavel Mosunov 
 *  \since 2005/11/01
 */

#include "StdAfx.h"
#include "resource.h"
#include "Database.h"
#include "ApplicationImpl.h"
#include "fileutils.h"
#include "GuiUtils.h"
#include "DocStorageImpl.h"
#include "ModuleLoader.h"
#include "ssfiledialog.h"
#include "Defines.h"
#include "dateutils.h"
#include "HiddenWnd.h"
#include "AssistFrame.h"

//CApplicationImpl* GetApp()
//{
//	return CApplicationImpl::Get();
//}

//////////////////////////////////////////////////////////////////////////
// Helpers

CString GetDefaultDataFileName()
{
	TCHAR szDefaultFile[MAX_PATH];
	::SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, szDefaultFile);
	::PathCombine(szDefaultFile, szDefaultFile, APP_NAME);
	if (!::PathFileExists(szDefaultFile))
	{
		fileutils::CreateDirectoryRecursive(szDefaultFile);
	}
	::PathCombine(szDefaultFile, szDefaultFile, APP_DEFAULT_FILE_NAME);
	return szDefaultFile;
}

//////////////////////////////////////////////////////////////////////////
// CApplicationImpl

/**
 * 
 */
CApplicationImpl::CApplicationImpl(void)
:	m_spDatabase(::GetDatabase()),
	m_spDataFile(NULL),
	m_Options(GetOptionsPath(_T("Application"))),
	m_spMainView(NULL)
{
	m_mru.ReadFromRegistry(GetOptionsPath(_T("Application")));
}

CApplicationImpl::CApplicationImpl( CApplicationImpl const& right )
{
	assert(false);
}

/**
* 
*/
CApplicationImpl::~CApplicationImpl(void)
{
	CloseAllItemWindows();
}

void CApplicationImpl::CloseAllItemWindows()
{
	CItemWindows listTemp = m_listItemWindows;
	for (CItemWindows::iterator it = listTemp.begin(); it != listTemp.end(); ++it)
	{
		DestroyWindow(it->first);
	}
}

BOOL CApplicationImpl::CreateAppWindow()
{
	if (!spHiddenWnd.get())
	{
		spHiddenWnd = std::auto_ptr<CHiddenWnd>(new CHiddenWnd);
		if (!spHiddenWnd->Create(HWND_MESSAGE, CWindow::rcDefault, APP_NAME))
			ThrowError(_T("can not create app window"));

		return TRUE;
	}
	return FALSE;
}

void CApplicationImpl::PostCloseAppWindow()
{
	ATLASSERT(spHiddenWnd.get());
	spHiddenWnd->PostMessage(WM_CLOSE);
}

/**
 * 
 */
void CApplicationImpl::SetMainView(CAssistFrame* pView)
{
	m_spMainView = pView;
}

/**
 * 
 */
CAssistFrame* CApplicationImpl::GetMainView()
{
	return m_spMainView;
}

/**
 * 
 */
HWND CApplicationImpl::GetMainViewHwnd()
{
	if (!m_spMainView.is_null() && ::IsWindow(m_spMainView->GetHwnd()))
		return m_spMainView->GetHwnd();

	return NULL;
}

/**
* 
*/
void CApplicationImpl::UpdateViews(int code, WPARAM wparam /*= 0*/, LPARAM lparam /*= 0*/)
{
	if (m_spMainView.is_null())
	{
		return;
	}
	try
	{
		m_spMainView->Refresh(code, wparam, lparam);
	}
	CATCH_ALL_ERRORS(GetMainViewHwnd())
}

/**
* 
*/
Database::IDatabase* CApplicationImpl::GetDatabase()
{
	return m_spDatabase;
}

/**
* 
*/
CString CApplicationImpl::GetOptionsPath(LPCTSTR szSubKey)
{
	CString csResult;
	::PathCombine(csResult.GetBufferSetLength(MAX_PATH), APP_OPTION_PATH, szSubKey);
	csResult.ReleaseBuffer();
	return csResult;
}
/**
* 
*/
void CApplicationImpl::GetModuleOptionsPath(
	OUT LPTSTR szBuf,
	IN int mod_id, 
	IN LPCTSTR szSubKey
	)
{
	::PathCombine(szBuf, APP_OPTION_PATH, _T("Modules"));
	::PathCombine(szBuf, szBuf, strutils::to_string(mod_id).c_str());
	if (szSubKey != NULL)
	{
		::PathCombine(szBuf, szBuf, szSubKey);
	}
}

/**
 * 
 */
IStatement* CApplicationImpl::GetStatement()
{
	return m_spDataFile->GetStatement();
}

/**
 
 */
Assist::IItemIterator* CApplicationImpl::GetItems(int nFolderId)
{
	return CItems::GetItems(nFolderId);
}

Assist::IItemIterator* CApplicationImpl::GetItem(int nItemId)
{
	return CItems::GetItem(nItemId);
}

/**
 
 */
void CApplicationImpl::AddItemWindow(HWND hWnd)
{
	// Calculate position of the new item window.
	CPoint pointTopLeft(0, 0);

	if (m_listItemWindows.empty())
	{
		 CWindow(hWnd).CenterWindow();
		 pointTopLeft = CWindowRect(hWnd).TopLeft();
	}
	else
	{
		CWindowRect rcLast(m_listItemWindows.back().first);
		pointTopLeft.x = max(pointTopLeft.x, rcLast.left);
		pointTopLeft.y = max(pointTopLeft.y, rcLast.top);

		pointTopLeft.Offset(GetSystemMetrics(SM_CYCAPTION), GetSystemMetrics(SM_CYCAPTION));
		CWindowRect rc(hWnd);
		rc.OffsetRect(pointTopLeft);
		CWindowRect rcDesktop(GetDesktopWindow());
		if (!rcDesktop.PtInRect(rc.BottomRight()))
		{
			pointTopLeft.SetPoint(0, 0);
		}
		SetWindowPos(hWnd, NULL, pointTopLeft.x, pointTopLeft.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	// add to the list
	m_listItemWindows.push_back(std::make_pair(hWnd, pointTopLeft));
}

/**
 
 */
void CApplicationImpl::DeleteItemWindow(HWND hWnd)
{
	for (CItemWindows::iterator it = m_listItemWindows.begin(); it != m_listItemWindows.end(); ++it)
	{
		if (it->first == hWnd)
		{
			m_listItemWindows.erase(it);
			return;
		}
	}
}

/**
* 
*/
BOOL CApplicationImpl::IsDataFileOpened(const CString& csFileName)
{
	return (!m_spDataFile.is_null() && 
			!csFileName.IsEmpty() && 
			m_spDataFile->GetFileName() == csFileName);
}

/**
* 
*/
BOOL CApplicationImpl::IsDataFileOpened()
{
	if (m_spDataFile.is_null())
		return FALSE;

	CString csFileName = m_spDataFile->GetFileName();
	if (!csFileName.IsEmpty())
	{
		UpdateViews(UNM_FILE_OPENED, (WPARAM)m_spDataFile->GetFileName());
		return TRUE;
	}
	return FALSE;
}

/**
* 
*/
void CApplicationImpl::OpenLastDataFile()
{
	// open last datafile

	CString csLastFile;
	CApplicationImpl::Get().m_mru.GetFromList(ID_FILE_MRU_FIRST, csLastFile);
	
	CString csDefFileName = GetDefaultDataFileName();

	if (csLastFile.IsEmpty())
	{
		csLastFile = csDefFileName;
	}
	else if (csLastFile != csDefFileName && !::PathFileExists(csLastFile))
	{
		GuiUtils::ErrorMessage(resutils::resstring_fmt(IDS_FILE_NOT_FOUND, csLastFile).c_str());
		csLastFile = csDefFileName;
	}
	try
	{
		OpenDataFile(csLastFile);
		return; // no error
	}
	CATCH_ALL_ERRORS(GetMainViewHwnd())

	// error opening default datafile - show open dialog

	CSSFileDialog dlg(TRUE, _T("*.adx"), GetDefaultDataFileName(), OFN_HIDEREADONLY, CResString<64>(IDS_DATA_FILE_FILTER), NULL);
	if (dlg.DoModal() == IDOK)
	{
		OpenDataFile(dlg.m_szFileName);
	}
	else
	{
		ThrowError(_T("No file selected"));
	}
}

/**
 * Open recent datafile
 */
void CApplicationImpl::OpenDataFile(int wID)
{
	TCHAR szFile[MAX_PATH];
	if (m_mru.GetFromList(wID, szFile, MAX_PATH))
	{
		if (::PathFileExists(szFile))
		{
			OpenDataFile(szFile);
		}
		else
		{
			m_mru.RemoveFromList(wID);
			m_mru.WriteToRegistry(GetOptionsPath(_T("Application")));
			ThrowError(resutils::resstring_fmt(IDS_FILE_NOT_FOUND, szFile).c_str());
		}
	}
}

/**
* 
*/
void CApplicationImpl::OpenDataFile(const CString& csFileName)
{ 
	if (!IsDataFileOpened(csFileName))
	{
		Database::IDataFile* pTempDataFile = GetDatabase()->OpenDataFile(csFileName);
		try
		{
			if (pTempDataFile->IsEmpty())
			{
				CreateDefaultContent(pTempDataFile);
			}
			else
			{
				CheckContent(pTempDataFile);
			}

			CheckModules(pTempDataFile);

			if (!m_spDataFile.is_null())
			{
				GetDatabase()->CloseDataFile(m_spDataFile.get());
			}
			m_spDataFile = pTempDataFile;

			m_mru.AddToList(m_spDataFile->GetFileName());
			m_mru.WriteToRegistry(GetOptionsPath(_T("Application")));

			UpdateViews(UNM_FILE_OPENED, (WPARAM)m_spDataFile->GetFileName());
		}
		catch(...)
		{
			// close new opened temp data file and throw exception up
			GetDatabase()->CloseDataFile(pTempDataFile);
			throw;
		}
	}
}

/**
* 
*/
void CApplicationImpl::CreateDefaultContent(Database::IDataFile* pDataFile)
{
	_tstring script = resutils::reshtml(IDR_CREATE_SQL);
	IStatementPtr st = pDataFile->GetStatement();
	st->sql(script.c_str());
	st->exec_script();

	int ver = DB_VER;
	st->sql(_T("insert into adx_params(name, value) values ('db_version', :val)"));
	st->use(ver, _T(":val"));
	st->exec();
}

/**
* 
*/
void CApplicationImpl::CheckContent(Database::IDataFile* pDataFile)
{
	int ver = 0;
	IStatementPtr st = pDataFile->GetStatement();

	st->sql(_T("select value from adx_params where name = 'db_version'"));
	st->into(ver, _T("value"));
	st->exec();

	if (ver != DB_VER)
	{
		ThrowError(resutils::resstring_fmt(IDS_INVALID_DB_VERSION, ver).c_str());
	}
}

/**
* 
*/
void CApplicationImpl::CheckModules(Database::IDataFile* pDataFile)
{
	IModuleList* pModules = CModuleLoader::Get().GetModules();
	if (pModules->first())
	{
		do 
		{
			Module::IModuleInfo* pModuleInfo = pModules->current().pModuleInfo;
			CDocStorageImpl StorageDef(pModuleInfo, pDataFile);
			try
			{
				pModuleInfo->CheckStorage(&StorageDef);
			}
			CATCH_ALL_ERRORS(GetMainViewHwnd())

		} while (pModules->next());
	}
}

/**
 * 
 */
int CApplicationImpl::AddFolder(LPCTSTR szFolderName, int nParentId)
{
	int nId = CItems::New(szFolderName, _T(""), nParentId, TRUE);
	UpdateViews(UNM_FOLDER_ADDED, nId, nParentId);
	return nId;
}

/**
 * 
 */
void CApplicationImpl::RenameFolder(LPCTSTR szFolderName, int nId)
{
	CItems::Rename(szFolderName, nId);
}

/**/
void CApplicationImpl::DeleteItems(int* pBuf, int nSize, bool bConfirm /*= true*/)
{
	RemoveItems(pBuf, nSize, bConfirm, FALSE);
}

/**/
void CApplicationImpl::RemoveItems(int* pBuf, int nSize, bool bConfirm /*= true*/, bool bRemove /*= true*/)
{
	if (bConfirm &&
		IDCANCEL == AtlMessageBox(GetMainViewHwnd(), IDS_DELETION_CONFIRM, IDR_MAINFRAME, MB_OKCANCEL | MB_ICONQUESTION))
	{
		return;
	}
	CWaitCursor wc;
	CItems::Delete(std::vector<int>(pBuf, pBuf + nSize), bRemove);
}

/**
 * 
 */
CString CApplicationImpl::GetItemPath(int nId)
{
	CString sPath;
	Assist::IItemIteratorPtr pItem  = CItems::GetItem(nId);
	while (pItem->GetParentId() != 0)
	{
		pItem  = CItems::GetItem(pItem->GetParentId());
		if (sPath.IsEmpty())
			sPath = pItem->GetName();
		else
			sPath = CString(pItem->GetName()) + _T("\\") + sPath;
	}
	return sPath;
}

/**
 
 */
void CApplicationImpl::StoreItem(int nNoteId, LPCTSTR szName, LPCTSTR szContent)
{

}

/**
 
 */
void CApplicationImpl::NewItem(int nModuleId, int nParentId, LPCTSTR szName, LPCTSTR szContent)
{
	if (szContent == NULL || szContent[0] == 0)
	{
		ThrowError(_T("Empty content"));
	}
	CModule* pModule = CModuleLoader::Get().FindModule(nModuleId);
	assert(pModule != NULL);
	if (pModule == NULL)
	{
		ThrowError(strutils::format(_T("Module with %d id not found"), nModuleId).c_str());
	}
	if (nParentId == 0)
	{
		int nCurModuleId = m_spMainView->GetCurModule()->GetId();
		if (nCurModuleId == nModuleId)
		{
			nParentId = m_spMainView->GetCurFolder();
		}
		else
		{
			nParentId = m_spMainView->GetRootFolder(nModuleId);
		}
	}
	int nNewItemId = CItems::New(szName, szContent, nParentId, FALSE, nModuleId);
	pModule->pModuleInfo->OnNewItem(nNewItemId, nParentId);
		
	UpdateViews(UNM_ITEM_ADDED, nNewItemId, nParentId);
}

