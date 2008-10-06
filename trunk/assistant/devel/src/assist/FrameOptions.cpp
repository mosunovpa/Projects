/**
 * @file FrameOptions.cpp
 *
 * Frame options implementation.
 *
 * @author Pavel Mosunov
 * @since 2004-9-22
 */

#include "StdAfx.h"
#include "ApplicationImpl.h"
#include "FrameOptions.h"

/**
 * CFrameOptions::Init()
 */
void CFrameOptions::OnBeforeInit()
{
	FramePos = CWindow::rcDefault;
}

/**
 * 
 */
BOOL CFrameOptions::RestoreBand(CReBarCtrl& rebar, DWORD nBandId)
{
	int nIndex = rebar.IdToIndex(nBandId);
	if (nIndex >= 0)
	{
		for (CBandsList::iterator it = Bands.begin();
			it != Bands.end(); ++it)
		{
			if (it->nBandId == nBandId)
			{
				REBARBANDINFO rbBand = { 0 };
				rbBand.cbSize = sizeof(REBARBANDINFO);
				rbBand.fMask = RBBIM_SIZE | RBBIM_STYLE;
				rbBand.cx = it->cxWidth;
				rbBand.fStyle = it->nStyle;
				rebar.SetBandInfo(nIndex, &rbBand);
				if (it->nIndex != nIndex && it->nIndex < (int)rebar.GetBandCount())
					rebar.MoveBand(nIndex, it->nIndex);
				return TRUE;
			}
		}
	}
	return FALSE;
}

/**
 * 
 */
BOOL CFrameOptions::StoreBand(CReBarCtrl& rebar, DWORD nBandId)
{
	int nIndex = rebar.IdToIndex(nBandId);
	if (nIndex >= 0)
	{
		REBARBANDINFO rbBand = { 0 };
		rbBand.cbSize = sizeof(REBARBANDINFO);
		rbBand.fMask = RBBIM_SIZE | RBBIM_STYLE | RBBIM_ID;
		if (rebar.GetBandInfo(nIndex, &rbBand))
		{
			BAND_INFO BandInfo;
			BandInfo.nBandId = rbBand.wID;
			BandInfo.cxWidth = rbBand.cx;
			BandInfo.nStyle = rbBand.fStyle;
			BandInfo.nIndex = nIndex;
			Bands.push_back(BandInfo);
			return TRUE;
		}
	}
	return FALSE;
}

CString CFrameOptions::GetFrameOptionsPath()
{
	return CApplicationImpl::Get().GetOptionsPath(_T("Frame"));
}
