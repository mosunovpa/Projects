/*!
 *	\file Tasks.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/11/01
 */

#ifndef __TASKS_H__
#define __TASKS_H__

#ifdef TASKS_EXPORTS
#define TASKS_API __declspec(dllexport)
#else
#define TASKS_API __declspec(dllimport)
#endif

#include "ModuleInfo.h"

extern "C" 
{
/**
 * return module information
 */
TASKS_API Module::IModuleInfo* GetModuleInfo(Assist::IApplication* pApplication);

} //extern "C" 

#endif //__TASKS_H__
