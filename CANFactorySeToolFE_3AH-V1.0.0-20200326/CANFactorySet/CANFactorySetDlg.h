
// CANFactorySetDlg.h : 头文件
//
#pragma once
#include "CANSend.h"
#include "BkDialog.h"
#include "ShadeButtonST.h"
#include "afxwin.h"
#include "btnst.h"
#include "Error.h"
#include "PASS.h"
//#include "Version.h"
//#include "MyTabCtrl.h"
#define TxMsgTimer     0
#define RxMsgTimer     1


#define TestMode(DID,DIDCMD)     			if (ToolModel == 2){Delay(300);CCANSend::Single_Frame(TestID,0x22,DID,DIDCMD,NULL,0);SetFlg = 0;return;}/*if (ToolModel == 2){KillTimer(3);SetFlg = 0;MessageBox(L"设置成功！",L"提示");;return;}*/
#define QAMode(DID,DIDCMD)     			if (ToolModel == 3){Delay(300);CCANSend::Single_Frame(TestID,0x22,DID,DIDCMD,NULL,0);SetFlg = 0;return;}/*if (ToolModel == 3){KillTimer(3);SetFlg = 0;MessageBox(L"设置成功！",L"提示");;return;}*/


extern BYTE NetWorkConfigData[8];

// CCANFactorySetDlg 对话框
class CCANFactorySetDlg : public CBkDialog/*CDialogEx*/ , public CCANSend
{
// 构造
public:
	CCANFactorySetDlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
	enum { IDD = IDD_CANFACTORYSET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	HICON m_hIcon;
	CError* Dlg_Error;
	CPASS* Dlg_PASS;
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//CMyTabCtrl m_TabCtrl;
	void EnterTextMode(void);
	void ExitTextmode(void);
	void SetFactoryMode(void);
	void SetReadMode(void);
	void SetQAMode(void);
	void SetTestMode(void);
	void SendShowData(CString Str);
	void SetDlgTrue(void);
	void ByteStuffing(BYTE *data/*数据*/,UINT DataLen/*数据长度*/,UINT FinalLen/*最终数据长度*/); //字节填充

//	void GetEditText(void);

public: 
	void Init_Control(void);
	void DataAnalyze(void);
	void ECUMakeDate(void);
	void ECUSWDate(void);
	void ECUConfigDate(void);
	UINT GetSNDate(void);
// 	void Show_Box(CString dat);
// 	void DisTxData(BYTE *byData,int inum,UINT DLC);
	void SetCarName(void);
	void SetCarVIN(void);
	void SetDiagnoseSN(void);
	void SetECUMakeDate(void);
	void SetECUSWDate(void);
	void SetECUConfigDate(void);
	void SetMMIConfig(void);
	void SetNetworkConfig(void);
	void SetSN(void);
	void SetECUHWVersion(void);
	void SetECUSWVersion(void);
	void SetPartNumber(void);
	void SetSystemSupplierId(void);
	void DataCmp(UINT Num);
	void strtoarray(void);

	UINT Compare(BYTE *dat0,BYTE *dat1,UINT Len);
	//扫码枪功能函数
	void dataconfig(void);//数据配置
	void savedata(void);
	void Dataread(void);
	void Honeywell_judgment(void);
public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

// 	CShadeButtonST m_IntoTest;
// 	CShadeButtonST m_ExitTest;
	CShadeButtonST m_But_CarName;
	CShadeButtonST m_But_CarVIN;
	CShadeButtonST m_But_DiagnoseSN;
	CEdit m_Edit_CarName;
	CString m_Edit_CarNameS;
	CEdit m_Edit_CarVIN;
	CString m_Edit_CarVINS;
	CEdit m_Edit_DiagnoseSN;
	CString m_Edit_DiagnoseSNS;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CShadeButtonST m_Set;
	CShadeButtonST m_Read;
	afx_msg void OnBnClickedButton6();
	CShadeButtonST m_But_ECU_MakeDate;
	CShadeButtonST m_But_ECU_SWDate;
	CShadeButtonST m_But_ECU_ConfigDate;
	CEdit m_Edit_Year0;
	CString m_Edit_Year0S;
	CEdit m_Edit_Year1;
	CString m_Edit_Year1S;
	CEdit m_Edit_Year2;
	CString m_Edit_Year2S;
	CEdit m_Edit_month0;
	CString m_Edit_month0S; 
	CEdit m_Edit_month1;
	CString m_Edit_month1S;         
	CEdit m_Edit_month2;
	CString m_Edit_month2S; 
	CEdit m_Edit_Day0; 
	CString m_Edit_Day0S; 
	CEdit m_Edit_Day1;
	CString m_Edit_Day1S; 
	CEdit m_Edit_Day2;
	CString m_Edit_Day2S; 
	CShadeButtonST m_NetworkConfig0;
	CShadeButtonST m_NetworkConfig1;
	CShadeButtonST m_TPMS0;
	CShadeButtonST m_TPMS1;
	CShadeButtonST m_BCM_Rmirror0;
	CShadeButtonST m_BCM_Rmirror1;
	CShadeButtonST m_Auto_CloseWindow0;
	CShadeButtonST m_Auto_CloseWindow1;
	CShadeButtonST m_cx_wpp;
	CShadeButtonST m_cx_3ah;
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton26();
	CShadeButtonST m_But_SNSet;
	CString m_Edit_SN;
	CString m_Edit_LN;
	CString m_Edit_SN_Year;
	CString m_Edit_SN_Month;
	CString m_Edit_SN_Day;
	afx_msg void OnStnClickedStatic31();
	CListBox m_ListBox;

/*	afx_msg void OnBnClickedButton22();*/
/*	afx_msg void OnBnClickedButton21();*/
// 	CShadeButtonST m_Mode0; //工厂
// 	CShadeButtonST m_Mode1; //读
// 	CShadeButtonST m_Mode2; //测试
// 	CShadeButtonST m_Mode3; //QA
// 	afx_msg void OnBnClickedButton23();
// 	afx_msg void OnBnClickedButton24();
// 	afx_msg void OnBnClickedButton27();

	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton20();
	CShadeButtonST m_ESC_Config0;
	CShadeButtonST m_ESC_Config1;
	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton29();
	// 高低配设置
	CComboBox m_Config;
	afx_msg void OnCbnSelchangeCombo1();
/*	afx_msg void OnBnClickedButton30();*/
	
