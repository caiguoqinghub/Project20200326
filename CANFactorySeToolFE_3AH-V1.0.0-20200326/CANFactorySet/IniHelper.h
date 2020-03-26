#pragma once

namespace InitHelper
{

	struct FieldShowIDMap
	{
		enum {
			STRING_TYPE = 0,
			NUMBER_TYPE
		} type;

		union {
			struct {
				LPCTSTR  field;
				UINT     show_id;
				LPCTSTR  defval;
			} string_field_show_id_map;

			struct  {
				LPCTSTR  field;
				UINT     show_id;
				UINT defval;
			} number_field_show_id_map;
		};
	};

#define STRING_FIELD_SHOW_ID_MAP_ITEM(field, show_id, defval) {InitHelper::FieldShowIDMap::STRING_TYPE, {field, show_id, defval}}
#define NUMBER_FIELD_SHOW_ID_MAP_ITEM(field, show_id, defval) {InitHelper::FieldShowIDMap::NUMBER_TYPE, {field, show_id, defval}}

void LoadConfigUpdateUI(FieldShowIDMap *fsm, UINT fsm_cnt, LPCTSTR ini_file, LPCTSTR sector_name, CDialog *owner);

}