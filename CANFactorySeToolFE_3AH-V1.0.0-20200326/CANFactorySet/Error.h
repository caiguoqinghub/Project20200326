#pragma once
#include "CANSend.h"

// CError �Ի���

class CError : public CDialogEx
{
	DECLARE_DYNAMIC(CError)

public:
	CError(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CError();

// �Ի�������
	enum { IDD = IDD_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	UINT DLGIDD;
public:
	void SetErrorMsg(UINT DLGID,CString Str);
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
