#include "stdafx.h"
#include "guiutils.h"
	
void guiutils::DrawBitmap(CDC& dc, HBITMAP hBitmap, CRect const& rc)
{
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(dc);
	HBITMAP bmpOld = dcBmp.SelectBitmap(hBitmap);
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), dcBmp, 0, 0, SRCCOPY);
	dcBmp.SelectBitmap(bmpOld);
}

BOOL guiutils::WinPosIsValid(const CRect& WinPos)
{
	CRect rc;
	::GetWindowRect(GetDesktopWindow(), &rc);
	return rc.PtInRect(WinPos.TopLeft()) && rc.PtInRect(WinPos.BottomRight());
}