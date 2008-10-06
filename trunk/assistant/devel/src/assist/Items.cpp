/**
 * \file Items.cpp
 *
 * \since 2006/12/08
 * \author pavel
 */

#include "stdafx.h"
#include "resource.h"
#include "Items.h"
#include "defines.h"
#include "ApplicationImpl.h"
#include "dateutils.h"

//////////////////////////////////////////////////////////////////////////
// helpers

void EnumItems(int nId, std::vector< std::pair<int, int> >& vIds)
{
	Assist::IItemIteratorPtr item = CItems::GetItem(nId);

	IStatementPtr st = CApplicationImpl::Get().GetStatement();
	st->sql(_T("select id from adx_items where parent = :parent"));
	int id;
	st->use((int&)nId, _T(":parent"));
	st->into(id, _T("id"));
	while (st->exec())
	{
		EnumItems(id, vIds);
	}
	
	if (item->GetParentId() > 0)
	{
		std::pair<int, int> pairItem(nId, item->IsFolder());
		vIds.push_back(pairItem);
	}
}

void RemoveItem(int nId)
{
	IStatementPtr st = CApplicationImpl::Get().GetStatement();
	st->sql(_T("delete from adx_items where id = :nId"));
	st->use((int&)nId, _T(":nId"));
	st->exec();
}

void MarkDeleted(int nId)
{
	IStatementPtr st = CApplicationImpl::Get().GetStatement();
	time_t t = dateutils::GetCurrentDate();

	st->sql(_T("update adx_items set deleted = 1, modified = :d where id = :id"));
	st->use((int&)nId, _T(":id"));
	st->use(t, _T(":d"));
	st->exec();
}

//////////////////////////////////////////////////////////////////////////
// CItemIterator

CItemIterator::CItemIterator() 
:	m_st(CApplicationImpl::Get().GetStatement()),
	m_nId(0),
	m_szName(1024),
	m_nModule(0),
	m_bDeleted(0),
	m_bFolder(0),
	m_nParent(0),
	m_tCreated(0),
	m_tModified(0)
{

}

//////////////////////////////////////////////////////////////////////////
// CItems 

Assist::IItemIterator* CItems::SelectFolders(int module, int parent)
{
	CItemIterator* iterator = new CItemIterator;

	iterator->m_nModule = module;
	iterator->m_nParent = parent;

	iterator->m_st->sql(_T("select id, name from adx_items where module = :module and parent = :parent and deleted = 0 and folder = 1"));
	iterator->m_st->into(iterator->m_nId, _T("id"));
	iterator->m_st->into(iterator->m_szName, _T("name"));
	iterator->m_st->use(iterator->m_nModule, _T(":module"));
	iterator->m_st->use(iterator->m_nParent, _T(":parent"));

	return iterator;
}

Assist::IItemIterator* CItems::SelectTrash()
{
	CItemIterator* iterator = new CItemIterator;

	iterator->m_st->sql(_T("select id, modified, name, module from adx_items where deleted = 1"));
	iterator->m_st->into(iterator->m_nId, _T("id"));
	iterator->m_st->into(iterator->m_szName, _T("name"));
	iterator->m_st->into(iterator->m_nModule, _T("module"));
	iterator->m_st->into(iterator->m_tModified, _T("modified"));

	return iterator;
}

Assist::IItemIterator* CItems::GetItems(int nFolderId)
{
	CItemIterator* pIterator = new CItemIterator;
	pIterator->m_nParent = nFolderId;

	pIterator->m_st->sql(_T("select id, modified, name from adx_items where deleted = 0 and folder = 0 and parent = :parent"));
	pIterator->m_st->into(pIterator->m_nId, _T("id"));
	pIterator->m_st->into(pIterator->m_tModified, _T("modified"));
	pIterator->m_st->into(pIterator->m_szName, _T("name"));
	pIterator->m_st->use(pIterator->m_nParent, _T(":parent"));

	return pIterator;
}

void CItems::Rename(LPCTSTR szFolderName, int nId)
{
	CStringBuf sName(szFolderName);
	IStatementPtr st = CApplicationImpl::Get().GetStatement();
	st->sql(_T("update adx_items set name = :name where id = :id"));
	st->use(sName, _T(":name"));
	st->use((int&)nId, _T(":id"));
	st->exec();
}

int CItems::New(LPCTSTR szName, LPCTSTR szContent, int nParentId, BOOL bFolder, int nModuleId/* =-1*/)
{
	if (bFolder == FALSE || nModuleId == -1) // new item is not folder, to get module id from parent folder
	{
		Assist::IItemIteratorPtr pParent = GetItem(nParentId);
		nModuleId = pParent->GetModuleId();
		if (nModuleId == ID_TRASH)
			ThrowError(RESSTR(IDS_CAN_NOT_ADD_TO_TRASH));
	}

	IStatementPtr st = CApplicationImpl::Get().GetStatement();

	CStringBuf sName(szName);
	CStringBuf sContent(szContent);
	time_t t = dateutils::GetCurrentDate();
	st->sql(_T("insert into adx_items (created, name, content, module, parent, folder) values (:created, :name, :content, :module, :parent, :folder)"));
	st->use(t, _T(":created"));
	st->use(sName, _T(":name"));
	st->use(sContent, _T(":content"));
	st->use(nModuleId, _T(":module"));
	st->use(nParentId, _T(":parent"));
	st->use((int&)bFolder, _T(":folder"));
	st->exec();
	return st->last_insert_rowid();
}

Assist::IItemIterator* CItems::GetItem(int nId)
{
	CItemIterator* iterator = new CItemIterator;

	iterator->m_st->sql(_T("select id, name, module, deleted, folder, parent from adx_items where id = :nId"));
	iterator->m_st->into(iterator->m_nId, _T("id"));
	iterator->m_st->into(iterator->m_szName, _T("name"));
	iterator->m_st->into(iterator->m_nModule, _T("module"));
	iterator->m_st->into((int&)iterator->m_bDeleted, _T("deleted"));
	iterator->m_st->into((int&)iterator->m_bFolder, _T("folder"));
	iterator->m_st->into(iterator->m_nParent, _T("parent"));
	iterator->m_st->use(nId, _T(":nId"));
	if (!iterator->m_st->exec())
		ThrowError(strutils::format(_T("No item with %d id"), nId).c_str());
	iterator->m_st->finalize();
	
	return iterator;
}

void CItems::Delete(std::vector<int>& vIds, BOOL bRemove /*= FALSE*/)
{
	std::map<int, int> setIds;
	for (std::vector<int>::iterator it = vIds.begin(); it != vIds.end(); ++it)
	{
		if (setIds.find(*it) == setIds.end())
		{
			std::vector< std::pair<int, int> > v;
			EnumItems(*it, v);
			for (std::vector< std::pair<int, int> >::iterator vi = v.begin(); vi != v.end(); ++vi)
			{
				// first - id, second - is folder
				setIds[vi->first] = vi->second;
			}
		}
	}

	for (std::map<int, int>::iterator it = setIds.begin(); it != setIds.end(); ++it)
	{
		if (bRemove)
		{
			RemoveItem(it->first);
		}
		else
		{
			MarkDeleted(it->first);
		}
		if (it->second) // folder
		{
			CApplicationImpl::Get().UpdateViews(UNM_FOLDER_DELETED, it->first);
		}
		else
		{
			CApplicationImpl::Get().UpdateViews(UNM_ITEM_DELETED, it->first);
		}
	}
}
