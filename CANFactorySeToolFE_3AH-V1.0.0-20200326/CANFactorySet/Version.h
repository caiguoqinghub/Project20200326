#pragma once
#include "btnst.h"
#include "CANFactorySetDlg.h"

// CVersion 对话框

class CVersion : public CDialogEx
{
	DECLARE_DYNAMIC(CVersion)

public:
	CVersion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVersion();

// 对话框数据
	enum { IDD = IDD_Version };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CError* Dlg_Error;
	CPASS* Dlg_PASS;
public:
	void ButInit(void);
	void SendShowData(CString Str);
	void DataAnalyze(void);
	void DataCmp(UINT VersionID,UINT Num);
	UINT Compare(BYTE *dat0,BYTE *dat1,UINT Len);
	void ByteStuffing(BYTE *data/*数据*/,UINT DataLen/*数据长度*/,UINT FinalLen/*最终数据长度*/); 
	int HexToDem(CString str);
	void StrToByte(BYTE *byte,CString Str);
	void SWModeShow(void);
	CCANSend CANSendDlg;
	void SetDlgTure(void);
	void savedata();//保存数据到文件（设置数据具体格式）
	void Dataread();//读取文件
public:
	afx_msg void OnBnClickedButton1();

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	// AC配置
	CButtonST m_But_AC_ON;
	CButtonST m_But_AC_OFF;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton9();
private:
	BOOL triggered_by_key_;
};
