// /**
//  * \file ModulesImpl.h
//  *
//  * \since 2006/09/12
//  * \author pavel
//  */
// 
// #pragma once
// 
// #include "Model.h"
// #include "RowIteratorImpl.h"
// 
// class CModuleRow : public Models::IModule
// {
// public:
// 	// interface
// 	virtual int GetVer() { return m_nVer; }
// 	virtual void SetType(LPCTSTR szType) { m_szType = szType; }
// 	virtual void SetVer(int nVer) { m_nVer = nVer; }
// 	virtual void SetName(LPCTSTR szName) { m_sName = szName; }
// 
// 	// implementation
// 	_tstring GetStorageName();
// 
// 	_tstring m_szType;
// 	_tstring m_sName;
// 	int m_nVer;
// };
// 
// class CModulesImpl
// 	: public CRowIteratorImpl<Models::IModule, CModuleRow, Models::IModules>
// {
// public:
// 	CModulesImpl(sqlitepp::session& sess);
// 
// 	// interface
// 	virtual void select(LPCTSTR szType);
// 	virtual void insert();
// 	virtual void update();
// 
// 	// implementation
// 	_tstring GetStorageName(LPCTSTR szType);
// 
// public:
// 	virtual ~CModulesImpl(void);
// };
