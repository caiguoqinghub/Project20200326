// PASS.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "PASS.h"
#include "afxdialogex.h"


// CPASS 对话框

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


// CPASS 消息处理程序


BOOL CPASS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常: OCX 属性页应返回 FALSE
}


void CPASS::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(66,219,112));
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


HBRUSH CPASS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if ((nCtlColor == CTLCOLOR_STATIC) && (pWnd->GetDlgCtrlID() == IDC_STC_PASS))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CPASS::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::SendMessage(MainHwnd,WM_SUB_MSG,DLGIDD,NULL);
	CDialogEx::OnClose();
}

void CPASS::SetPASSMSG(UINT DLGID,CString Str)
{
	DLGIDD = DLGID;
	SetDlgItemText(IDC_STC_PASS,Str);
}