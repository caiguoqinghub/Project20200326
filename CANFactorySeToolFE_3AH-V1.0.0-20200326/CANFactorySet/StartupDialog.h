#pragma once
#include "MyDialogEx.h"

// CStartupDialog 对话框

class CStartupDialog : public CMyDialogEx
{
	DECLARE_DYNAMIC(CStartupDialog)

public:
	CStartupDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartupDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_STARTUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
