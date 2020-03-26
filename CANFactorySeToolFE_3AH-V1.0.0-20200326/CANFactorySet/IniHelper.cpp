#include "stdafx.h"
#include "IniHelper.h"

void InitHelper::LoadConfigUpdateUI(FieldShowIDMap *fsm, UINT fsm_cnt, LPCTSTR ini_file, LPCTSTR sector_name, CDialog *owner)
{
	if (!PathFileExists(ini_file))
	{
		CString error;
		error.Format(_T("配置文件[%s]不存在"), ini_file);
		AfxMessageBox(error);
		return;
	}

	TCHAR buf[BUFSIZ];
	DWORD number = 0;


	for (UINT i = 0; i < fsm_cnt; i++) {
		switch (fsm[i].type)
		{
		case FieldShowIDMap::STRING_TYPE:
			{
				auto& item = fsm[i].string_field_show_id_map;
				number = GetPrivateProfileString(sector_name, item.field, item.defval, buf, BUFSIZ, ini_file);
				buf[number] = _T('\0');
				owner->SetDlgItemText(item.show_id, buf);
			}
			break;
		case FieldShowIDMap::NUMBER_TYPE:
			{
				auto& item = fsm[i].number_field_show_id_map;
				owner->SetDlgItemInt(item.show_id, GetPrivateProfileInt(sector_name, item.field, item.defval, ini_file));
			}
			break;
		}
	}
}
