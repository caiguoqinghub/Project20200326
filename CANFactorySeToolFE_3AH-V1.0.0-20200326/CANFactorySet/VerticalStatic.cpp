// VerticalStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "VerticalStatic.h"


// CVerticalStatic

IMPLEMENT_DYNAMIC(CVerticalStatic, CStatic)

	CVerticalStatic::CVerticalStatic()
{

}

CVerticalStatic::~CVerticalStatic()
{
}


void CVerticalStatic::SetWindowText(LPCTSTR text)
{
	CStatic::SetWindowText(text);
	Invalidate();
}

BEGIN_MESSAGE_MAP(CVerticalStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CVerticalStatic 消息处理程序

void CVerticalStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	/* 绘制背景 */
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(66,219,112));

	/* 绘制垂直文本 */
	CFont font;
	CFont *pOldFont;

	font.CreateFont(rect.Width() - 10, 0, 2700, 2700, FW_NORMAL, 
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("@宋体"));
	pOldFont = dc.SelectObject(&font);

	CString text;
	GetWindowText(text);
	dc.TextOutW(rect.Width() / 2 + 30, 20, text);
	dc.SelectObject(pOldFont);
}