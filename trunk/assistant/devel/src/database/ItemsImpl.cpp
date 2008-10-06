// #include "StdAfx.h"
// #include "ItemsImpl.h"
// #include "strutils.h"
// #include "ModulesImpl.h"
// 
// CItemsImpl::CItemsImpl(LPCTSTR module_type, sqlitepp::session& sess) 
// :	CRowIteratorImpl<Models::IItem, CItem, Models::IItems>(sess),
// 	m_module_type(module_type)
// {
// }
// 
// CItemsImpl::~CItemsImpl(void)
// {
// }
// 
// void CItemsImpl::AddCol(LPCTSTR column)
// {
// 	try
// 	{
// 		CModulesImpl modules(m_sess);
// 		_tstring sql = strutils::format(_T("alter table %s add %s"), 
// 			modules.GetStorageName(m_module_type).c_str(),
// 			column);
// 		m_sess << sql;
// //		m_st << _T("alter table ")
// 	}
// 	RAISE()
// }
