#pragma once
#include "CANSend.h"

// CError 对话框

class CError : public CDialogEx
{
	DECLARE_DYNAMIC(CError)

public:
	CError(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CError();

// 对话框数据
	enum { IDD = IDD_ERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
