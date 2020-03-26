#pragma once
#include "mytabctrl.h"
#include "CANSend.h"
#include "CANFactorySetDlg.h"
#include "afxwin.h"
#include "VerticalStatic.h"
// Main 对话框

class Main : public CDialog
{
	DECLARE_DYNAMIC(Main)

public:
	Main(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Main();

// 对话框数据
	enum { IDD = IDD_Main };

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CCANSend* CANDlg;
public:
	CMyTabCtrl m_TabCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListBox m_ListBox;
	void Show_Box(CString dat);
	void GetEditText();
	void DisTxData(BYTE *byData,int inum,UINT DLC);
	afx_msg void On32776();
	afx_msg void On32777();
	afx_msg void On32778();
	afx_msg void On32779();
	afx_msg void On32780();
	afx_msg void On32781();
	afx_msg void On32782();
	afx_msg void On32783();

protected:
	afx_msg LRESULT OnSubMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void On32771();
	afx_msg void On32775();
	afx_msg void On32784();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void FileCreate();
	CVerticalStatic machine_kind_;
};
