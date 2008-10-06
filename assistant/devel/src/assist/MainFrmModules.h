/**
 * \file MainFrmModules.h
 *
 * \since 2006/10/11
 * \author pavel
 */

#pragma once

struct CModule;

	// data
struct MODULE
{
	CModule* m_pModule;
	DWORD m_nCmd;
	int m_nImage;
	DWORD m_nCurrentFolder;

	MODULE(CModule* pModule = NULL, DWORD nCmd = 0, int nImage = -1)
		:	m_pModule(pModule), m_nCmd(nCmd), m_nImage(nImage), m_nCurrentFolder(0)
	{
	}
};

class CMainFrmModules : public std::list<MODULE>
{
public:
	CMainFrmModules(void);

	MODULE* GetModule(DWORD mod_id);
//	MODULE* GetModuleByCmd(DWORD module_cmd);

 	DWORD GetModuleCmd(DWORD mod_id);
 	DWORD GetModuleId(DWORD module_cmd);
 	BOOL IsModuleCmd(DWORD module_cmd);

	DWORD GetModuleItemCmd(DWORD mod_id);
	DWORD GetModuleItemId(DWORD module_cmd);
	BOOL IsModuleItemCmd(DWORD module_cmd);
	void Reset();
};
