/*!
 *	\file Notes.h
 *
 *	\author Pavel Mosunov 
 *  \since 2005/11/01
 */

#ifndef __NOTES_H__
#define __NOTES_H__

#ifdef NOTES_EXPORTS
#define NOTES_API __declspec(dllexport)
#else
#define NOTES_API __declspec(dllimport)
#endif

#include "ModuleInfo.h"

extern "C" 
{
/**
 * return module information
 */
NOTES_API Module::IModuleInfo* GetModuleInfo(Assist::IApplication* pApplication);

} //extern "C" 

#endif //__NOTES_H__
