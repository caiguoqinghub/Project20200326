// StartupDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "StartupDialog.h"
#include "afxdialogex.h"


// CStartupDialog 对话框

IMPLEMENT_DYNAMIC(CStartupDialog, CMyDialogEx)

CStartupDialog::CStartupDialog(CWnd* pParent /*=NULL*/)
	: CMyDialogEx(CStartupDialog::IDD, pParent)
{

}

CStartupDialog::~CStartupDialog()
{
}

void CStartupDialog::DoDataExchange(CDataExchange* pDX)
{
	CMyDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartupDialog, CMyDialogEx)
END_MESSAGE_MAP()


// CStartupDialog 消息处理程序
