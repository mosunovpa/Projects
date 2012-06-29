#pragma once

class CNotebaseAutoRegister
{
private:
	HMODULE m_basemodule;
public:
	CNotebaseAutoRegister()
	{
		m_basemodule = LoadLibrary( _T("notebase.dll"));
		ATLASSERT(m_basemodule);
	}

	~CNotebaseAutoRegister()
	{
		FreeLibrary(m_basemodule);
	}
};

#ifdef NOTEBASE_EXPORTS
#define NOTEBASE_API __declspec(dllexport)
#else
#define NOTEBASE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

NOTEBASE_API HWND GetPrevActiveWindow();
NOTEBASE_API void SetHook(HWND hwnd, DWORD cmd);
NOTEBASE_API void RemoveHook();

#ifdef __cplusplus
}
#endif
