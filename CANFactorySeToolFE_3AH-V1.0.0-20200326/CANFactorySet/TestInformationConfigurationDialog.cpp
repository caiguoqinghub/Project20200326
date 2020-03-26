// TestInformationConfigurationDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "TestInformationConfigurationDialog.h"
#include "afxdialogex.h"
#include "IniHelper.h"



// CTestInformationConfigurationDialog �Ի���

IMPLEMENT_DYNAMIC(CTestInformationConfigurationDialog, CMyDialogEx)

CTestInformationConfigurationDialog::CTestInformationConfigurationDialog(CWnd* pParent /*=NULL*/)
	: CMyDialogEx(CTestInformationConfigurationDialog::IDD, pParent), cfg_file_(_T(""))
	, ecu_software_version_(_T(""))
	, ecu_hardware_version_(_T(""))
	, part_number_(_T(""))
	, supplier_identifier_(_T(""))
{

}

CTestInformationConfigurationDialog::~CTestInformationConfigurationDialog()
{
}

void CTestInformationConfigurationDialog::LoadConfig(LPCTSTR cfg_file)
{
	InitHelper::FieldShowIDMap items[] = {
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ECU����汾��"), IDC_STATIC_SOFTWARE_VERSION, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ECUӲ���汾��"), IDC_STATIC_HARDWARE_VERSION2, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("��Ӧ��ʶ���"), IDC_STATIC_SUPPLIER_IDENTIFIER, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("�����"), IDC_STATIC_PART_NUMBER, _T("")),
	};

	//	GetDlgItem(IDC_EDIT20)->SetWindowText(ReadStr);	//�����
	InitHelper::LoadConfigUpdateUI(items, ARRAY_SIZE(items), cfg_file, _T("������Ϣ"), this);
	UpdateData(TRUE);
}

UINT CTestInformationConfigurationDialog::DoModal(LPCTSTR file)
{
	cfg_file_ = file;
	return __super::DoModal();
}

LPCTSTR CTestInformationConfigurationDialog::GetECUSoftwareVersion()
{
	return ecu_software_version_;
}

LPCTSTR CTestInformationConfigurationDialog::GetECUHardeardVersion()
{
	return ecu_hardware_version_;
}

LPCTSTR CTestInformationConfigurationDialog::GetPartNumber()
{
	return part_number_;
}

LPCTSTR CTestInformationConfigurationDialog::GetSupplierIdentifier()
{
	return supplier_identifier_;
}

void CTestInformationConfigurationDialog::DoDataExchange(CDataExchange* pDX)
{
	CMyDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_SOFTWARE_VERSION, ecu_software_version_);
	DDX_Text(pDX, IDC_STATIC_HARDWARE_VERSION2, ecu_hardware_version_);
	DDX_Text(pDX, IDC_STATIC_PART_NUMBER, part_number_);
	DDX_Text(pDX, IDC_STATIC_SUPPLIER_IDENTIFIER, supplier_identifier_);
}


BEGIN_MESSAGE_MAP(CTestInformationConfigurationDialog, CMyDialogEx)
END_MESSAGE_MAP()

// CTestInformationConfigurationDialog ��Ϣ�������


BOOL CTestInformationConfigurationDialog::OnInitDialog()
{
	CMyDialogEx::OnInitDialog();

	LoadConfig(cfg_file_);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}