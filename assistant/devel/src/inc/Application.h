/*!
 *	\file Application.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/11/01
 */
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "statement.h"


// refresh codes
#define UNM_FILE_OPENED				1
#define UNM_DETAILS_SHOWN			2
#define UNM_FOLDER_CHANGED			3
#define UNM_ITEM_ADDED				4
#define UNM_ITEM_DELETED			5
#define UNM_FOLDER_ADDED			6
#define UNM_FOLDER_DELETED			7

namespace Assist
{

#define APP_NAME _T("Assistant")
#define APP_VER 0x0100
#define APP_OPTION_PATH _T("Software\\")APP_NAME
#define APP_DEFAULT_FILE_NAME _T("Assistant.adx")

#define WM_BASE		WM_USER + 100

// messages

#define WM_SETUP			WM_BASE + 1

/**
 
 */
class IDocStorage
{
public:
	virtual void Update(LPCTSTR szScript, int nVer) = 0;
	virtual int GetStorageVer() const = 0;
};

/**
 
 */
class IItemIterator : public IReleasable
{
public:
	virtual BOOL exec() = 0;

	virtual int GetId() = 0;
	virtual LPCTSTR GetName() = 0;
	virtual int GetModuleId() = 0;
	virtual BOOL IsDeleted() = 0;
	virtual BOOL IsFolder() = 0;
	virtual int GetParentId() = 0;
	virtual time_t GetCreatedDate() = 0;
	virtual time_t GetModifiedDate() = 0;
protected:
	virtual ~IItemIterator() {}
};
typedef shared_ptr_2<IItemIterator> IItemIteratorPtr;

/**
 
 */
class IApplication
{
public:
	virtual void GetModuleOptionsPath(
		OUT LPTSTR szBuf,
		IN int mod_id, 
		IN LPCTSTR szSubKey = NULL
		) = 0;
	virtual IStatement* GetStatement() = 0;
	virtual void AddItemWindow(HWND hWnd) = 0;
	virtual void DeleteItemWindow(HWND hWnd) = 0;
	virtual IItemIterator* GetItems(int nFolderId) = 0;
	virtual IItemIterator* GetItem(int nItemId) = 0;
	virtual void DeleteItems(int* pBuf, int nSize, bool bConfirm = true) = 0;
	virtual void StoreItem(int nNoteId, LPCTSTR szName, LPCTSTR szContent) = 0;
	virtual void NewItem(int nModuleId, int nParentId, LPCTSTR szName, LPCTSTR szContent) = 0;

protected:
	virtual ~IApplication() {}
};

} //namespace Assist


//////////////////////////////////////////////////////////////////////////
// export functions

#ifdef ASSIST_EXPORTS
#define ASSIST_API __declspec(dllexport)
#else
#define ASSIST_API __declspec(dllimport)
#endif

#endif //__APPLICATION_H__
