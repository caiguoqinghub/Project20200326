// PasswordVerifyDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "PasswordVerifyDialog.h"
#include "afxdialogex.h"


// CPasswordVerifyDialog 对话框

IMPLEMENT_DYNAMIC(CPasswordVerifyDialog, CDialogEx)

	CPasswordVerifyDialog::CPasswordVerifyDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPasswordVerifyDialog::IDD, pParent)
	, password_str_(_T(""))
{

}

CPasswordVerifyDialog::~CPasswordVerifyDialog()
{
}

void CPasswordVerifyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, password_);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, password_str_);
}


BEGIN_MESSAGE_MAP(CPasswordVerifyDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPasswordVerifyDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPasswordVerifyDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPasswordVerifyDialog 消息处理程序


void CPasswordVerifyDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	password_str_.MakeUpper();
	if (password_str_ == PASSWORD)
	{
		return CDialogEx::OnOK();
	}

	AfxMessageBox(_T("密码输入错误"));
}


void CPasswordVerifyDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
