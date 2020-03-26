// PreReleaseBtnST.cpp : 实现文件
//

#include "stdafx.h"
//#include "CarTest.h"
#include "PreReleaseBtnST.h"


// CPreReleaseButtonST

IMPLEMENT_DYNAMIC(CPreReleaseButtonST, CButton)

CPreReleaseButtonST::CPreReleaseButtonST()
{

}

CPreReleaseButtonST::~CPreReleaseButtonST()
{
}


BEGIN_MESSAGE_MAP(CPreReleaseButtonST, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CPreReleaseButtonST 消息处理程序




void CPreReleaseButtonST::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
         WPARAM wParam = (((DWORD)WM_LBUTTONDOWN)<<16)+((DWORD)GetDlgCtrlID());
        ::SendMessage(this->GetParent()->GetSafeHwnd(),WM_COMMAND,wParam,0);
	CButton::OnLButtonDown(nFlags, point);
}


void CPreReleaseButtonST::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	  	     WPARAM wParam = (((DWORD)WM_LBUTTONUP)<<16)+((DWORD)GetDlgCtrlID());
	         ::SendMessage(this->GetParent()->GetSafeHwnd(),WM_COMMAND,wParam,0);

	CButton::OnLButtonUp(nFlags, point);
}


void CPreReleaseButtonST::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnLButtonDown(0,0);
	CShadeButtonST::OnLButtonDblClk(nFlags, point);

}