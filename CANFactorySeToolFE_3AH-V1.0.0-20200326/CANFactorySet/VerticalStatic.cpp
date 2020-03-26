// VerticalStatic.cpp : ʵ���ļ�
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



// CVerticalStatic ��Ϣ�������

void CVerticalStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	/* ���Ʊ��� */
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(66,219,112));

	/* ���ƴ�ֱ�ı� */
	CFont font;
	CFont *pOldFont;

	font.CreateFont(rect.Width() - 10, 0, 2700, 2700, FW_NORMAL, 
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, _T("@����"));
	pOldFont = dc.SelectObject(&font);

	CString text;
	GetWindowText(text);
	dc.TextOutW(rect.Width() / 2 + 30, 20, text);
	dc.SelectObject(pOldFont);
}