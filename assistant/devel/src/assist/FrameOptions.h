/**
 * @file FrameOptions.h
 *
 * Frame options interface.
 *
 * @author Pavel Mosunov
 * @since 2004-9-22
 */

#ifndef __FRAMEOPTIONS_H__
#define __FRAMEOPTIONS_H__

#include "rsettings.h"

class CFrameOptions : public CRegSettings
{
public:
	// structure for storing band info 
	struct BAND_INFO : public CRegSettings
	{
		DWORD nBandId;
		DWORD nStyle;
		int cxWidth;
		int nIndex;

		BEGIN_REG_MAP(BAND_INFO)
			REG_ITEM(nBandId, 0)
			REG_ITEM(nStyle, RBBS_BREAK)
			REG_ITEM(cxWidth, 100)
			REG_ITEM(nIndex, 0)
		END_REG_MAP()	
	};

	typedef std::list<BAND_INFO> CBandsList;

	RECT FramePos;
	DWORD Styles;
	DWORD TreeWidth;
	BOOL FolderState;
	BOOL ToolBar;
	BOOL StatusBar;
	BOOL ViewSelector;
	BOOL ViewAddressBar;
	DWORD ActiveModule;
	CBandsList Bands;
	
	BEGIN_HKCU_REG_MAP(CFrameOptions, GetFrameOptionsPath())
		REG_ITEM(Styles, 0)
		REG_ITEM(TreeWidth, 140)
		REG_ITEM(FolderState, FALSE)
		REG_ITEM(ToolBar,TRUE)
		REG_ITEM(StatusBar,TRUE)
		REG_ITEM(ViewSelector,TRUE)
		REG_ITEM(ViewAddressBar,TRUE)
		REG_ITEM(ActiveModule, 0)
		REG_ITEM_BINARY(FramePos)
		REG_ITEM_VECTOR(Bands)
	END_REG_MAP()	

	virtual void OnBeforeInit();
	BOOL RestoreBand(CReBarCtrl& rebar, DWORD nBandId);
	BOOL StoreBand(CReBarCtrl& rebar, DWORD nBandId);
private:
	CString GetFrameOptionsPath();
};

#endif // __FRAMEOPTIONS_H__

