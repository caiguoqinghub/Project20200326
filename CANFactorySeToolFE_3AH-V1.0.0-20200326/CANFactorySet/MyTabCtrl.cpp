// MyTabCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "MyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{
	dlg1 = new CCANFactorySetDlg;
	dlg2 = new CVersion;
	m_TabPages[0] = dlg1;
	m_TabPages[1] = dlg2;
	m_NumberOfPages = 2;
}

CMyTabCtrl::~CMyTabCtrl()
{

	for (UINT nCount = 0;nCount < 2;nCount++)
	{
		delete m_TabPages[nCount];
	}
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CMyTabCtrl ��Ϣ�������

void CMyTabCtrl::Init()
{
	m_TabCurrent = 0;
	m_TabPages[0]->Create(IDD_CANFACTORYSET_DIALOG,this);
	m_TabPages[0]->ShowWindow(SW_SHOW);
	m_TabPages[1]->Create(IDD_Version,this);
	m_TabPages[1]->ShowWindow(SW_HIDE);
	SetRectangle();
}

void CMyTabCtrl::SetRectangle()
{
	CRect tabRect,itemRect;
	int nX,nY,nXc,nYc;

	GetClientRect(&tabRect);
	GetItemRect(0,&itemRect);

	nX = itemRect.left;						   //����TAB���Ҳ����� �Ի���������ƶ�
	nY = itemRect.bottom + 2;					   //����TAB���²����� �Ի���������ƶ�
	nXc = tabRect.right - itemRect.left - 3;   //����TAB�Ҳ�����   �Ի���Ŀ��
	nYc = tabRect.bottom - itemRect.bottom - 4;//����TAB�ײ�����   �Ի���ĸ߶�

	m_TabPages[0]->SetWindowPos(&wndTop,nX,nY,nXc,nYc,SWP_SHOWWINDOW);
	for (int nCount=1; nCount < m_NumberOfPages; nCount++){
		m_TabPages[nCount]->SetWindowPos(&wndTop,nX,nY,nXc,nYc,SWP_HIDEWINDOW);
	}
}

void CMyTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CTabCtrl::OnLButtonDown(nFlags, point);
	if (m_TabCurrent != GetCurFocus()){
		m_TabPages[m_TabCurrent]->ShowWindow(SW_HIDE);
		m_TabCurrent = GetCurFocus();
		m_TabPages[m_TabCurrent]->ShowWindow(SW_SHOW);
		m_TabPages[m_TabCurrent]->SetFocus();
	}
}
