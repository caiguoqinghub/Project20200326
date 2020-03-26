// MyDialogEx.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "MyDialogEx.h"
#include "afxdialogex.h"


// CMyDialogEx 对话框

IMPLEMENT_DYNAMIC(CMyDialogEx, CDialogEx)

	CMyDialogEx::CMyDialogEx(UINT id, CWnd* pParent /*=NULL*/)
	: CDialogEx(id, pParent), backgroud_color_(RGB(66,219,112)), static_color_(RGB(0, 0, 0))
{

}

CMyDialogEx::~CMyDialogEx()
{
}

void CMyDialogEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyDialogEx, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMyDialogEx 消息处理程序


void CMyDialogEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, backgroud_color_);
}


HBRUSH CMyDialogEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(static_color_);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}

	return hbr;
}

void CMyDialogEx::SetBackgroundColor(COLORREF backgroud_color)
{
	backgroud_color_ = backgroud_color;
	if (GetSafeHwnd()) {
		Invalidate();
	}
}

COLORREF CMyDialogEx::GetBackgroundColor() const
{
	return backgroud_color_;
}

void CMyDialogEx::SetStaticColor(COLORREF static_color)
{
	static_color_ = static_color;
	if (GetSafeHwnd()) {
		Invalidate();
	}
}

COLORREF CMyDialogEx::GetStaticColor() const
{
	return static_color_;
}
