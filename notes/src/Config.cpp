#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig(void)
{
}

CConfig::~CConfig(void)
{
}

_tstring CConfig::GetLastDataFileName() const
{
	return m_last_datafile_name;
}

void CConfig::SetLastDataFileName (_tstring const& file_name)
{
	m_last_datafile_name = file_name;
}