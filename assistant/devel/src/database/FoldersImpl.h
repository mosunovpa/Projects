// /**
//  * \file FoldersImpl.h
//  *
//  * \since 2006/07/17
//  * \author pavel
//  */
// 
// #ifndef FoldersImpl_h__
// #define FoldersImpl_h__
// 
// #include "Model.h"
// #include "RowIteratorImpl.h"
// 
// //////////////////////////////////////////////////////////////////////////
// //
// 
// class CFolder : public Models::IFolder
// {
// public:
// 	int m_nId;
// 	_tstring m_scName;
// 
// 	virtual int GetId() { return m_nId; }
// 	virtual LPCTSTR GetName() { return m_scName.c_str(); }
// 
// };
// 
// //////////////////////////////////////////////////////////////////////////
// //
// 
// class CFoldersImpl 
// 	:	public CRowIteratorImpl<Models::IFolder, CFolder, Models::IFolders>
// {
// public:
// 	CFoldersImpl(sqlitepp::session& sess);
// 	virtual  ~CFoldersImpl() {}
// 	virtual void select(int nParentId);
// private:
// 	int m_nParentId;
// };
// #endif // FoldersImpl_h__
