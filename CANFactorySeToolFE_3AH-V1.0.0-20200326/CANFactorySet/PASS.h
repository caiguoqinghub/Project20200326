#pragma once
#include "CANSend.h"

// CPASS 对话框

class CPASS : public CDialogEx
{
	DECLARE_DYNAMIC(CPASS)

public:
	CPASS(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPASS();

// 对话框数据
	enum { IDD = IDD_PASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	UINT DLGIDD;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	void SetPASSMSG(UINT DLGID,CString Str);
};
