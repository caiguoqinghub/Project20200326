// Error.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "Error.h"
#include "afxdialogex.h"


// CError �Ի���

IMPLEMENT_DYNAMIC(CError, CDialogEx)

CError::CError(CWnd* pParent /*=NULL*/)
	: CDialogEx(CError::IDD, pParent)
{

}

CError::~CError()
{
}

void CError::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CError, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CError ��Ϣ�������


void CError::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect   rect;  
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,0,0));
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

void CError::SetErrorMsg(UINT DLGID,CString Str)
{
	DLGIDD = DLGID;
	SetDlgItemText(IDC_STC_Error,Str);
	return;
}

HBRUSH CError::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
	if ((nCtlColor == CTLCOLOR_STATIC) && (pWnd->GetDlgCtrlID() == IDC_STC_Error))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	if ((nCtlColor == CTLCOLOR_STATIC) && (pWnd->GetDlgCtrlID() == IDC_STATIC1))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CError::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CFont* font,*sont;
	font	= new CFont;
	sont	= new CFont;
	sont->CreateFont(60,
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
	font->CreateFont(40,
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
	GetDlgItem(IDC_STC_Error)->SetFont(font);
	GetDlgItem(IDC_STATIC1)->SetFont(sont);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CError::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
 //   GetDlgItem(IDD_Version)->EnableWindow(TRUE);
	::SendMessage(MainHwnd,WM_SUB_MSG,DLGIDD,NULL);
	CDialogEx::OnClose();
}
