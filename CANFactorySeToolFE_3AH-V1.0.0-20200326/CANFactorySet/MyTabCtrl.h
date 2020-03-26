#pragma once

// CMyTabCtrl
#include "CANFactorySetDlg.h"
#include "Version.h"
class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

	CCANFactorySetDlg* dlg1;
	CVersion* dlg2;
	CDialog *m_TabPages[2];	
	int m_TabCurrent;
	int m_NumberOfPages; 
public:
	void Init(void);
	void SetRectangle(void);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


