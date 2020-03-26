#pragma once
#include "btnst.h"
#include "CANFactorySetDlg.h"

// CVersion �Ի���

class CVersion : public CDialogEx
{
	DECLARE_DYNAMIC(CVersion)

public:
	CVersion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVersion();

// �Ի�������
	enum { IDD = IDD_Version };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CError* Dlg_Error;
	CPASS* Dlg_PASS;
public:
	void ButInit(void);
	void SendShowData(CString Str);
	void DataAnalyze(void);
	void DataCmp(UINT VersionID,UINT Num);
	UINT Compare(BYTE *dat0,BYTE *dat1,UINT Len);
	void ByteStuffing(BYTE *data/*����*/,UINT DataLen/*���ݳ���*/,UINT FinalLen/*�������ݳ���*/); 
	int HexToDem(CString str);
	void StrToByte(BYTE *byte,CString Str);
	void SWModeShow(void);
	CCANSend CANSendDlg;
	void SetDlgTure(void);
	void savedata();//�������ݵ��ļ����������ݾ����ʽ��
	void Dataread();//��ȡ�ļ�
public:
	afx_msg void OnBnClickedButton1();

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	// AC����
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
