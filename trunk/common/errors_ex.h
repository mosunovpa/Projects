
#pragma once

#include "errors.h"

#define CATCH_ALL_ERRORS(hWnd) \
	catch (IExeption* e) { errors::ErrorMessage(e->What(), hWnd); e->Delete(); } \
	catch (wexception e) { errors::ErrorMessage(e.what(), hWnd); } \
	catch (std::runtime_error e) { errors::ErrorMessage(ATOT(e.what(), strlen(e.what())).c_str(), hWnd); } \
	catch (std::exception e) { errors::ErrorMessage(ATOT(e.what(), strlen(e.what())).c_str(), hWnd); } \
	catch (...) { errors::ErrorMessage(_T("Unknown error"), hWnd); } \

#define BEGIN_MSG_MAP_TRY(theClass) BEGIN_MSG_MAP(theClass) try {

#define END_MSG_MAP_CATCH() } CATCH_ALL_ERRORS(m_hWnd) END_MSG_MAP()

namespace errors
{
	// you should implement this function in your application
	int ErrorMessage(LPCTSTR msg, HWND hWnd = NULL);
}

