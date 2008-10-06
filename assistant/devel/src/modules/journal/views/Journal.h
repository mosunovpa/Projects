/*!
 *	\file Journal.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/11/01
 */

#ifndef __JOURNAL_H__
#define __JOURNAL_H__

#ifdef JOURNAL_EXPORTS
#define JOURNAL_API __declspec(dllexport)
#else
#define JOURNAL_API __declspec(dllimport)
#endif

#include "ModuleInfo.h"
#include "Application.h"

extern "C" 
{
/**
 * return module information
 */
JOURNAL_API Module::IModuleInfo* GetModuleInfo(Assist::IApplication* pApplication);

} //extern "C" 

#endif //__JOURNAL_H__
