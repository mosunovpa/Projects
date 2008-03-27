
#pragma once
	
namespace guiutils
{
	void DrawBitmap(CDC& dc, HBITMAP hBitmap, CRect const& rc);
	BOOL WinPosIsValid(const CRect& WinPos);
}
