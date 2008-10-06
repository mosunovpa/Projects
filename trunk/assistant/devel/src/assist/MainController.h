/**
 * @file MainController.h
 *
 * CMainController interface.
 *
 * @author Pavel Mosunov
 * @since 2004-9-6
 */

#ifndef __MAINFRMCONTROLLER_H__
#define __MAINFRMCONTROLLER_H__

#include "controller.h"
#include "resource.h"
#include "Application.h"

/**
 * class CMainController.
 */
class CMainController : public Controllers::CController
{
public:
	CMainController();
	virtual ~CMainController(void);

	BEGIN_MSG_MAP(CMainController)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_NEW_DATAFILE, OnNewDataFile)
		COMMAND_RANGE_HANDLER(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnFileRecent)
	END_MSG_MAP()

private:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNewDataFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileRecent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void ShowAbout();
};

#endif // __MAINFRMCONTROLLER_H__
