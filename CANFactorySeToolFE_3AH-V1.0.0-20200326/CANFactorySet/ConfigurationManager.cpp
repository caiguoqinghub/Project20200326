#include "StdAfx.h"
#include "ConfigurationManager.h"

LPCTSTR ConfigurationManager::GetMachineKind(UINT config)
{
	ASSERT(config < CONFIG_NUM);
	LPCTSTR machine_kinds[] = {
		_T("FE_3AH"), _T("FE_3AH"), _T("FE_3AH")
	};
	return machine_kinds[config];
}

LPCTSTR ConfigurationManager::GetPartNumber(UINT config)
{
	ASSERT(config < CONFIG_NUM);
	LPCTSTR part_numbers[] = {
		_T("6600016081"), _T("6600070596"), _T("6600070596")
	};
	return part_numbers[config];
}

LPCTSTR ConfigurationManager::GetBDProjectNumber(UINT config)
{
	ASSERT(config < CONFIG_NUM);
	return _T("BD2011");
}

