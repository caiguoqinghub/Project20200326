#pragma once


// CMyDialogEx �Ի���

class CMyDialogEx : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialogEx)

public:
	CMyDialogEx(UINT id, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyDialogEx();

	// �Ի�������
	enum { IDD = IDD_MYDIALOGEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
