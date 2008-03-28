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
