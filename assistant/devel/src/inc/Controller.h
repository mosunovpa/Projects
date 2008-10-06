/*!
 * \file Controller.h
 * 
 * \author Pavel Mosunov
 * \since 2005-4-19
 */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "safeptr.h"

namespace Views
{
	class IAssistView;
}

namespace Controllers
{

class CController
{
public:
	CController(void);
	virtual ~CController(void);
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
			LRESULT& lResult, DWORD dwMsgMapID = 0) = 0;
//	void SetView(Views::IAssistView* pView);
	
public:
//	safe_ptr<Views::IAssistView> m_pView;
};

} // namespace Controllers

#endif //__CONTROLLER_H__
