// PASS.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "PASS.h"
#include "afxdialogex.h"


// CPASS �Ի���

IMPLEMENT_DYNAMIC(CPASS, CDialogEx)

CPASS::CPASS(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPASS::IDD, pParent)
{

}

CPASS::~CPASS()
{
}

void CPASS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPASS, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPASS ��Ϣ�������


BOOL CPASS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CFont* font = new CFont;
	font->CreateFont(200,
		0,
		0,
		0,
		FW_BOLD, 
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_SWISS,
		_T("Arial")
		);
	GetDlgItem(IDC_STC_PASS)->SetFont(font);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPASS::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(66,219,112));
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


HBRUSH CPASS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if ((nCtlColor == CTLCOLOR_STATIC) && (pWnd->GetDlgCtrlID() == IDC_STC_PASS))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CPASS::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SendMessage(MainHwnd,WM_SUB_MSG,DLGIDD,NULL);
	CDialogEx::OnClose();
}

void CPASS::SetPASSMSG(UINT DLGID,CString Str)
{
	DLGIDD = DLGID;
	SetDlgItemText(IDC_STC_PASS,Str);
}