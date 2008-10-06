/*!
 * \file ModuleLoader.h
 * 
 * \author Pavel Mosunov
 * \since 2005-5-10
 */

#ifndef __MODULELOADER_H__
#define __MODULELOADER_H__

#include "AssistView.h"
#include "ModuleInfo.h"
#include "ContainerImpl.h"
#include "Application.h"
#include "simplesinglton.h"

//////////////////////////////////////////////////////////////////////////
// class CModule

struct CModule
{
	CModule();
	~CModule();

	Module::IModuleInfo* pModuleInfo;
	HMODULE hModule;
};

//////////////////////////////////////////////////////////////////////////
// types

typedef IContainer<CModule> IModuleList;

//////////////////////////////////////////////////////////////////////////
// CModuleLoader

class CModuleLoader : public CSimpleSinglton<CModuleLoader>
{
	friend class CSimpleSinglton<CModuleLoader>;

	typedef CContainerImplT< std::list<CModule> > CModuleList;
protected:
	CModuleLoader();
	CModuleLoader(CModuleLoader const& right);
	virtual ~CModuleLoader();
public:
	void LoadModules();
	CModule* FindModule(DWORD mod_id);
	IModuleList* GetModules();

private:
	BOOL LoadModule(const CString& csModuleName, CModule& module);
	CModuleList m_Modules;
};

#endif //__MODULELOADER_H__
