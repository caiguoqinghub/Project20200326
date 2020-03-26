// VersionInformationConfigurationDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "VersionInformationConfigurationDialog.h"
#include "afxdialogex.h"
#include "IniHelper.h"


// CVersionInformationConfigurationDialog �Ի���

IMPLEMENT_DYNAMIC(CVersionInformationConfigurationDialog, CMyDialogEx)

CVersionInformationConfigurationDialog::CVersionInformationConfigurationDialog(CWnd* pParent /*=NULL*/)
	: CMyDialogEx(CVersionInformationConfigurationDialog::IDD, pParent)
	, cfg_file_(_T(""))
	, main_board_software_version_(_T(""))
	, front_board_system_version_(_T(""))
	, front_board_software_version_(_T(""))
	, nav_software_version_(_T(""))
	, main_board_hardware_version_(_T(""))
	, supplier_software_version_(_T(""))
	, map_resource_size_max_(_T(""))
	, map_resource_size_min_(_T(""))
	, supplier_hardware_version_(_T(""))
	, nsatellite_number_(_T(""))
{

}

CVersionInformationConfigurationDialog::~CVersionInformationConfigurationDialog()
{
}
//20190414
void CVersionInformationConfigurationDialog::DoDataExchange(CDataExchange* pDX)
{
	CMyDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_MAIN_BOARD_SOFTWARE_VERSION, main_board_software_version_);

#if FRONT_BOARD_SYSTEM_VERSION_ENABLED
	DDX_Text(pDX, IDC_STATIC_FRONT_BOARD_SYSTEM_VERSION, front_board_system_version_);
#endif

#if FRONT_BOARD_SOFTWARE_VERSION_ENABLED
	DDX_Text(pDX, IDC_STATIC_FRONT_BOARD_SOFTWARE_VERSION, front_board_software_version_);
#endif

#if NAVIGATE_SOFTWARE_VERSION_ENABLED
	DDX_Text(pDX, IDC_STATIC_NAV_SOFTWARE_VERSION, nav_software_version_);
#endif
	DDX_Text(pDX, IDC_STATIC_SUPPLIER_SOFTWARE_VERSION, supplier_software_version_);
	DDX_Text(pDX, IDC_STATIC_MAIN_BOARD_HARDWARE_VERSION, main_board_hardware_version_);
	DDX_Text(pDX, IDC_STATIC_SUPPLIER_HARDWARE_VERSION, supplier_hardware_version_);

#if MAP_RESOURCE_CHECK_ENABLED
	DDX_Text(pDX, IDC_STATIC_MAP_RESOURCE_SIZE, map_resource_size_min_);
	DDX_Text(pDX, IDC_STATIC_MAP_RESOURCE_SIZE_MAX, map_resource_size_max_);
#endif

#if NSATELLITE_CHECK_ENABLED
	DDX_Text(pDX, IDC_STATIC_NSATELLITE_NUMBER, nsatellite_number_);
#endif

}


UINT CVersionInformationConfigurationDialog::DoModal(LPCTSTR cfg_file)
{
	cfg_file_ = cfg_file;
	return __super::DoModal();
}

BEGIN_MESSAGE_MAP(CVersionInformationConfigurationDialog, CMyDialogEx)
END_MESSAGE_MAP()


// CVersionInformationConfigurationDialog ��Ϣ�������


BOOL CVersionInformationConfigurationDialog::OnInitDialog()
{
	CMyDialogEx::OnInitDialog();

	LoadConfig(cfg_file_);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CVersionInformationConfigurationDialog::LoadConfig(LPCTSTR cfg_file)
{
	InitHelper::FieldShowIDMap items[] = {
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("��������汾��"), IDC_STATIC_MAIN_BOARD_SOFTWARE_VERSION, _T("")),
#if FRONT_BOARD_SYSTEM_VERSION_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ǰ��ϵͳ�汾��"), IDC_STATIC_FRONT_BOARD_SYSTEM_VERSION, _T("")),
#endif

#if FRONT_BOARD_SOFTWARE_VERSION_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ǰ������汾��"), IDC_STATIC_FRONT_BOARD_SOFTWARE_VERSION, _T("")),
#endif

#if NAVIGATE_SOFTWARE_VERSION_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("��������汾��"), IDC_STATIC_NAV_SOFTWARE_VERSION, _T("")),
#endif
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ECU����汾��"), IDC_STATIC_SUPPLIER_SOFTWARE_VERSION, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("����Ӳ���汾��"), IDC_STATIC_MAIN_BOARD_HARDWARE_VERSION, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ECUӲ���汾��"), IDC_STATIC_SUPPLIER_HARDWARE_VERSION, _T("")),

#if MAP_RESOURCE_CHECK_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("��ͼ��Դ��С����Сֵ��"), IDC_STATIC_MAP_RESOURCE_SIZE, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("��ͼ��Դ��С�����ֵ��"), IDC_STATIC_MAP_RESOURCE_SIZE_MAX, _T("")),
#endif
#if NSATELLITE_CHECK_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("������Сֵ"), IDC_STATIC_NSATELLITE_NUMBER, _T("")),
#endif
	};

	//	GetDlgItem(IDC_EDIT20)->SetWindowText(ReadStr);	//�����
	InitHelper::LoadConfigUpdateUI(items, ARRAY_SIZE(items), cfg_file, _T("������Ϣ"), this);
	UpdateData(TRUE);
}

LPCTSTR CVersionInformationConfigurationDialog::GetMainBoardSoftwareVersion() const
{
	return main_board_software_version_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetFrontBoardSystemVersion() const
{
	return front_board_system_version_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetFrontBoardSoftwareVersion() const
{
	return front_board_software_version_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetNavSoftwareVersion() const
{
	return nav_software_version_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetSupplierSoftwareVersion() const
{
	return supplier_software_version_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetMainBoardHardwareVersion() const
{
	return main_board_hardware_version_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetSupplierHardwareVersion() const
{
	return supplier_hardware_version_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetMinMapResourceSize() const
{
	return map_resource_size_min_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetMaxMapResourceSize() const
{
	return map_resource_size_max_;
}

LPCTSTR CVersionInformationConfigurationDialog::GetNsatelliteNumber() const
{
	return nsatellite_number_;
}
