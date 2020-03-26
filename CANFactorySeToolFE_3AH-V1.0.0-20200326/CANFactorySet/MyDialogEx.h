#pragma once


// CMyDialogEx 对话框

class CMyDialogEx : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialogEx)

public:
	CMyDialogEx(UINT id, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDialogEx();

	// 对话框数据
	enum { IDD = IDD_MYDIALOGEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void SetBackgroundColor(COLORREF backgroud_color);
	COLORREF GetBackgroundColor() const;

	void SetStaticColor(COLORREF static_color);
	COLORREF GetStaticColor() const;

private:
	COLORREF backgroud_color_;
	COLORREF static_color_;
};
