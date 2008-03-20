/*!
Wide char exception
*/

#ifndef __WEXCEPT_H__
#define __WEXCEPT_H__

#include <exception>
#include <string>

class wexception
{
public:
	explicit wexception(const std::wstring& _Message = L"Unknown exception")
		: _Str(_Message)
	{	// construct from message string
	}

	virtual ~wexception()
	{}	// destroy the object

	virtual const wchar_t *what() const _THROW0()
	{	// return pointer to message string
		return (_Str.c_str());
	}
private:
	std::wstring _Str;	// the stored message string
};

class wlogic_error : public wexception
{
public:
	explicit wlogic_error(const std::wstring& _Message)
		: wexception(_Message)
	{	// construct from message string
	}
};

#if defined(_UNICODE) || defined(UNICODE) || defined(SQLITEPP_UNICODE)
#define _texception wexception
#define _tlogic_error wlogic_error
#else
#define _texception exception
#define _tlogic_error logic_error
#endif 

#endif __WEXCEPT_H__
