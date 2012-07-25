#pragma once


#ifdef NOTESHOOK_EXPORTS
#define NOTESHOOK_API __declspec(dllexport)
#else
#define NOTESHOOK_API __declspec(dllimport)
#endif



#ifdef __cplusplus
extern "C" {
#endif

NOTESHOOK_API HWND GetPrevActiveWindow();
NOTESHOOK_API void SetHook();
NOTESHOOK_API void RemoveHook();

#ifdef __cplusplus
}
#endif


class CNotesHookAutoRegister
{
private:
	HMODULE m_basemodule;
public:
	CNotesHookAutoRegister()
	{
		m_basemodule = LoadLibrary( _T("noteshook.dll"));
		ATLASSERT(m_basemodule);
	}

	~CNotesHookAutoRegister()
	{
		FreeLibrary(m_basemodule);
	}
};
