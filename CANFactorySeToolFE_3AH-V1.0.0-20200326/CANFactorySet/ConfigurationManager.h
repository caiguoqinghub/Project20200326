#pragma once

namespace ConfigurationManager
{
	enum {
		HIGH_CONFIG = 0,
		MID_CONFIG,
		LOW_CONFIG,
		CONFIG_NUM
	};

	LPCTSTR GetMachineKind(UINT config);
	LPCTSTR GetPartNumber(UINT config);
	LPCTSTR GetBDProjectNumber(UINT config);
}