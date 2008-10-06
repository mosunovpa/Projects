/*!
 *	\file ApplicationImpl.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/11/01
 */

#ifndef __APPLICATIONIMPL_H__
#define __APPLICATIONIMPL_H__
 
#include "Application.h" 
#include "rsettings.h"
#include "safeptr.h"
#include "statement.h"
#include "assistView.h"
#include "simplesinglton.h"
#include "Items.h"

class CHiddenWnd;
class CAssistFrame;

namespace Database
{
	class IDataFile;
	class IDatabase;
}

//////////////////////////////////////////////////////////////////////////
//
class CApplicationOptions : public CRegSettings
{
public:
	BEGIN_HKCU_REG_MAP(CApplicationOptions, _T(""))
	END_REG_MAP()
};

//////////////////////////////////////////////////////////////////////////
//
class CApplicationImpl 
	:	public CSimpleSinglton<CApplicationImpl>, public Assist::IApplication
{
	friend class CSimpleSinglton<CApplicationImpl>;
	friend class CItems;

protected:
	CApplicationImpl(void);
	CApplicationImpl(CApplicationImpl const& right);
	virtual ~CApplicationImpl(void);

public:

	// Assist::IApplication
	
	virtual void GetModuleOptionsPath(
		OUT LPTSTR szBuf,
		IN int mod_id, 
		IN LPCTSTR szSubKey = NULL
		) override;

	virtual IStatement* GetStatement() override;
	virtual void AddItemWindow(HWND hWnd) override;
	virtual void DeleteItemWindow(HWND hWnd) override;
	virtual Assist::IItemIterator* GetItems(int nFolderId) override;
	virtual Assist::IItemIterator* GetItem(int nItemId) override;

	virtual void DeleteItems(int* pBuf, int nSize, bool bConfirm = true) override;
	virtual void StoreItem(int nNoteId, LPCTSTR szName, LPCTSTR szContent) override;
	virtual void NewItem(int nModuleId, int nParentId, LPCTSTR szName, LPCTSTR szContent) override;

	// implementation

	BOOL CreateAppWindow();
	void PostCloseAppWindow();
	void CloseAllItemWindows();
	
	CString GetOptionsPath(LPCTSTR szSubKey);
	void OpenLastDataFile();
	void OpenDataFile(const CString& csFileName);
	void OpenDataFile(int wID); // open recent datafile
	BOOL IsDataFileOpened();
	BOOL IsDataFileOpened(const CString& csFileName);

	int AddFolder(LPCTSTR szFolderName, int nParentId);
	void RenameFolder(LPCTSTR szFolderName, int nId);
	void RemoveItems(int* pBuf, int nSize, bool bConfirm = true, bool bRemove = true);
	CString GetItemPath(int nId);

	void SetMainView(CAssistFrame* pView);
	CAssistFrame* GetMainView();
	HWND GetMainViewHwnd();

	CRecentDocumentList m_mru;

private:
	Database::IDatabase* GetDatabase();

	void CApplicationImpl::UpdateViews(int code, WPARAM wparam = 0, LPARAM lparam = 0);

	void CreateDefaultContent(Database::IDataFile* pDataFile);
	void CheckContent(Database::IDataFile* pDataFile);
	void CheckModules(Database::IDataFile* pDataFile);
	
	safe_ptr<CAssistFrame> m_spMainView;
	safe_ptr<Database::IDataFile> m_spDataFile;
	safe_ptr<Database::IDatabase> m_spDatabase;
	CApplicationOptions m_Options;
	std::auto_ptr<CHiddenWnd> spHiddenWnd;
	
	typedef std::list<std::pair<HWND, POINT> > CItemWindows;
	CItemWindows m_listItemWindows;
};

//CApplicationImpl* GetApp();

#endif // __APPLICATIONIMPL_H__