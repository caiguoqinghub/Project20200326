#pragma once
#include "afxwin.h"
#include "MyDialogEx.h"

// CVersionInformationConfigurationDialog 对话框

class CVersionInformationConfigurationDialog : public CMyDialogEx
{
	DECLARE_DYNAMIC(CVersionInformationConfigurationDialog)

public:
	CVersionInformationConfigurationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVersionInformationConfigurationDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_VERSION_INFORMATION_CONFIGURATION };

	UINT DoModal(LPCTSTR cfg_file);

	LPCTSTR GetMainBoardSoftwareVersion() const;
	LPCTSTR GetFrontBoardSystemVersion() const;
	LPCTSTR GetFrontBoardSoftwareVersion() const;
	LPCTSTR GetNavSoftwareVersion() const;
	LPCTSTR GetSupplierSoftwareVersion() const;
	LPCTSTR GetMainBoardHardwareVersion() const;
	LPCTSTR GetSupplierHardwareVersion() const;
	LPCTSTR GetMinMapResourceSize() const;
	LPCTSTR GetMaxMapResourceSize() const;
	LPCTSTR GetNsatelliteNumber() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void LoadConfig(LPCTSTR cfg_file);

	DECLARE_MESSAGE_MAP()
public:
	CString main_board_software_version_;
	CString front_board_system_version_;
	CString front_board_software_version_;
	CString nav_software_version_;

	CString supplier_software_version_;
	CString main_board_hardware_version_;
	CString supplier_hardware_version_;

	CString map_resource_size_min_;
	CString map_resource_size_max_;
	CString nsatellite_number_;

private:
	LPCTSTR cfg_file_;
public:
	virtual BOOL OnInitDialog();

};
