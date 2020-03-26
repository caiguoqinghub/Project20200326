// Error.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "Error.h"
#include "afxdialogex.h"


// CError 对话框

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


// CError 消息处理程序


void CError::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect   rect;  
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,0,0));
	// 不为绘图消息调用 CDialogEx::OnPaint()
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
	// TODO:  在此更改 DC 的任何特性
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
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CError::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常: OCX 属性页应返回 FALSE
}


void CError::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
 //   GetDlgItem(IDD_Version)->EnableWindow(TRUE);
	::SendMessage(MainHwnd,WM_SUB_MSG,DLGIDD,NULL);
	CDialogEx::OnClose();
}
