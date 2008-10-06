// /**
//  * \file ModulesImpl.cpp
//  *
//  * \since 2006/09/12
//  * \author pavel
//  */
// 
// #include "StdAfx.h"
// #include "ModulesImpl.h"
// #include "strutils.h"
// #include "crc32.h"
// 
// 
// //////////////////////////////////////////////////////////////////////////
// // CModuleRow
// 
// _tstring CModuleRow::GetStorageName()
// {
// 	return strutils::format(_T("adx_%s_%u"), 
// 		m_sName.c_str(),
// 		Crc32::crc32_get_crc((unsigned char*)m_szType.c_str(), m_szType.size() * sizeof(TCHAR)));
// }
// 
// //////////////////////////////////////////////////////////////////////////
// // CModulesImpl
// 
// CModulesImpl::CModulesImpl(sqlitepp::session& sess)
// :	CRowIteratorImpl<Models::IModule, CModuleRow, Models::IModules>(sess)
// {
// }
// 
// CModulesImpl::~CModulesImpl(void)
// {
// }
// 
// void CModulesImpl::select(LPCTSTR szType)
// {
// 	try
// 	{
// 		m_row.m_szType = szType;
// 		m_st << _T("select name, version from adx_modules where type = :szType"),
// 			sqlitepp::use(m_row.m_szType, _T("szType")),
// 			sqlitepp::into(m_row.m_sName, _T("name")),
// 			sqlitepp::into(m_row.m_nVer, _T("version"));
// 	}
// 	RAISE()
// }
// 
// _tstring CModulesImpl::GetStorageName(LPCTSTR szType)
// {
// 	select(szType);
// 	if (!next())
// 		ThrowError(_T("can't find storage"));
// 
// 	return m_row.GetStorageName();
// }
// 
// void CModulesImpl::insert()
// {
// 	try
// 	{
// 		// create items table
// 		_tstring sql = strutils::format(_T("create table %s  (id integer primary key)"),
// 			m_row.GetStorageName().c_str());
// 		m_st << sql;
// 		m_st.exec();
// 
// 		// add info about storage
// 		m_st << _T("insert into adx_modules (type, name, version) values (:type, :name, :version)"),
// 			sqlitepp::use(m_row.m_szType),
// 			sqlitepp::use(m_row.m_sName),
// 			sqlitepp::use(m_row.m_nVer);
// 		m_st.exec();
// 	}
// 	RAISE()
// }
// 
// void CModulesImpl::update()
// {
// 	try
// 	{
// 		m_st << _T("update adx_modules set name = :name, version = :version where type = :type)"),
// 			sqlitepp::use(m_row.m_sName),
// 			sqlitepp::use(m_row.m_nVer),
// 			sqlitepp::use(m_row.m_szType);
// 		m_st.exec();
// 	}
// 	RAISE()
// }
