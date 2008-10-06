// /**
//  * \file FoldersImpl.cpp
//  *
//  * \since 2006/07/17
//  * \author pavel
//  */
// 
// #include "StdAfx.h"
// #include "FoldersImpl.h"
// 
// CFoldersImpl::CFoldersImpl(sqlitepp::session& sess)
// :	CRowIteratorImpl<Models::IFolder, CFolder, Models::IFolders>(sess), m_nParentId(0)
// {
// }
// 
// void CFoldersImpl::select(int nParentId)
// {
// 	try
// 	{
// 		m_nParentId = nParentId;
// 		m_st << _T("select id, name from adx_folders where parent = :parent_id")
// 			,sqlitepp::into(m_row.m_nId, _T("id"))
// 			,sqlitepp::into(m_row.m_scName, _T("name"))
// 			,sqlitepp::use(m_nParentId)
// 			;
// 	}
// 	RAISE()
// }