	CButton m_NumAdd;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg void OnEnChangeEdit1();
	CShadeButtonST m_tcu_config0;
	CShadeButtonST m_tcu_config1;
	CShadeButtonST m_acu_config0;
	CShadeButtonST m_acu_config1;
	CShadeButtonST m_esc_config0;
	CShadeButtonST m_esc_config1;
	CShadeButtonST m_eps_config0;
	CShadeButtonST m_eps_config1;
	CShadeButtonST m_ems_config0;
	CShadeButtonST m_ems_config1;
	CShadeButtonST m_ipk_config0;
	CShadeButtonST m_ipk_config1;
	CShadeButtonST m_mmi_config0;
	CShadeButtonST m_mmi_config1;
	CShadeButtonST m_ac_config0;
	CShadeButtonST m_ac_config1;
	CShadeButtonST m_sas_config0;
	CShadeButtonST m_sas_config1;
	CShadeButtonST m_abs_config0;
	CShadeButtonST m_abs_config1;
	CShadeButtonST m_epb_config0;
	CShadeButtonST m_epb_config1;
	CShadeButtonST m_cx_FE_3AH;

	CShadeButtonST m_rooffj0;
	CShadeButtonST m_rooffj1;
	CShadeButtonST m_gw;
	CShadeButtonST m_gl;
	CShadeButtonST m_xc;
	afx_msg void OnBnClickedButton30();
	afx_msg void OnBnClickedButton31();
	afx_msg void OnBnClickedButton32();
	afx_msg void OnBnClickedButton33();
	afx_msg void OnBnClickedButton34();
	afx_msg void OnBnClickedButton35();
	afx_msg void OnBnClickedButton36();
	afx_msg void OnBnClickedButton37();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedButton38();
	afx_msg void OnBnClickedButton39();
	afx_msg void OnBnClickedButton40();
	afx_msg void OnBnClickedButton41();
	afx_msg void OnBnClickedButton42();
	afx_msg void OnBnClickedButton43();
	afx_msg void OnBnClickedButton44();
	afx_msg void OnBnClickedButton45();
	afx_msg void OnBnClickedButton46();
	afx_msg void OnBnClickedButton47();
	afx_msg void OnBnClickedButton48();
	afx_msg void OnBnClickedButton50();
	afx_msg void OnBnClickedButton51();
	afx_msg void OnBnClickedButton52();
	afx_msg void OnBnClickedButton53();
	afx_msg void OnBnClickedButton54();
	afx_msg void OnBnClickedButton55();
	void LoadConfig(LPCTSTR cfg_file);
	CShadeButtonST m_xc_w;
private:
	void doOneKeySet();
	void loadConfig( const CString &FilePathName );
	void QHInit(void);
	void TuBiaohs(void);
	void TuBiaoys(void);
	BOOL trigger_by_set_key_;
public:
	afx_msg void OnBnClickedButton56();
	afx_msg void OnBnClickedButton57();
	CShadeButtonST m_kt0;
	CShadeButtonST m_kt1;
	CShadeButtonST m_Carmera0;
	CShadeButtonST m_Carmera1;
	CShadeButtonST m_Console_Ambient_Light0;
	CShadeButtonST m_Console_Ambient_Light1;
	afx_msg void OnBnClickedButton49();
	afx_msg void OnBnClickedButton58();
	afx_msg void OnBnClickedButton59();
	afx_msg void OnBnClickedButton60();
	afx_msg void OnBnClickedButton61();
	afx_msg void OnBnClickedButton62();
	CShadeButtonST gw_config0;
	CShadeButtonST gw_config1;
};
