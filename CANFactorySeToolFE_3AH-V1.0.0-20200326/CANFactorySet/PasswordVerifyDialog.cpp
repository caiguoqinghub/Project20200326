// PasswordVerifyDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "PasswordVerifyDialog.h"
#include "afxdialogex.h"


// CPasswordVerifyDialog �Ի���

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


// CPasswordVerifyDialog ��Ϣ�������


void CPasswordVerifyDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	password_str_.MakeUpper();
	if (password_str_ == PASSWORD)
	{
		return CDialogEx::OnOK();
	}

	AfxMessageBox(_T("�����������"));
}


void CPasswordVerifyDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
