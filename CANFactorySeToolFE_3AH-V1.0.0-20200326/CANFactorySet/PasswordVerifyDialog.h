#pragma once
#include "afxwin.h"

#define PASSWORD	_T("FE_3AH_20200326")

// CPasswordVerifyDialog �Ի���

class CPasswordVerifyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordVerifyDialog)

public:
	CPasswordVerifyDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPasswordVerifyDialog();

	// �Ի�������
	enum { IDD = IDD_PASSWORDVERIFYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit password_;
	CString password_str_;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
