#pragma once


// CVerticalStatic

class CVerticalStatic : public CStatic
{
	DECLARE_DYNAMIC(CVerticalStatic)

public:
	CVerticalStatic();
	virtual ~CVerticalStatic();

	void SetWindowText(LPCTSTR text);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
};


