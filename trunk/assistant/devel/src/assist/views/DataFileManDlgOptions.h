/**
 * @file DataFileManDlgOptions.h
 *
 * Data file manager dialog options interface.
 *
 * @author Pavel Mosunov
 * @since 2004-9-22
 */

#pragma once

#include "rsettings.h"

class CDataFileManDlgOptions : public CRegSettings
{
public:
	void Init();
	virtual ~CDataFileManDlgOptions(void);
	RECT Pos;
	
	BEGIN_REG_MAP(CDataFileManDlgOptions)
		REG_ITEM_BINARY(Pos)
	END_REG_MAP()	
};
