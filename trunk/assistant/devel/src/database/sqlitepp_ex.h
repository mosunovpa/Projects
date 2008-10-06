
#pragma once

#include "sqlitepp.hpp"
#include "strbuf.h"

namespace sqlitepp 
{

	template<>
	struct converter<CStringBuf>
	{
		typedef string_t base_type;
		static CStringBuf to(string_t const& s)
		{
			return CStringBuf(s.c_str());
		}
		static string_t from(CStringBuf const& t)
		{
			return string_t((LPCTSTR)t);
		}
	};


	/// Named use binder.
	template<typename T>
	class use_name_any_binder : public use_pos_binder<T>
	{
	public:
		use_name_any_binder(T& value, string_t const& name)
			: use_pos_binder<T>(value)
			, name_(name)
		{
		}
	private:
		void do_bind(statement& st, int)
		{
			st.use_value(st.use_pos(this->name_), used_ = converter<T>::from(this->value_));
		}

		typename converter<T>::base_type used_;
		string_t name_;
	};

	template<typename T>
	class use_name_any_binder_const : public use_pos_binder_const<T>
	{
	public:
		use_name_any_binder_const(T const& value, string_t const& name)
			: use_pos_binder_const<T>(value)
			, name_(name)
		{
		}
	private:
		void do_bind(statement& st, int)
		{
			st.use_value(st.use_pos(this->name_), used_ = converter<T>::from(this->value_));
		}

		typename converter<T>::base_type used_;
		string_t name_;
	};
}
