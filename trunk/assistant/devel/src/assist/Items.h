/**
 * \file Items.h
 *
 * \since 2006/12/08
 * \author pavel
 */

#pragma once

#include "Statement.h"
#include "time.h"
#include "Application.h"


//////////////////////////////////////////////////////////////////////////
// CItemIterator

class CItems;

class CItemIterator : public Assist::IItemIterator
{
	friend class CItems;

public:
	CItemIterator();

	virtual int GetId() override { return m_nId; }
	virtual LPCTSTR GetName() override { return m_szName; }
	virtual int GetModuleId() override { return m_nModule; }
	virtual BOOL IsDeleted() override { return m_bDeleted; }
	virtual BOOL IsFolder() override { return m_bFolder; }
	virtual int GetParentId() override { return m_nParent; }
	virtual time_t GetCreatedDate() override { return m_tCreated; }
	virtual time_t GetModifiedDate() override { return m_tModified; }

private:
	virtual BOOL exec() { return m_st->exec(); }
	virtual void release() { delete this; }
	virtual ~CItemIterator() {}

	IStatementPtr m_st;

	int m_nId;
	CStringBuf m_szName;
	int m_nModule;
	BOOL m_bDeleted;
	BOOL m_bFolder;
	int m_nParent;
	time_t m_tCreated;
	time_t m_tModified;
};
typedef shared_ptr_2<CItemIterator> CItemIteratorPtr;

//////////////////////////////////////////////////////////////////////////
// CItems

class CItems
{
public:
	static Assist::IItemIterator* SelectFolders(int module, int parent);
	static Assist::IItemIterator* SelectTrash();
	static Assist::IItemIterator* GetItems(int nFolderId);
	static void Rename(LPCTSTR szFolderName, int nId);
	static int New(LPCTSTR szName, LPCTSTR szContent, int nParentId, BOOL bFolder, int nModuleId =-1);
	static Assist::IItemIterator* GetItem(int nId);
	static void Delete(std::vector<int>& vIds, BOOL bRemove = FALSE);
};
