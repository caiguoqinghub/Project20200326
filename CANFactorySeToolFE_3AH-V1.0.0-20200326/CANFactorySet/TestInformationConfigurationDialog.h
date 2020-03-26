#pragma once
#include "afxwin.h"
#include "MyDialogEx.h"

// CTestInformationConfigurationDialog 对话框

class CTestInformationConfigurationDialog : public CMyDialogEx
{
	DECLARE_DYNAMIC(CTestInformationConfigurationDialog)

public:
	CTestInformationConfigurationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestInformationConfigurationDialog();

	// 对话框数据
	enum { IDD = IDD_DIALOG_TEST_INFORMATION_CONFIGURATION };

	void LoadConfig(LPCTSTR file);
	UINT DoModal(LPCTSTR file);

	LPCTSTR GetECUSoftwareVersion();
	LPCTSTR GetECUHardeardVersion();
	LPCTSTR GetPartNumber();
	LPCTSTR GetSupplierIdentifier();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	LPCTSTR cfg_file_;
public:
	virtual BOOL OnInitDialog();
	CString ecu_software_version_;
	CString ecu_hardware_version_;
	CString part_number_;
	CString supplier_identifier_;
};
