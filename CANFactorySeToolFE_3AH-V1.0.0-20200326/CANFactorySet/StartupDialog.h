#pragma once
#include "MyDialogEx.h"

// CStartupDialog �Ի���

class CStartupDialog : public CMyDialogEx
{
	DECLARE_DYNAMIC(CStartupDialog)

public:
	CStartupDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartupDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_STARTUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
