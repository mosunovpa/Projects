// 
// #pragma once
// 
// #include "Model.h"
// #include "RowIteratorImpl.h"
// 
// class CItem : public Models::IItem
// {
// 
// };
// 
// class CItemsImpl 
// 	: public CRowIteratorImpl<Models::IItem, CItem, Models::IItems>
// {
// public:
// 	CItemsImpl(LPCTSTR module_type, sqlitepp::session& sess);
// 	virtual ~CItemsImpl(void);
// 
// 	virtual void AddCol(LPCTSTR column);
// private:
// 	LPCTSTR m_module_type;
// };
