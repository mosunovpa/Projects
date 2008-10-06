/**
 * @file TaskListPane.cpp
 *
 * Task list view window implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-09-02
 */

#include "stdafx.h"
#include "resource.h"
#include "TaskListPane.h"
#include "resutils.h"
#include "Application.h"
#include "TaskModuleImpl.h"

//////////////////////////////////////////////////////////////////////////
// local functions

CString GetTaskListOptionsPath(DWORD mod_id)
{
	TCHAR szPath[MAX_PATH];
	CTaskModuleImpl::Get().GetMainApp()->GetModuleOptionsPath(szPath, mod_id);
	return CString(szPath) + CString(_T("\\TaskList"));
}

//////////////////////////////////////////////////////////////////////////
//

namespace Views
{

CTaskListPane::CTaskListPane() 
:	CListCtrlWindow(GetTaskListOptionsPath(CTaskModuleImpl::Get().GetId()), &CTaskModuleImpl::Get())
{

}

/**
 * CTaskListPane::PreTranslateMessage
 */
BOOL CTaskListPane::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

/**
 * 
 */
void CTaskListPane::AddColumns()
{
	m_listCtrl.AddColumn(RESSTR(IDS_NAME), 0);
}

void CTaskListPane::AddBitmaps()
{

}

} // namespace Views
