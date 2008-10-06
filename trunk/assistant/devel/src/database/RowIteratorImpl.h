/**
 * \file RowIteratorImpl.h
 *
 * \since 2006/07/17
 * \author pavel
 */

// #ifndef RowIteratorImpl_h__
// #define RowIteratorImpl_h__
//
// #include "session.hpp"
// #include "statement.hpp"
// #include "rowiterator.h"
// 
// template <class TInt, class TIntImpl, class TBase = CRowIteratorT<TInt> >
// class CRowIteratorImpl : public TBase
// {
// public:
// 	CRowIteratorImpl(sqlitepp::session& sess) : m_sess(sess), m_st(sess) {}
// 	virtual ~CRowIteratorImpl() {}
// 
// 	virtual TInt* next() 
// 	{ 
// 		try
// 		{
// 			if (m_st.exec())
// 				return row();
// 		}
// 		RAISE()
// 		return NULL; 
// 	}
// 	virtual TInt* row() { return &m_row; }
// 	virtual void release() { delete this; }
// 
// protected:
// 	sqlitepp::session& m_sess;
// 	sqlitepp::statement m_st;
// 	TIntImpl m_row;
// };
//
//#endif // RowIteratorImpl_h__