#pragma once


// CPreReleaseButtonST

class CPreReleaseButtonST : public CShadeButtonST
{
	DECLARE_DYNAMIC(CPreReleaseButtonST)

public:
	CPreReleaseButtonST();
	virtual ~CPreReleaseButtonST();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


