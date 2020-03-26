#pragma once
#include "afxwin.h"

#define PASSWORD	_T("FE_3AH_20200326")

// CPasswordVerifyDialog 对话框

class CPasswordVerifyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordVerifyDialog)

public:
	CPasswordVerifyDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPasswordVerifyDialog();

	// 对话框数据
	enum { IDD = IDD_PASSWORDVERIFYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit password_;
	CString password_str_;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
