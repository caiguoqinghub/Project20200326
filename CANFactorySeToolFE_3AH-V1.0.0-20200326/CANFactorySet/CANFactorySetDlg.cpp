
// CANFactorySetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CANFactorySet.h"
#include "CANFactorySetDlg.h"
#include "afxdialogex.h"
#include "Main.h"
#include "MyTabCtrl.h"

#include "IniHelper.h"

#include "ExceptFrameFixer.h"
#include "FileFolderUtil.h"
#include "TestInformationConfigurationDialog.h"
//#include "CANSend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
#define  FactorySetDlg      2

#if 1
#define IS_MODEL_FIXER_ENABLED() \
	(FACTORY_TOOL_MODEL == ToolModel)
#else
#define IS_MODEL_FIXER_ENABLED() \
	((FACTORY_TOOL_MODEL == ToolModel)||(TEST_TOOL_MODEL == ToolModel))
#endif

CTime t=CTime::GetCurrentTime();
CString Year,Months,Date;
UINT ReadMode = 0;
BYTE KeyInit[]={'T','E','S','T'};
BYTE CarNameData[8] = {0,0,0,0,0,0,0,0};
BYTE CarVINData[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
BYTE DiagnoseSNData[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
BYTE ECUMakeDayData[4] = {0,0,0,0};
BYTE ECUSWDayData[4] = {0,0,0,0};
BYTE ECUConfigDayData[4] = {0,0,0,0};
BYTE NetWorkConfigData[8] = {0x7F,0x2D,0x00,0x00,0x40,0x00,0x00,0x00};
BYTE MMIConfigData[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//BYTE MMIConfigData[16] = {0x14,0x00,0xC1,0x48,0x0C,0x20,0x17,0x00,0x00,0x00,0x70,0x8A,0x00,0x00,0x00,0x00};
//BYTE MMIConfigData[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
BYTE SNData[24]= {'2','5','0','0','6','0','0','0','1','1','7','0','2','1','4','0',0,0,0,0,0,0,0,0};
//BYTE SNData[24]= {'2','5','0','0','6','0','0','0','1','1','7','0','1','9','1','9',0,0,0,0,0,0,0,0};
BYTE *PartNumberBuf = new BYTE[100];
BYTE *SystemSupplierIdBuf = new BYTE[100];

BYTE *HWVersion = new BYTE[100];
BYTE *SWVersion = new BYTE[100];
CString SNStr,SNStr1,SNStr2,SNStr3,SNStr4,ECUMakeDayStr0,ECUMakeDayStr1,ECUMakeDayStr2,ECUSWDayStr0,ECUSWDayStr1,ECUSWDayStr2,ECUConfigDayStr0,ECUConfigDayStr1,ECUConfigDayStr2,CarNameStr,CarVINStr,DiagnoseSNStr;
		//比对错误信息
CString ErrorStr=0;
UINT NumAdd = 0;
int iSum=0;
CString pNewName=0;//工厂设置错误命名
CString pNewName1=0;//版本比对错误命名
CString ERRORSTR;//传递工厂设置错误信息（确保信息不被置零）
// class CAboutDlg : public /*CDialogEx*/CBkDialog
// {
// public:
// 	CAboutDlg();
// 
// // 对话框数据
// 	enum { IDD = IDD_ABOUTBOX };
// 
// 	protected:
// 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
// 
// // 实现
// protected:
// 	DECLARE_MESSAGE_MAP()
// };
// 
// CAboutDlg::CAboutDlg() : /*CDialogEx*/CBkDialog(CAboutDlg::IDD)
// {
// }
// 
// void CAboutDlg::DoDataExchange(CDataExchange* pDX)
// {
// 	/*CDialogEx*/CBkDialog::DoDataExchange(pDX);
// }
// 
// BEGIN_MESSAGE_MAP(CAboutDlg, /*CDialogEx*/CBkDialog)
// END_MESSAGE_MAP()


// CCANFactorySetDlg 对话框




CCANFactorySetDlg::CCANFactorySetDlg(CWnd* pParent /*=NULL*/)
	: /*CDialogEx*/CBkDialog(CCANFactorySetDlg::IDD, pParent), trigger_by_set_key_(FALSE)
{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCANFactorySetDlg::DoDataExchange(CDataExchange* pDX)
{
	/*CDialogEx*/CBkDialog::DoDataExchange(pDX);
	// 	DDX_Control(pDX, IDC_BUTTON1, m_IntoTest);
	// 	DDX_Control(pDX, IDC_BUTTON2, m_ExitTest);
	DDX_Control(pDX, IDC_BUTTON3, m_But_CarName);
	DDX_Control(pDX, IDC_BUTTON4, m_But_CarVIN);
	DDX_Control(pDX, IDC_BUTTON5, m_But_DiagnoseSN);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_CarName);
	DDX_Text(pDX,IDC_EDIT1,m_Edit_CarNameS);
	DDV_MaxChars(pDX,m_Edit_CarNameS,8);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_CarVIN);
	DDX_Text(pDX,IDC_EDIT2,m_Edit_CarVINS);
	DDV_MaxChars(pDX,m_Edit_CarVINS,17);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_DiagnoseSN);
	DDX_Text(pDX,IDC_EDIT3,m_Edit_DiagnoseSNS);
	DDV_MaxChars(pDX,m_Edit_DiagnoseSNS,16);
	DDX_Control(pDX, IDC_BUTTON6, m_Set);
	DDX_Control(pDX, IDC_BUTTON7, m_Read);
	DDX_Control(pDX, IDC_BUTTON8, m_But_ECU_MakeDate);
	DDX_Control(pDX, IDC_BUTTON9, m_But_ECU_SWDate);
	DDX_Control(pDX, IDC_BUTTON10, m_But_ECU_ConfigDate);
	DDX_Control(pDX, IDC_EDIT4, m_Edit_Year0);
	DDX_Text(pDX,IDC_EDIT4,m_Edit_Year0S);
	DDV_MaxChars(pDX,m_Edit_Year0S,4);
	DDX_Control(pDX, IDC_EDIT7, m_Edit_Year1);
	DDX_Text(pDX,IDC_EDIT7,m_Edit_Year1S);
	DDV_MaxChars(pDX,m_Edit_Year1S,4);
	DDX_Control(pDX, IDC_EDIT10, m_Edit_Year2);
	DDX_Text(pDX,IDC_EDIT10,m_Edit_Year2S);
	DDV_MaxChars(pDX,m_Edit_Year2S,4);
	DDX_Control(pDX, IDC_EDIT5, m_Edit_month0);
	DDX_Text(pDX,IDC_EDIT5,m_Edit_month0S);
	DDV_MaxChars(pDX,m_Edit_month0S,2);
	DDX_Control(pDX, IDC_EDIT8, m_Edit_month1);
	DDX_Text(pDX,IDC_EDIT8,m_Edit_month1S);
	DDV_MaxChars(pDX,m_Edit_month1S,2);
	DDX_Control(pDX, IDC_EDIT11, m_Edit_month2);
	DDX_Text(pDX,IDC_EDIT11,m_Edit_month2S);
	DDV_MaxChars(pDX,m_Edit_month2S,2);
	DDX_Control(pDX, IDC_EDIT6, m_Edit_Day0);
	DDX_Text(pDX,IDC_EDIT6,m_Edit_Day0S);
	DDV_MaxChars(pDX,m_Edit_Day0S,2);
	DDX_Control(pDX, IDC_EDIT9, m_Edit_Day1);
	DDX_Text(pDX,IDC_EDIT9,m_Edit_Day1S);
	DDV_MaxChars(pDX,m_Edit_Day1S,2);
	DDX_Control(pDX, IDC_EDIT12, m_Edit_Day2);
	DDX_Text(pDX,IDC_EDIT12,m_Edit_Day2S);
	DDV_MaxChars(pDX,m_Edit_Day2S,2);
	DDX_Control(pDX, IDC_BUTTON11, m_NetworkConfig0);
	DDX_Control(pDX, IDC_BUTTON12, m_NetworkConfig1);
	DDX_Control(pDX, IDC_BUTTON13, m_TPMS0);
	DDX_Control(pDX, IDC_BUTTON14, m_TPMS1);
	DDX_Control(pDX, IDC_BUTTON15, m_BCM_Rmirror0);
	DDX_Control(pDX, IDC_BUTTON16, m_BCM_Rmirror1);
	DDX_Control(pDX, IDC_BUTTON17, m_Auto_CloseWindow0);
	DDX_Control(pDX, IDC_BUTTON18, m_Auto_CloseWindow1);
	DDX_Control(pDX, IDC_BUTTON25, m_cx_wpp);
	DDX_Control(pDX, IDC_BUTTON26, m_cx_3ah);
	DDX_Control(pDX, IDC_BUTTON19, m_But_SNSet);
	DDX_Text(pDX,IDC_EDIT15,m_Edit_SN_Year);
	DDV_MaxChars(pDX,m_Edit_SN_Year,4);
	DDX_Text(pDX,IDC_EDIT16,m_Edit_SN_Month);
	DDV_MaxChars(pDX,m_Edit_SN_Month,2);
	DDX_Text(pDX,IDC_EDIT17,m_Edit_SN_Day);
	DDV_MaxChars(pDX,m_Edit_SN_Day,2);

	DDX_Text(pDX,IDC_EDIT14,m_Edit_SN);
	DDV_MaxChars(pDX,m_Edit_SN,4);
	DDX_Text(pDX,IDC_EDIT13,m_Edit_LN);
	DDV_MaxChars(pDX,m_Edit_LN,1);


	/*DDX_Control(pDX, IDC_LIST1, m_ListBox);*/
	// 	DDX_Control(pDX, IDC_BUTTON21, m_CLEAR);
	// 	DDX_Control(pDX, IDC_BUTTON22, m_SAVE);
	// 	DDX_Control(pDX, IDC_BUTTON23, m_Mode0);
	// 	DDX_Control(pDX, IDC_BUTTON24, m_Mode1);
	// 	DDX_Control(pDX, IDC_BUTTON27, m_Mode2);
	DDX_Control(pDX, IDC_BUTTON28, m_ESC_Config0);
	DDX_Control(pDX, IDC_BUTTON29, m_ESC_Config1);
	DDX_Control(pDX, IDC_COMBO1, m_Config);
	//	DDX_Control(pDX, IDC_BUTTON30, m_Mode3);
	DDX_Control(pDX, IDC_CHECKADD, m_NumAdd);

	DDX_Control(pDX, IDC_BUTTON30, m_tcu_config0);
	DDX_Control(pDX, IDC_BUTTON31, m_tcu_config1);
	DDX_Control(pDX, IDC_BUTTON32, m_acu_config0);
	DDX_Control(pDX, IDC_BUTTON33, m_acu_config1);
	DDX_Control(pDX, IDC_BUTTON34, m_esc_config0);
	DDX_Control(pDX, IDC_BUTTON35, m_esc_config1);
	DDX_Control(pDX, IDC_BUTTON36, m_eps_config0);
	DDX_Control(pDX, IDC_BUTTON37, m_eps_config1);
	DDX_Control(pDX, IDC_BUTTON22, m_ems_config0);
	DDX_Control(pDX, IDC_BUTTON23, m_ems_config1);
	DDX_Control(pDX, IDC_BUTTON24, m_ipk_config0);
	DDX_Control(pDX, IDC_BUTTON27, m_ipk_config1);
	DDX_Control(pDX, IDC_BUTTON38, m_mmi_config0);
	DDX_Control(pDX, IDC_BUTTON39, m_mmi_config1);
	DDX_Control(pDX, IDC_BUTTON40, m_ac_config0);
	DDX_Control(pDX, IDC_BUTTON41, m_ac_config1);
	DDX_Control(pDX, IDC_BUTTON42, m_sas_config0);
	DDX_Control(pDX, IDC_BUTTON43, m_sas_config1);
	DDX_Control(pDX, IDC_BUTTON44, m_abs_config0);
	DDX_Control(pDX, IDC_BUTTON45, m_abs_config1);
	DDX_Control(pDX, IDC_BUTTON46, m_epb_config0);
	DDX_Control(pDX, IDC_BUTTON47, m_epb_config1);
	DDX_Control(pDX, IDC_BUTTON48, m_cx_FE_3AH);
	DDX_Control(pDX, IDC_BUTTON50, m_rooffj0);
	DDX_Control(pDX, IDC_BUTTON51, m_rooffj1);
	DDX_Control(pDX, IDC_BUTTON52, m_gw);
	DDX_Control(pDX, IDC_BUTTON53, m_gl);
	DDX_Control(pDX, IDC_BUTTON54, m_xc);
	DDX_Control(pDX, IDC_BUTTON55, m_xc_w);
	DDX_Control(pDX, IDC_BUTTON56, m_kt0);
	DDX_Control(pDX, IDC_BUTTON57, m_kt1);
	DDX_Control(pDX, IDC_BUTTON49, m_Carmera0);
	DDX_Control(pDX, IDC_BUTTON58, m_Carmera1);
	DDX_Control(pDX, IDC_BUTTON59, m_Console_Ambient_Light0);
	DDX_Control(pDX, IDC_BUTTON60, m_Console_Ambient_Light1);
	DDX_Control(pDX, IDC_BUTTON61, gw_config0);
	DDX_Control(pDX, IDC_BUTTON62, gw_config1);
}

BEGIN_MESSAGE_MAP(CCANFactorySetDlg, /*CDialogEx*/CBkDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON6, &CCANFactorySetDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON11, &CCANFactorySetDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CCANFactorySetDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CCANFactorySetDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CCANFactorySetDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CCANFactorySetDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CCANFactorySetDlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CCANFactorySetDlg::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CCANFactorySetDlg::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON25, &CCANFactorySetDlg::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON26, &CCANFactorySetDlg::OnBnClickedButton26)
	ON_STN_CLICKED(IDC_STATIC31, &CCANFactorySetDlg::OnStnClickedStatic31)
/*	ON_BN_CLICKED(IDC_BUTTON22, &CCANFactorySetDlg::OnBnClickedButton22)*/
/*	ON_BN_CLICKED(IDC_BUTTON21, &CCANFactorySetDlg::OnBnClickedButton21)*/
// 	ON_BN_CLICKED(IDC_BUTTON23, &CCANFactorySetDlg::OnBnClickedButton23)
// 	ON_BN_CLICKED(IDC_BUTTON24, &CCANFactorySetDlg::OnBnClickedButton24)
// 	ON_BN_CLICKED(IDC_BUTTON27, &CCANFactorySetDlg::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BUTTON19, &CCANFactorySetDlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON4, &CCANFactorySetDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON8, &CCANFactorySetDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CCANFactorySetDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CCANFactorySetDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON3, &CCANFactorySetDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CCANFactorySetDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CCANFactorySetDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON20, &CCANFactorySetDlg::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON28, &CCANFactorySetDlg::OnBnClickedButton28)
	ON_BN_CLICKED(IDC_BUTTON29, &CCANFactorySetDlg::OnBnClickedButton29)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCANFactorySetDlg::OnCbnSelchangeCombo1)
/*	ON_BN_CLICKED(IDC_BUTTON30, &CCANFactorySetDlg::OnBnClickedButton30)*/
ON_WM_CHAR()
ON_EN_CHANGE(IDC_EDIT1, &CCANFactorySetDlg::OnEnChangeEdit1)
ON_BN_CLICKED(IDC_BUTTON30, &CCANFactorySetDlg::OnBnClickedButton30)
ON_BN_CLICKED(IDC_BUTTON31, &CCANFactorySetDlg::OnBnClickedButton31)
ON_BN_CLICKED(IDC_BUTTON32, &CCANFactorySetDlg::OnBnClickedButton32)
ON_BN_CLICKED(IDC_BUTTON33, &CCANFactorySetDlg::OnBnClickedButton33)
ON_BN_CLICKED(IDC_BUTTON34, &CCANFactorySetDlg::OnBnClickedButton34)
ON_BN_CLICKED(IDC_BUTTON35, &CCANFactorySetDlg::OnBnClickedButton35)
ON_BN_CLICKED(IDC_BUTTON36, &CCANFactorySetDlg::OnBnClickedButton36)
ON_BN_CLICKED(IDC_BUTTON37, &CCANFactorySetDlg::OnBnClickedButton37)
ON_BN_CLICKED(IDC_BUTTON22, &CCANFactorySetDlg::OnBnClickedButton22)
ON_BN_CLICKED(IDC_BUTTON23, &CCANFactorySetDlg::OnBnClickedButton23)
ON_BN_CLICKED(IDC_BUTTON24, &CCANFactorySetDlg::OnBnClickedButton24)
ON_BN_CLICKED(IDC_BUTTON27, &CCANFactorySetDlg::OnBnClickedButton27)
ON_BN_CLICKED(IDC_BUTTON38, &CCANFactorySetDlg::OnBnClickedButton38)
ON_BN_CLICKED(IDC_BUTTON39, &CCANFactorySetDlg::OnBnClickedButton39)
ON_BN_CLICKED(IDC_BUTTON40, &CCANFactorySetDlg::OnBnClickedButton40)
ON_BN_CLICKED(IDC_BUTTON41, &CCANFactorySetDlg::OnBnClickedButton41)
ON_BN_CLICKED(IDC_BUTTON42, &CCANFactorySetDlg::OnBnClickedButton42)
ON_BN_CLICKED(IDC_BUTTON43, &CCANFactorySetDlg::OnBnClickedButton43)
ON_BN_CLICKED(IDC_BUTTON44, &CCANFactorySetDlg::OnBnClickedButton44)
ON_BN_CLICKED(IDC_BUTTON45, &CCANFactorySetDlg::OnBnClickedButton45)
ON_BN_CLICKED(IDC_BUTTON46, &CCANFactorySetDlg::OnBnClickedButton46)
ON_BN_CLICKED(IDC_BUTTON47, &CCANFactorySetDlg::OnBnClickedButton47)
ON_BN_CLICKED(IDC_BUTTON48, &CCANFactorySetDlg::OnBnClickedButton48)
ON_BN_CLICKED(IDC_BUTTON50, &CCANFactorySetDlg::OnBnClickedButton50)
ON_BN_CLICKED(IDC_BUTTON51, &CCANFactorySetDlg::OnBnClickedButton51)
ON_BN_CLICKED(IDC_BUTTON52, &CCANFactorySetDlg::OnBnClickedButton52)
ON_BN_CLICKED(IDC_BUTTON53, &CCANFactorySetDlg::OnBnClickedButton53)
ON_BN_CLICKED(IDC_BUTTON54, &CCANFactorySetDlg::OnBnClickedButton54)
ON_BN_CLICKED(IDC_BUTTON55, &CCANFactorySetDlg::OnBnClickedButton55)
ON_BN_CLICKED(IDC_BUTTON56, &CCANFactorySetDlg::OnBnClickedButton56)
ON_BN_CLICKED(IDC_BUTTON57, &CCANFactorySetDlg::OnBnClickedButton57)
ON_BN_CLICKED(IDC_BUTTON49, &CCANFactorySetDlg::OnBnClickedButton49)
ON_BN_CLICKED(IDC_BUTTON58, &CCANFactorySetDlg::OnBnClickedButton58)
ON_BN_CLICKED(IDC_BUTTON59, &CCANFactorySetDlg::OnBnClickedButton59)
ON_BN_CLICKED(IDC_BUTTON60, &CCANFactorySetDlg::OnBnClickedButton60)
ON_BN_CLICKED(IDC_BUTTON61, &CCANFactorySetDlg::OnBnClickedButton61)
ON_BN_CLICKED(IDC_BUTTON62, &CCANFactorySetDlg::OnBnClickedButton62)
END_MESSAGE_MAP()


// CCANFactorySetDlg 消息处理程序

BOOL CCANFactorySetDlg::OnInitDialog()
{
	/*CDialogEx*/CBkDialog::OnInitDialog();

	SetBitmap(IDB_BDBK);
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	/*
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	*/
	// TODO: 在此添加额外的初始化代码
/*	CCANSend::Single_Frame(0,0,0,NULL,NULL,0);*/

//	CCANSend::Open_VCIPort();
	//this->SetFocus();

	

// 	VCI_CAN_OBJ SendInit;
// 	SendInit.TimeStamp = 0;
// 	SendInit.TimeFlag = 0;
// 	SendInit.SendType = 1;
// 	SendInit.RemoteFlag = 0;//是否是远程帧
// 	SendInit.ExternFlag = 0;//是否是扩展帧
// 	SendInit.DataLen = 8;
// 	SendInit.ID = 0x00;
// 	memset(SendInit.Data,0x00,8);
// 	if (VCI_Transmit(VCI_USBCAN1,0,0,&SendInit,1)  == STATUS_OK)
// 	{	/*OK*/	}
// 	else
// 	{/*Fail*//*ErrorFlg = 0x01;*/}
	
	Init_Control();
	QHInit();
	TuBiaoys();
	Dlg_Error = new CError;
	Dlg_Error->Create(IDD_ERROR,this);
	Dlg_Error->CenterWindow();
	Dlg_PASS = new CPASS;
	Dlg_PASS->Create(IDD_PASS,this);
	Dlg_PASS->CenterWindow();
	//GetEditText();
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCANFactorySetDlg::Init_Control()
{
	UINT i;

	m_kt0.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_kt1.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_But_CarName.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_But_CarVIN.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_But_DiagnoseSN.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_Set.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_Read.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_But_ECU_ConfigDate.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_But_ECU_MakeDate.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_But_ECU_SWDate.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_NetworkConfig0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_NetworkConfig1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_TPMS0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_TPMS1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ESC_Config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ESC_Config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_BCM_Rmirror0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_BCM_Rmirror1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_Auto_CloseWindow1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_Auto_CloseWindow0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_But_SNSet.SetShade(CShadeButtonST::SHS_HARDBUMP);
	m_tcu_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_tcu_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_acu_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_acu_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_esc_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_esc_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_eps_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_eps_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ems_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ems_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ipk_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ipk_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_mmi_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_mmi_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ac_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_ac_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_sas_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_sas_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_abs_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_abs_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_epb_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_epb_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_rooffj1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_rooffj0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_gl.SetShade(CShadeButtonST::SHS_HSHADE);
	m_gw.SetShade(CShadeButtonST::SHS_HSHADE);
	m_xc.SetShade(CShadeButtonST::SHS_HSHADE);
	m_xc_w.SetShade(CShadeButtonST::SHS_HSHADE);
	m_cx_wpp.SetShade(CShadeButtonST::SHS_HSHADE);
	m_cx_3ah.SetShade(CShadeButtonST::SHS_HSHADE);
	m_cx_FE_3AH.SetShade(CShadeButtonST::SHS_HSHADE);
	m_Carmera0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_Carmera1.SetShade(CShadeButtonST::SHS_HSHADE);
	m_Console_Ambient_Light0.SetShade(CShadeButtonST::SHS_HSHADE);
	m_Console_Ambient_Light1.SetShade(CShadeButtonST::SHS_HSHADE);
	gw_config1.SetShade(CShadeButtonST::SHS_HSHADE);
	gw_config0.SetShade(CShadeButtonST::SHS_HSHADE);
	GetDlgItem(IDC_EDIT1)->SetWindowText(L"FE_3AH");
	GetDlgItem(IDC_EDIT2)->SetWindowText(L"000000000000055AA");
	GetDlgItem(IDC_EDIT3)->SetWindowText(L"00000000000055AA");
	GetDlgItem(IDC_EDIT13)->SetWindowText(L"0");
	GetDlgItem(IDC_EDIT14)->SetWindowText(L"0000");

   Year.Format(_T("%02d"),t.GetYear());
   Months.Format(_T("%02d"),t.GetMonth());
   Date.Format(_T("%02d"),t.GetDay());
   GetDlgItem(IDC_EDIT4)->SetWindowText(Year);
   GetDlgItem(IDC_EDIT5)->SetWindowText(Months);
   GetDlgItem(IDC_EDIT6)->SetWindowText(Date);
   GetDlgItem(IDC_EDIT7)->SetWindowText(Year);
   GetDlgItem(IDC_EDIT8)->SetWindowText(Months);
   GetDlgItem(IDC_EDIT9)->SetWindowText(Date);
   GetDlgItem(IDC_EDIT10)->SetWindowText(Year);
   GetDlgItem(IDC_EDIT11)->SetWindowText(Months);
   GetDlgItem(IDC_EDIT12)->SetWindowText(Date);
   GetDlgItem(IDC_EDIT15)->SetWindowText(Year);
   GetDlgItem(IDC_EDIT16)->SetWindowText(Months);
   GetDlgItem(IDC_EDIT17)->SetWindowText(Date);

   // 高低配设置
   m_Config.SetCurSel(0);
/*SetCueBanner(_T(""), TRUE);*/
   m_Edit_month2.SetCueBanner(L"",TRUE);

   static CFont font,sont,zont;
   font.DeleteObject();
   font.CreatePointFont(180,_T("楷体"));
   sont.DeleteObject();
   sont.CreatePointFont(110,_T("楷体"));
   zont.DeleteObject();
   zont.CreatePointFont(160,_T("楷体"));

   for (i = IDC_STATIC4;i <=IDC_STATIC6 ;i++)
   {
	    GetDlgItem(i)->SetFont(&font);
   }
   for (i = IDC_STATIC24;i <=IDC_STATIC31 ;i++)
   {
	   GetDlgItem(i)->SetFont(&font);
   }
//    GetDlgItem(IDC_STATIC21)->SetFont(&font);
//    GetDlgItem(IDC_STATIC23)->SetFont(&font);
   m_But_CarName.SetFont(&sont);
   m_But_CarVIN.SetFont(&sont);
   m_But_DiagnoseSN.SetFont(&sont);
   m_But_ECU_ConfigDate.SetFont(&sont);
   m_But_ECU_MakeDate.SetFont(&sont);
   m_But_ECU_SWDate.SetFont(&sont);
   m_But_SNSet.SetFont(&sont);
   GetDlgItem(IDC_EDIT13)->SetFont(&sont);
   GetDlgItem(IDC_EDIT14)->SetFont(&sont);
   GetDlgItem(IDC_STATIC32)->SetFont(&sont);
   GetDlgItem(IDC_STATIC33)->SetFont(&sont);
   GetDlgItem(IDC_STATIC36)->SetFont(&sont);
   GetDlgItem(IDC_STATIC37)->SetFont(&sont);

   GetDlgItem(IDC_EDIT18)->SetFont(&zont);
   GetDlgItem(IDC_EDIT19)->SetFont(&zont);
   GetDlgItem(IDC_EDIT20)->SetFont(&zont);
   GetDlgItem(IDC_EDIT21)->SetFont(&zont);

   m_Edit_CarName.SetFont(&zont);
   m_Edit_CarVIN.SetFont(&zont);
   m_Edit_DiagnoseSN.SetFont(&zont);
   m_Edit_Year0.SetFont(&font);
   m_Edit_Year1.SetFont(&font);
   m_Edit_Year2.SetFont(&font);
   m_Edit_month0.SetFont(&font);
   m_Edit_month1.SetFont(&font);
   m_Edit_month2.SetFont(&font);
   m_Edit_Day0.SetFont(&font);
   m_Edit_Day1.SetFont(&font);
   m_Edit_Day2.SetFont(&font);
  
}

void CCANFactorySetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
// 	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
// 	{
// 		CAboutDlg dlgAbout;
// 		dlgAbout.DoModal();
// 	}
// 	else
	{
		/*CDialogEx*/CBkDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCANFactorySetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/*CDialogEx*/CBkDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCANFactorySetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CCANFactorySetDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	delete[] PartNumberBuf;
	delete[] SystemSupplierIdBuf;

//	KillTimer(TxMsgTimer);
   
	/*CDialogEx*/CBkDialog::OnClose();
}


void CCANFactorySetDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

// 	switch (nIDEvent)
// 	{
// 	case TxMsgTimer :
// 		CCANSend::Transmit();
// 		if (ShowTxFlg == 1)
// 		{
// 			ShowTxFlg = 0;
// 			DisTxData(TxShowData,11,8);
// 		}
// 		break;
// 	case RxMsgTimer:
// 		CCANSend::Deal_ReceiveFrameMsg();
// 		if (ShowRxFlg == 1)
// 		{
// 			ShowRxFlg = 0;
// 			DisTxData(RxShowData,10,8);
// 		}
// 		break;
// 	case 3 :
// 		DataAnalyze();
// 		break;
// 	case 4:
// 		CCANSend::Single_Frame(TestID,0x22,0xA0,NULL,NULL,0);
// 		KillTimer(4);
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	__super::OnTimer(nIDEvent);
}

void CCANFactorySetDlg::DataAnalyze()
{

	OverTime ++ ;

	if (TestFlg != 0)
	{

		if (TestFlg == 01)
		{
			BYTE Key[4];
			for(UINT i = 0;i<4;i++)
			{
				Key[i] = KeyInit[i]^Seed[i];
			}
			CCANSend::Single_Frame(REQUEST_CAN_ID,0x10,0x02,NULL,Key,sizeof(Key));
		}else if(TestFlg == 0x021){
		//	KillTimer(3);
			DataAnalyzeFlg = 0;
			CString Sum;
			UINT iSum;
			if (NumAdd)
			{
				GetDlgItem(IDC_EDIT14)->GetWindowText(Sum);
				iSum = _ttoi(Sum);
				iSum++;
				Sum.Format(L"%05d",iSum);
				GetDlgItem(IDC_EDIT14)->SetWindowText(Sum);
			}

			MessageBox(L"进入测试模式成功！",L"提示");
			TestFlg1=1;
		}else if (TestFlg == 0x022){
			//KillTimer(3);
			DataAnalyzeFlg = 0;
			MessageBox(L"当前已在测试模式！",L"提示");
			TestFlg1=1;
		}
		else if (TestFlg == 0x03){
		//	KillTimer(3);
			DataAnalyzeFlg = 0;
			MessageBox(L"退出测试模式成功！",L"提示");
			TestFlg1=0;
		}
		TestFlg = 0;
		OverTime = 0;
		return;
	}
	if (SetFlg != 0x0)
	{
		// 避免重发的模式，只对只对工厂模式有效
		if (IS_MODEL_FIXER_ENABLED()) {
			// 下面代码避免发送重帧
			if (fixer.IsAlreadSet(SetFlg)) {
				SetFlg = 0;
				return;
			}
			fixer.AlreadySet(SetFlg);
		}


		if (SetFlg == 0xA0){
			TestMode(0xA0,NULL);//测试模式下，读车辆名称
			CCANSend::Multi_Frame(TestID,0x2E,0xA1,NULL,CarVINData,sizeof(CarVINData)); //车辆VIN码设置
		}else if (SetFlg == 0xA1){
			TestMode(0xA1,NULL);//测试模式下，读车辆vin码
			CCANSend::Multi_Frame(TestID,0x2E,0xA2,NULL,DiagnoseSNData,sizeof(DiagnoseSNData));//
		}else if (SetFlg == 0xA2){
			TestMode(0xA2,NULL);
			CCANSend::Single_Frame(TestID,0x2E,0xA4,NULL,ECUMakeDayData,sizeof(ECUMakeDayData));
		}else if (SetFlg == 0xA3){
			//
		}else if (SetFlg == 0xA4){
			TestMode(0xA4,NULL);
			CCANSend::Single_Frame(TestID,0x2E,0xA5,NULL,ECUSWDayData,sizeof(ECUSWDayData));
		}else if (SetFlg == 0xA5){
			TestMode(0xA5,NULL);
			CCANSend::Single_Frame(TestID,0x2E,0xA6,NULL,ECUConfigDayData,sizeof(ECUConfigDayData));
		}else if (SetFlg == 0xA6){
			TestMode(0xA6,NULL);
			//CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
			CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		}
		else if (SetFlg == 0xA7){
			//
		}
		//else if (SetFlg == 0xA8){
		//	TestMode(0xA8,NULL);
		//	CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//}
		else if (SetFlg == 0xA9){
			TestMode(0xA9,NULL);
			QAMode(0xA8,NULL);
			CCANSend::Multi_Frame(TestID,0x2E,0x84,NULL,SNData,sizeof(SNData));
		}
		else if (SetFlg == 0x84){
			TestMode(0x84,NULL);
           // SetTimer(4,1000,NULL);
			Time4 = 1;
			//KillTimer(3);
		}
		SetFlg = 0;
		OverTime = 0;
		return ;
	}
	if (ErrorFlg != 0x00)
	{
	//	KillTimer(3);
		DataAnalyzeFlg = 0;
		if (ErrorFlg == 0x01)
		{
		//	MessageBox(L"数据发送失败，请检查设备是否连接",L"提醒");
		}else if (ErrorFlg == 0x11){
			MessageBox(L"服务ID不支持！",L"提示");
		   // GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x12){
			MessageBox(L"DID不支持！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x13){
			MessageBox(L"报文长度或数据格式错误！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x31){
			MessageBox(L"请求设置的数值超出范围！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x32){
			MessageBox(L"请求数值无效！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x35){
			MessageBox(L"无效的解锁钥匙！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x40){
			MessageBox(L"DID Command 不支持！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x41){
			MessageBox(L"请求设置的功能DVD未完成！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x80){
			MessageBox(L"不在测试模式！",L"提示");
			TestFlg1=0;
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x71){
			MessageBox(L"报文类型错误！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x72){
			MessageBox(L"报文SN错误！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x73){
			if (IS_MODEL_FIXER_ENABLED() && fixer.Restore()) {
				DataAnalyzeFlg = 1;
//				ErrorFlg1=0;
			} else {
				fixer.Log();
				MessageBox(L"报文时间超时！",L"提示");
			}
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x74){
			MessageBox(L"接收报文长度错误！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}else if (ErrorFlg == 0x75){
			MessageBox(L"填充字节错误！",L"提示");
			//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
		}
		//ErrorFlg = 0;
		if (FACTORY_TOOL_MODEL != ToolModel)
		{
			ErrorFlg = 0;
		}

		if (trigger_by_set_key_)
		{
			ErrorFlg = 0;
			trigger_by_set_key_ = FALSE;
		}

		OverTime = 0;
		return ;
	}

	if (ReadFlg!=0)
	{
		if (IS_MODEL_FIXER_ENABLED()) {
			if (fixer.IsAlreadyRead(ReadFlg))
			{
				ReadFlg = 0;
				return;
			}
			fixer.AlreadyRead(ReadFlg);
		}

		if (ReadFlg == 0xA0){
			if(ToolModel != 1){
				
				if (ToolModel == 2){
				//	KillTimer(3);
					DataAnalyzeFlg = 0;
					DataCmp(0xA0);
				}else if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetCarName();
						CCANSend::Single_Frame(TestID,0x22,0xA1,NULL,NULL,0);
					}else{
						DataCmp(0xA0);
						CCANSend::Single_Frame(TestID,0x22,0xA1,NULL,NULL,0);
					}

				}else {
					DataCmp(0xA0);
					CCANSend::Single_Frame(TestID,0x22,0xA1,NULL,NULL,0);
				}
			}else {
				SetCarName();
				CCANSend::Single_Frame(TestID,0x22,0xA1,NULL,NULL,0);
			}
			
		}else if (ReadFlg == 0xA1){
			if(ToolModel != 1){
				
				if (ToolModel == 2){
				//	KillTimer(3);
					DataAnalyzeFlg = 0;
					DataCmp(0xA1);
				}else if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetCarVIN();
						CCANSend::Single_Frame(TestID,0x22,0xA2,NULL,NULL,0);
					}else{
						DataCmp(0xA1);
						CCANSend::Single_Frame(TestID,0x22,0xA2,NULL,NULL,0);
					}

				}else {
					DataCmp(0xA1);
					CCANSend::Single_Frame(TestID,0x22,0xA2,NULL,NULL,0);
				}
			}else {
				SetCarVIN();
				CCANSend::Single_Frame(TestID,0x22,0xA2,NULL,NULL,0);
			}
		}else if (ReadFlg == 0xA2){
			if(ToolModel != 1){

				if (ToolModel == 2){
				//	KillTimer(3);
					DataAnalyzeFlg = 0;
					DataCmp(0xA2);
				}else if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetDiagnoseSN();
						CCANSend::Single_Frame(TestID,0x22,0xA4,NULL,NULL,0);
					}else{
						DataCmp(0xA2);
						CCANSend::Single_Frame(TestID,0x22,0xA4,NULL,NULL,0);
					}

				}else {
					CCANSend::Single_Frame(TestID,0x22,0xA4,NULL,NULL,0);
				}
			}else {
				SetDiagnoseSN();
				CCANSend::Single_Frame(TestID,0x22,0xA4,NULL,NULL,0);
			}
		}else if (ReadFlg == 0xA3){
			//
		}else if (ReadFlg == 0xA4){
			if(ToolModel != 1){
				
				if (ToolModel == 2){
				//	KillTimer(3);
					DataAnalyzeFlg = 0;
					DataCmp(0xA4);
				}else if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetECUMakeDate();
						CCANSend::Single_Frame(TestID,0x22,0xA5,NULL,NULL,0);
					}else{
						DataCmp(0xA4);
						CCANSend::Single_Frame(TestID,0x22,0xA5,NULL,NULL,0);
					}

				}else {
					DataCmp(0xA4);
					CCANSend::Single_Frame(TestID,0x22,0xA5,NULL,NULL,0);
				}
			}else {
				SetECUMakeDate();
				CCANSend::Single_Frame(TestID,0x22,0xA5,NULL,NULL,0);
			}			
			
		}else if (ReadFlg == 0xA5){
			if(ToolModel != 1){
				

				if (ToolModel == 2){
				//	KillTimer(3);
					DataAnalyzeFlg = 0;
					DataCmp(0xA5);
				}else if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetECUSWDate();
						CCANSend::Single_Frame(TestID,0x22,0xA6,NULL,NULL,0);
					}else{
						DataCmp(0xA5);
						CCANSend::Single_Frame(TestID,0x22,0xA6,NULL,NULL,0);
					}

				}else {
					
					CCANSend::Single_Frame(TestID,0x22,0xA6,NULL,NULL,0);
				}

			}else {
				SetECUSWDate();
				CCANSend::Single_Frame(TestID,0x22,0xA6,NULL,NULL,0);
			}		
			
			
		}else if (ReadFlg == 0xA6){
			if(ToolModel != 1){
				

				if (ToolModel == 2){
				//	KillTimer(3);
					DataAnalyzeFlg = 0;
					DataCmp(0xA6);
				}else if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetECUConfigDate();
						CCANSend::Single_Frame(TestID,0x22,0xA8,NULL,NULL,0);
					
					}else{
						DataCmp(0xA6);
						CCANSend::Single_Frame(TestID,0x22,0xA8,NULL,NULL,0);
						
					}

				}else {
					DataCmp(0xA6);
					CCANSend::Single_Frame(TestID,0x22,0xA8,NULL,NULL,0);
				}

			}else {
				SetECUConfigDate();
				CCANSend::Single_Frame(TestID,0x22,0xA8,NULL,NULL,0);
				
			}	
			
		}else if (ReadFlg == 0xA7){
			//
		}
		
		//else if (ReadFlg == 0xA8){
		//	if(ToolModel != 1){
		//		
		//		if (ToolModel == 2){
		//		//	KillTimer(3);
		//			DataAnalyzeFlg = 0;
		//			DataCmp(0xA8);
		//		}else if (ToolModel == 3)
		//		{
		//			if (ReadMode == 1)
		//			{
		//				SetMMIConfig();
		//				CCANSend::Single_Frame(TestID,0x22,0xA9,NULL,NULL,0);
		//			}else{
		//				DataCmp(0xA8);
		//				CCANSend::Single_Frame(TestID,0x22,0xA9,NULL,NULL,0);
		//			}

		//		}else {
		//			DataCmp(0xA8);
		//			CCANSend::Single_Frame(TestID,0x22,0xA9,NULL,NULL,0);
		//		}
		//	}else {
		//		SetMMIConfig();
		//		CCANSend::Single_Frame(TestID,0x22,0xA9,NULL,NULL,0);
		//	}
		//	
		//	
		//}
		else if (ReadFlg == 0xA9){
			if(ToolModel != 1){
				
				if (ToolModel == 2){
				//	KillTimer(3);
					DataAnalyzeFlg = 0;
					DataCmp(0xA9);
				}else if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetNetworkConfig();
						CCANSend::Single_Frame(TestID,0x22,0x84,NULL,NULL,0);
					}else {

					//	KillTimer(3);
						DataAnalyzeFlg = 0;
						DataCmp(0xA9);
						if (ErrorStrFlg == 1)
						{
							ErrorStrFlg = 0;						
						    Dlg_Error->ShowWindow(SW_SHOW);
						 	Dlg_Error->SetErrorMsg(FactorySetDlg,ErrorStr);
						 	this->EnableWindow(FALSE);
						 	//ErrorStr.Empty();
						 }else {
						 	Dlg_PASS->ShowWindow(SW_SHOW);
						 	Dlg_PASS->SetPASSMSG(FactorySetDlg,L"设置成功");
						 	this->EnableWindow(FALSE);
						 }
					}
				}
				else {
					DataCmp(0xA9);
					CCANSend::Single_Frame(TestID,0x22,0x84,NULL,NULL,0);
				}
			}else {
				SetNetworkConfig();
				CCANSend::Single_Frame(TestID,0x22,0x84,NULL,NULL,0);
			}
		
			
		}
		else if (ReadFlg == 0x84){
			if(ToolModel != 1){
				
				if (ToolModel == 2){
					DataCmp(0x84);
				//	KillTimer(3);
					DataAnalyzeFlg = 0;
				}else if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetSN();
						CCANSend::Single_Frame(TestID,0x22,0x82,0x08,NULL,0);
					}else {
					//	KillTimer(3);
						DataAnalyzeFlg = 0;
					}
				}else {
					DataCmp(0x84);
					CCANSend::Single_Frame(TestID,0x22,0x82,0x08,NULL,0);
				}
			
			}else {
				SetSN();
				CCANSend::Single_Frame(TestID,0x22,0x82,0x08,NULL,0);
			}
			
		}else if (ReadFlg == 0x82)
		{
			if (HWVersionFlg = 0x08)
			{
				if (ToolModel != 1)
				{

					if (ToolModel == 3)
					{
						if (ReadMode == 1)
						{
							SetECUHWVersion();
						}
					}else {
						DataCmp(0x82);
					}

				}else {
					SetECUHWVersion();
				}
				CCANSend::Single_Frame(TestID,0x22,0x81,0x08,NULL,0);
			}
		}else if (ReadFlg == 0x81)
		{
			if(SWVersionFlg == 0x08)
			{
				if (ToolModel != 1)
				{


					if (ToolModel == 3)
					{
						if (ReadMode == 1)
						{
							SetECUSWVersion();
						}
					}else {
						DataCmp(0x81);
					}

				}else {
					SetECUSWVersion();
				}
				CCANSend::Single_Frame(TestID,0x22,0xAA,NULL,NULL,0);
			}

		}else if (ReadFlg == 0xAA)
		{
			if (ToolModel != 1)
			{

				if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetPartNumber();
					}
				}else {
					DataCmp(0xAA);
				}

				

			}else {
				SetPartNumber();
			}
			CCANSend::Single_Frame(TestID,0x22,0xAB,NULL,NULL,0);
			

		}else if (ReadFlg == 0xAB)
		{
			if (ToolModel != 1)
			{

				if (ToolModel == 3)
				{
					if (ReadMode == 1)
					{
						SetSystemSupplierId();
						
					}
				}else {
					DataCmp(0xAB);
				}
				

			}else {
				SetSystemSupplierId();
			}
		//	KillTimer(3);
			DataAnalyzeFlg = 0;
			
			if (ReadMode == 0 )
			{
				if (ErrorStrFlg == 1)
				{
					ErrorStrFlg = 0;
					//		MessageBox(ErrorStr,L"提示!");
					Dlg_Error->ShowWindow(SW_SHOW);
					Dlg_Error->SetErrorMsg(FactorySetDlg,ErrorStr);
					this->EnableWindow(FALSE);
					//ErrorStr.Empty();
				}else {
					Dlg_PASS->ShowWindow(SW_SHOW);
					Dlg_PASS->SetPASSMSG(FactorySetDlg,L"设置成功");
					this->EnableWindow(FALSE);
				}

				if (trigger_by_set_key_) {
					trigger_by_set_key_ = FALSE;
					ErrorStr = _T("");
					ERRORSTR = _T("");
				}
			}

			ReadMode = 0;

			if (IS_MODEL_FIXER_ENABLED()) {
				fixer.EndSet();
			}

			if (trigger_by_set_key_) {
				trigger_by_set_key_ = FALSE;
			}
		}

		ReadFlg = 0;
		OverTime = 0;
		return ;
	}
//	OverTime -= 2;
	if (OverTime == 200)
	{
		if (IS_MODEL_FIXER_ENABLED() && fixer.Restore()) {
			OverTime = 0;
			return;
		}

	//	KillTimer(3);
		DataAnalyzeFlg = 0;
		/*OverTime = 0;*/
		MessageBox(L"无应答信号，设置超时",L"提醒");
	//	GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
	}
}

UINT CCANFactorySetDlg::Compare(BYTE *dat0,BYTE *dat1,UINT Len)
{
	for (UINT i = 0;i<Len;i++)
	{
		if (*dat0 != *dat1)
		{
			return 0;
		}
		dat0++;
		dat1++;
	}
	return 1;
}

void CCANFactorySetDlg::DataCmp(UINT Num)
{
	CString Str,Str1;

	if (Num == 0xA0){
		if (Compare(RxCarName,CarNameData,8)){
			//Show_Box(L"车辆名正确");
			SendShowData(L"车辆名正确");
		}else {
			/*Show_Box(L"车辆名错误");*/
			SendShowData(L"车辆名错误");

         ErrorStr += L"车辆名错误\r\n";
		 ErrorStrFlg = 1;
		}
	}else if (Num == 0xA1){
		if (Compare(RxCarVIN,CarVINData,17)){
			/*Show_Box(L"车辆VIN码正确");*/
			SendShowData(L"车辆VIN码正确");
		}else {
			SendShowData(L"车辆VIN码错误");
			 ErrorStr += L"车辆VIN码错误\r\n";
		   ErrorStrFlg = 1;
		}
	}else if (Num == 0xA2){
		if (Compare(RxDiagnoseSN,DiagnoseSNData,16)){
			SendShowData(L"诊断仪序列号正确");
		}else {
			SendShowData(L"诊断仪序列号错误");
			ErrorStr += L"诊断仪序列号错误\r\n";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0xA3){
		//
	}else if (Num == 0xA4){
		if (Compare(RxECUMakeDate,ECUMakeDayData,4)){
			SendShowData(L"ECU制造日期正确");
		}else {
			SendShowData(L"ECU制造日期错误");
			ErrorStr += L"ECU制造日期错误\r\n";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0xA5){
		if (Compare(RxECUSWDate,ECUSWDayData,4)){
			SendShowData(L"ECU软件刷新日期正确");
		}else {
			SendShowData(L"ECU软件刷新日期错误");
			ErrorStr += L"ECU软件刷新日期错误\r\n";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0xA6){
		if (Compare(RxECUConfigDate,ECUConfigDayData,4)){
			SendShowData(L"ECU装配日期正确");
		}else {
			SendShowData(L"ECU装配日期错误");
			ErrorStr += L"ECU装配日期错误\r\n";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0xA7){
		//
	}else if (Num == 0xA8){
		if (Compare(RxMMIConfig,MMIConfigData,16)){
			SendShowData(L"MMI配置功能正确");
		}else {
			SendShowData(L"MMI配置功能错误");
			ErrorStr += L"MMI配置功能错误\r\n";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0xA9){
		if (Compare(RxNetWorkConfig,NetWorkConfigData,8)){
			SendShowData(L"网络架构配置正确");
		}else {
			SendShowData(L"网络架构配置错误");
			ErrorStr += L"网络架构配置错误\r\n";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0x84){
		if (Compare(RxDVDSN,SNData,24)){
			SendShowData(L"机器序列号正确");
		}else {
			SendShowData(L"机器序列号错误");
			ErrorStr += L"机器序列号错误\r\n";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0x82){
		if (Compare(ECUHWVersion->data,HWVersion,ECUHWVersion->Len)){
			SendShowData(L"硬件版本号正确");
		}else {
			SendShowData(L"硬件版本号错误");
			ErrorStr += L"硬件版本号错误\r\n";
			ErrorStrFlg = 1;
		}

	}else if (Num == 0x81){
		if (Compare(ECUSWVersion->data,SWVersion,ECUSWVersion->Len)){
			SendShowData(L"软件版本号正确");
		}else {
			SendShowData(L"软件版本号错误");
			ErrorStr += L"软件版本号错误";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0xAA){
		if (Compare(PartNumber,PartNumberBuf,10)){
			SendShowData(L"零件号正确");
		}else {
			SendShowData(L"零件号错误");
			ErrorStr += L"零件号错误\r\n";
			ErrorStrFlg = 1;
		}
	}else if (Num == 0xAB){
		if (Compare(SystemSupplierId,SystemSupplierIdBuf,6)){
			SendShowData(L"供应商识别号正确");
		}else {
			SendShowData(L"供应商识别号错误");
			ErrorStr += L"供应商识别号错误";
			ErrorStrFlg = 1;
		}
	}

}

void CCANFactorySetDlg::SetSystemSupplierId()
{
	CString Str,Str1;

	//	MultiByteToWideChar(CP_ACP,0,(LPCSTR)ECUSWVersion,ECUSWVersion->Len/*sizeof(ECUHWVersion)*/,(LPWSTR)(LPCTSTR)Str,ECUSWVersion->Len/*sizeof(ECUHWVersion)*/);

	for (UINT i = 0;i<6;i++)
	{
		Str1.Format(L"%c",SystemSupplierId[i]);
		Str += Str1;
	}

	GetDlgItem(IDC_EDIT21)->SetWindowText(Str);
}

void CCANFactorySetDlg::SetPartNumber()
{
	CString Str,Str1;

	//	MultiByteToWideChar(CP_ACP,0,(LPCSTR)ECUSWVersion,ECUSWVersion->Len/*sizeof(ECUHWVersion)*/,(LPWSTR)(LPCTSTR)Str,ECUSWVersion->Len/*sizeof(ECUHWVersion)*/);

	for (UINT i = 0;i<10;i++)
	{
		Str1.Format(L"%c",PartNumber[i]);
		Str += Str1;
	}

	GetDlgItem(IDC_EDIT20)->SetWindowText(Str);
}


void CCANFactorySetDlg::SetECUSWVersion()
{
	CString Str,Str1;

	//	MultiByteToWideChar(CP_ACP,0,(LPCSTR)ECUSWVersion,ECUSWVersion->Len/*sizeof(ECUHWVersion)*/,(LPWSTR)(LPCTSTR)Str,ECUSWVersion->Len/*sizeof(ECUHWVersion)*/);

	for (UINT i = 0;i<(ECUSWVersion->Len);i++)
	{
		Str1.Format(L"%c",ECUSWVersion->data[i]);
		Str += Str1;
	}

	GetDlgItem(IDC_EDIT18)->SetWindowText(Str);
}

void CCANFactorySetDlg::SetECUHWVersion()
{
	CString Str,Str1;
	//	MultiByteToWideChar(CP_ACP,0,(LPCSTR)ECUHWVersion,ECUHWVersion->Len/*sizeof(ECUHWVersion)*/,(LPWSTR)(LPCTSTR)Str,ECUHWVersion->Len);
	for (UINT i = 0;i<(ECUHWVersion->Len);i++)
	{
		Str1.Format(L"%c",ECUHWVersion->data[i]);
		Str += Str1;
	}
	GetDlgItem(IDC_EDIT19)->SetWindowText(Str);
	Str.Empty();
}

void CCANFactorySetDlg::SetSN()
{
	CString Str,Str1 ;
	UINT NUM;

	for (UINT i = 0;i<6;i++)
	{
		Str1.Format(L"%c",RxDVDSN[i]);
		Str += Str1;
	}

	GetDlgItem(IDC_STATIC24)->SetWindowText(Str);
	Str.Empty();
	Str1.Empty();

	for (UINT i = 6;i<16;i++)
	{
		Str1.Format(L"%c",RxDVDSN[i]);
		Str += Str1;
	}
	GetDlgItem(IDC_STATIC25)->SetWindowText(Str);
	Str.Empty();
	Str1.Empty();

	Str.GetBuffer(4);
	Str = L"20";
	NUM = RxDVDSN[16]-(RxDVDSN[16]/16)*6;
	Str1.Format(L"%d",NUM);
	Str += Str1;
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT15)->SetWindowText(Str);
	Str.GetBuffer(2);
	NUM = RxDVDSN[17]-(RxDVDSN[17]/16)*6;
	Str.Format(L"%02d",NUM);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT16)->SetWindowText(Str);
	Str.GetBuffer(2);
	NUM = RxDVDSN[18]-(RxDVDSN[18]/16)*6;
	Str.Format(L"%d",NUM);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT17)->SetWindowText(Str);
	Str.GetBuffer(2);
	Str.Format(L"%c",RxDVDSN[19]);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT13)->SetWindowText(Str);

	NUM = (RxDVDSN[20]<<24)|((RxDVDSN[21]<<16)|((RxDVDSN[22]<<8)|RxDVDSN[23]));
	Str.Format(L"%05d",NUM);
	GetDlgItem(IDC_EDIT14)->SetWindowText(Str);

}

void CCANFactorySetDlg::SetNetworkConfig()
{
	////字节0，第7位EPS
	//if ((RxNetWorkConfig[0]&0x80) == 0x80)
	//{
	//	m_eps_config1.SetIcon(IDI_LEDON);
	//	m_eps_config0.SetIcon(IDI_LED_BLUEOFF);
	//}else if((RxNetWorkConfig[0]&0x80) == 0x00){
	//	m_eps_config0.SetIcon(IDI_LEDON);
	//	m_eps_config1.SetIcon(IDI_LED_BLUEOFF);
	//}
	//字节0，第6位PEPS
	if ((RxNetWorkConfig[0]&0x40) == 0x40)
	{
		/*NetWorkConfigData[0] = 0x40;*/
		m_NetworkConfig1.SetIcon(IDI_LEDON);
		m_NetworkConfig0.SetIcon(IDI_LED_BLUEOFF);
	}else if((RxNetWorkConfig[0]&0x40) == 0x00){
	/*	NetWorkConfigData[0] = 0x00;*/
		m_NetworkConfig0.SetIcon(IDI_LEDON);
		m_NetworkConfig1.SetIcon(IDI_LED_BLUEOFF);
	}

	//字节0，第5位ESC
	if ((RxNetWorkConfig[0]&0x20) == 0x20)
	{
		m_esc_config1.SetIcon(IDI_LEDON);
		m_esc_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if((RxNetWorkConfig[0]&0x20) == 0x00){
		m_esc_config0.SetIcon(IDI_LEDON);
		m_esc_config1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节0，第4位ACU
	if ((RxNetWorkConfig[0]&0x10) == 0x10)
	{
		m_acu_config1.SetIcon(IDI_LEDON);
		m_acu_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if((RxNetWorkConfig[0]&0x10) == 0x00){
		m_acu_config0.SetIcon(IDI_LEDON);
		m_acu_config1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节0，第3位TCU
	if ((RxNetWorkConfig[0]&0x08) == 0x08)
	{
		m_tcu_config1.SetIcon(IDI_LEDON);
		m_tcu_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if((RxNetWorkConfig[0]&0x08) == 0x00){
		m_tcu_config0.SetIcon(IDI_LEDON);
		m_tcu_config1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节0，第2位BCM
	if ((RxNetWorkConfig[0]&0x04) == 0x04)
	{
		/*NetWorkConfigData[0] = 0x40;*/
		m_ESC_Config1.SetIcon(IDI_LEDON);
		m_ESC_Config0.SetIcon(IDI_LED_BLUEOFF);
	}else if((RxNetWorkConfig[0]&0x04) == 0x00){
		/*	NetWorkConfigData[0] = 0x00;*/
		m_ESC_Config0.SetIcon(IDI_LEDON);
		m_ESC_Config1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节0，第1位IPK
	if ((RxNetWorkConfig[0]&0x02) == 0x02)
	{
		m_ipk_config1.SetIcon(IDI_LEDON);
		m_ipk_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if((RxNetWorkConfig[0]&0x02) == 0x00){
		m_ipk_config0.SetIcon(IDI_LEDON);
		m_ipk_config1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节0，第0位ems
	if ((RxNetWorkConfig[0]&0x01) == 0x01)
	{
		m_ems_config1.SetIcon(IDI_LEDON);
		m_ems_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if((RxNetWorkConfig[0]&0x01) == 0x00){
		m_ems_config0.SetIcon(IDI_LEDON);
		m_ems_config1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节1，第5位TPMS
	if ((RxNetWorkConfig[1]&0x20) == 0x20)
	{
	/*	NetWorkConfigData[1] = 0x20;*/
		m_TPMS1.SetIcon(IDI_LEDON);
		m_TPMS0.SetIcon(IDI_LED_BLUEOFF);
	}else if ((RxNetWorkConfig[1]&0x20) == 0x00){
		/*NetWorkConfigData[1] = 0x00;*/
		m_TPMS0.SetIcon(IDI_LEDON);
		m_TPMS1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节1，第3位MMI
	if ((RxNetWorkConfig[1]&0x08) == 0x08)
	{
		m_mmi_config1.SetIcon(IDI_LEDON);
		m_mmi_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if ((RxNetWorkConfig[1]&0x08) == 0x00){
		m_mmi_config0.SetIcon(IDI_LEDON);
		m_mmi_config1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节1，第2位AC
	if ((RxNetWorkConfig[1]&0x04) == 0x04)
	{
		m_ac_config1.SetIcon(IDI_LEDON);
		m_ac_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if ((RxNetWorkConfig[1]&0x04) == 0x00){
		m_ac_config0.SetIcon(IDI_LEDON);
		m_ac_config1.SetIcon(IDI_LED_BLUEOFF);
	}
	//字节1，第0位SAS
	if ((RxNetWorkConfig[1]&0x01) == 0x01)
	{
		m_sas_config1.SetIcon(IDI_LEDON);
		m_sas_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if ((RxNetWorkConfig[1]&0x01) == 0x00){
		m_sas_config0.SetIcon(IDI_LEDON);
		m_sas_config1.SetIcon(IDI_LED_BLUEOFF);
	}
	////字节2，第5位ABS
	//if ((RxNetWorkConfig[2]&0x20) == 0x20)			//20190414: Modified by liufeifei
	//{
	//	m_abs_config1.SetIcon(IDI_LEDON);
	//	m_abs_config0.SetIcon(IDI_LED_BLUEOFF);
	//}else if ((RxNetWorkConfig[2]&0x20) == 0x00){
	//	m_abs_config0.SetIcon(IDI_LEDON);
	//	m_abs_config1.SetIcon(IDI_LED_BLUEOFF);
	//}
	////字节2，第0位EPB
	//if ((RxNetWorkConfig[2]&0x01) == 0x01)			//20190414: Modified by liufeifei
	//{
	//	m_epb_config1.SetIcon(IDI_LEDON);
	//	m_epb_config0.SetIcon(IDI_LED_BLUEOFF);
	//}else if ((RxNetWorkConfig[2]&0x01) == 0x00){
	//	m_epb_config0.SetIcon(IDI_LEDON);
	//	m_epb_config1.SetIcon(IDI_LED_BLUEOFF);
	//}


	//字节4，第6位GW
	if ((RxNetWorkConfig[4]&0x40) == 0x40)
	{
		gw_config1.SetIcon(IDI_LEDON);
		gw_config0.SetIcon(IDI_LED_BLUEOFF);
	}else if((RxNetWorkConfig[4]&0x40) == 0x00){
		
		gw_config1.SetIcon(IDI_LED_BLUEOFF);
		gw_config0.SetIcon(IDI_LEDON);
	}

}

void CCANFactorySetDlg::SetMMIConfig()
{
	////字节0，第7位BCM后视镜折叠状态
	//if ((RxMMIConfig[0]&0x80) == 0x80)
	//{
	///*	MMIConfigData[0] = 0x01;*/
	//	m_BCM_Rmirror1.SetIcon(IDI_LEDON);
	//	m_BCM_Rmirror0.SetIcon(IDI_LED_BLUEOFF);
	//}else{
	//	/*MMIConfigData[0] = 0x00;*/
	//	m_BCM_Rmirror0.SetIcon(IDI_LEDON);
	//	m_BCM_Rmirror1.SetIcon(IDI_LED_BLUEOFF);
	//}
	////字节0，第3位全景天窗防夹
	//if ((RxMMIConfig[0]&0x08) == 0x08)
	//{
	//	m_rooffj1.SetIcon(IDI_LEDON);
	//	m_rooffj0.SetIcon(IDI_LED_BLUEOFF);
	//}else{
	//	m_rooffj0.SetIcon(IDI_LEDON);
	//	m_rooffj1.SetIcon(IDI_LED_BLUEOFF);
	//}
	////字节0，第2位国五/国六
	//if ((RxMMIConfig[0]&0x04) == 0x04)
	//{
	//	m_gl.SetIcon(IDI_LEDON);
	//	m_gw.SetIcon(IDI_LED_BLUEOFF);
	//}else{
	//	m_gw.SetIcon(IDI_LEDON);
	//	m_gl.SetIcon(IDI_LED_BLUEOFF);
	//}
	////字节0，第0位行车记录仪
	//if ((RxMMIConfig[0]&0x01) == 0x01)
	//{
	//	m_xc.SetIcon(IDI_LEDON);
	//	m_xc_w.SetIcon(IDI_LED_BLUEOFF);
	//}else{
	//	m_xc.SetIcon(IDI_LED_BLUEOFF);
	//	m_xc_w.SetIcon(IDI_LEDON);
	//}
	////字节1，第7位自动关窗功能
	//if ((RxMMIConfig[1]&0x80) == 0x80)
	//{
	//	m_Auto_CloseWindow1.SetIcon(IDI_LEDON);
	//	m_Auto_CloseWindow0.SetIcon(IDI_LED_BLUEOFF);
	//}else{
	//	m_Auto_CloseWindow0.SetIcon(IDI_LEDON);
	//	m_Auto_CloseWindow1.SetIcon(IDI_LED_BLUEOFF);
	//}
	////字节0，第4,5,6位车辆类型
	//if ((RxMMIConfig[0]&0x70) == 0x20){//5位为1，4位为0值10
	//	m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
	//	m_cx_3ah.SetIcon(IDI_LED_BLUEOFF);
	//	m_cx_FE_3AH.SetIcon(IDI_LEDON);
	//}else if ((RxMMIConfig[0]&0x70) == 0x10){//5位为0，4位为1值01
	//	m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
	//	m_cx_3ah.SetIcon(IDI_LEDON);
	//	m_cx_FE_3AH.SetIcon(IDI_LED_BLUEOFF);
	//}else if ((RxMMIConfig[0]&0x70) == 0x00){//5位为0，4位为0值00
	//	m_cx_wpp.SetIcon(IDI_LEDON);
	//	m_cx_3ah.SetIcon(IDI_LED_BLUEOFF);
	//	m_cx_FE_3AH.SetIcon(IDI_LED_BLUEOFF);
	//}
	////字节1，第2位空调
	//if ((RxMMIConfig[1]&0x04) == 0x04)
	//{
	//	m_kt1.SetIcon(IDI_LEDON);
	//	m_kt0.SetIcon(IDI_LED_BLUEOFF);
	//}else{
	//	m_kt0.SetIcon(IDI_LEDON);
	//	m_kt1.SetIcon(IDI_LED_BLUEOFF);
	//}

	//字节4，第2,3位车辆类型
	if ((RxMMIConfig[4]&0x0C) == 0x0C){//3位为1，2位为1值11
		m_Carmera0.SetIcon(IDI_LED_BLUEOFF);
		m_Carmera1.SetIcon(IDI_LED_BLUEOFF);
		m_Console_Ambient_Light0.SetIcon(IDI_LED_BLUEOFF);
		m_Console_Ambient_Light1.SetIcon(IDI_LEDON);
	}else if ((RxMMIConfig[4]&0x0C) == 0x08){//3位为1，2位为0值10
		m_Carmera0.SetIcon(IDI_LED_BLUEOFF);
		m_Carmera1.SetIcon(IDI_LED_BLUEOFF);
		m_Console_Ambient_Light0.SetIcon(IDI_LEDON);
		m_Console_Ambient_Light1.SetIcon(IDI_LED_BLUEOFF);
	}else if ((RxMMIConfig[4]&0x0C) == 0x04){//3位为0，2位为1值01
		m_Carmera0.SetIcon(IDI_LED_BLUEOFF);
		m_Carmera1.SetIcon(IDI_LEDON);
		m_Console_Ambient_Light0.SetIcon(IDI_LED_BLUEOFF);
		m_Console_Ambient_Light1.SetIcon(IDI_LED_BLUEOFF);
	}else if ((RxMMIConfig[4]&0x0C) == 0x00){//3位为0，2位为0值00
		m_Carmera0.SetIcon(IDI_LEDON);
		m_Carmera1.SetIcon(IDI_LED_BLUEOFF);
		m_Console_Ambient_Light0.SetIcon(IDI_LED_BLUEOFF);
		m_Console_Ambient_Light1.SetIcon(IDI_LED_BLUEOFF);
	}



}


void CCANFactorySetDlg::SetECUConfigDate()
{
	CString Str,Str1 ;
	UINT NUM;
	Str.GetBuffer(4);
	NUM = RxECUConfigDate[0]-(RxECUConfigDate[0]/16)*6;
	Str.Format(L"%d",NUM);
	NUM = RxECUConfigDate[1]-(RxECUConfigDate[1]/16)*6;
	Str1.Format(L"%d",NUM);
	Str += Str1;
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT10)->SetWindowText(Str);
	Str.GetBuffer(2);
	NUM = RxECUConfigDate[2]-(RxECUConfigDate[2]/16)*6;
	Str.Format(L"%02d",NUM);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT11)->SetWindowText(Str);
	Str.GetBuffer(2);
	NUM = RxECUConfigDate[3]-(RxECUConfigDate[3]/16)*6;
	Str.Format(L"%d",NUM);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT12)->SetWindowText(Str);
}

void CCANFactorySetDlg::SetECUSWDate()
{
	CString Str,Str1 ;
	UINT NUM;
	Str.GetBuffer(4);
	NUM = RxECUSWDate[0]-(RxECUSWDate[0]/16)*6;
	Str.Format(L"%d",NUM);
	NUM = RxECUSWDate[1]-(RxECUSWDate[1]/16)*6;
	Str1.Format(L"%d",NUM);
	Str += Str1;
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT7)->SetWindowText(Str);
	Str.GetBuffer(2);
	NUM = RxECUSWDate[2]-(RxECUSWDate[2]/16)*6;
	Str.Format(L"%02d",NUM);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT8)->SetWindowText(Str);
	Str.GetBuffer(2);
	NUM = RxECUSWDate[3]-(RxECUSWDate[3]/16)*6;
	Str.Format(L"%d",NUM);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT9)->SetWindowText(Str);
}

void CCANFactorySetDlg::SetECUMakeDate()
{
	CString Str,Str1 ;
	UINT NUM;
	Str.GetBuffer(4);
	NUM = RxECUMakeDate[0]-(RxECUMakeDate[0]/16)*6;
	Str.Format(L"%d",NUM);
	NUM = RxECUMakeDate[1]-(RxECUMakeDate[1]/16)*6;
	Str1.Format(L"%d",NUM);
	Str += Str1;
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT4)->SetWindowText(Str);
	Str.GetBuffer(2);
	NUM = RxECUMakeDate[2]-(RxECUMakeDate[2]/16)*6;
	Str.Format(L"%02d",NUM);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT5)->SetWindowText(Str);
	Str.GetBuffer(2);
	NUM = RxECUMakeDate[3]-(RxECUMakeDate[3]/16)*6;
	Str.Format(L"%d",NUM);
	Str.ReleaseBuffer();
	GetDlgItem(IDC_EDIT6)->SetWindowText(Str);

}

void CCANFactorySetDlg::SetDiagnoseSN()
{
	CString Str,Str1;
	for (UINT i = 0;i<16;i++)
	{
		Str1.Format(L"%c",RxDiagnoseSN[i]);
		Str += Str1;
	}
	GetDlgItem(IDC_EDIT3)->SetWindowText(Str);
}

void CCANFactorySetDlg::SetCarVIN()
{
	CString Str,Str1;
	for (UINT i = 0;i<17;i++)
	{
		Str1.Format(L"%c",RxCarVIN[i]);
		Str += Str1;
	}
	GetDlgItem(IDC_EDIT2)->SetWindowText(Str);
}

void CCANFactorySetDlg::SetCarName()
{
	 CString Str,Str1;
/*	 MultiByteToWideChar(CP_ACP,0,(LPCSTR)RxCarName,8,(LPWSTR)(LPCTSTR)Str,8);*/
	 for (UINT i = 0;i<8;i++)
	 {
		 Str1.Format(L"%c",RxCarName[i]);
		 Str += Str1;
	 }
	 GetDlgItem(IDC_EDIT1)->SetWindowText(Str);
}



HBRUSH CCANFactorySetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	CBrush m_brush;                                                            // 画刷 对象
	// TODO:  在此更改 DC 的任何特性

	if(
		pWnd->GetDlgCtrlID() == IDC_STATIC53||
		pWnd->GetDlgCtrlID() == IDC_STATIC4||
		pWnd->GetDlgCtrlID() == IDC_STATIC5||
		pWnd->GetDlgCtrlID() == IDC_STATIC6||
// 		pWnd->GetDlgCtrlID() == IDC_STATIC7||
// 		pWnd->GetDlgCtrlID() == IDC_STATIC8||
// 		pWnd->GetDlgCtrlID() == IDC_STATIC9||
// 		pWnd->GetDlgCtrlID() == IDC_STATIC10||
// 		pWnd->GetDlgCtrlID() == IDC_STATIC11||
// 		pWnd->GetDlgCtrlID() == IDC_STATIC12||
		pWnd->GetDlgCtrlID() == IDC_STATIC13||
		pWnd->GetDlgCtrlID() == IDC_STATIC14||
		pWnd->GetDlgCtrlID() == IDC_STATIC15||
		pWnd->GetDlgCtrlID() == IDC_STATIC16||
		pWnd->GetDlgCtrlID() == IDC_STATIC17||
		pWnd->GetDlgCtrlID() == IDC_STATIC18||
		pWnd->GetDlgCtrlID() == IDC_STATIC19||
		pWnd->GetDlgCtrlID() == IDC_STATIC21||
		pWnd->GetDlgCtrlID() == IDC_STATIC20||
		pWnd->GetDlgCtrlID() == IDC_STATIC22||
//		pWnd->GetDlgCtrlID() == IDC_STATIC21||
		pWnd->GetDlgCtrlID() == IDC_STATIC23||
 		pWnd->GetDlgCtrlID() == IDC_STATIC24||
		pWnd->GetDlgCtrlID() == IDC_STATIC25||
		pWnd->GetDlgCtrlID() == IDC_STATIC26||
		pWnd->GetDlgCtrlID() == IDC_STATIC27||
		pWnd->GetDlgCtrlID() == IDC_STATIC28||
		pWnd->GetDlgCtrlID() == IDC_STATIC29||
		pWnd->GetDlgCtrlID() == IDC_STATIC30||
		pWnd->GetDlgCtrlID() == IDC_STATIC31||
		pWnd->GetDlgCtrlID() == IDC_STATIC32||
		pWnd->GetDlgCtrlID() == IDC_STATIC33||
		pWnd->GetDlgCtrlID() == IDC_STATIC34||
		pWnd->GetDlgCtrlID() == IDC_STATIC35||
		pWnd->GetDlgCtrlID() == IDC_STATIC36||
		pWnd->GetDlgCtrlID() == IDC_STATIC37||
		pWnd->GetDlgCtrlID() == IDC_STATIC38||
		pWnd->GetDlgCtrlID() == IDC_STATIC39||
		pWnd->GetDlgCtrlID() == IDC_STATIC40||
		pWnd->GetDlgCtrlID() == IDC_STATIC41||
		pWnd->GetDlgCtrlID() == IDC_STATIC42||
		pWnd->GetDlgCtrlID() == IDC_STATIC43||
		pWnd->GetDlgCtrlID() == IDC_STATIC44||
		pWnd->GetDlgCtrlID() == IDC_STATIC45||
		pWnd->GetDlgCtrlID() == IDC_STATIC46||
		pWnd->GetDlgCtrlID() == IDC_STATIC47||
		pWnd->GetDlgCtrlID() == IDC_STATIC49||
		pWnd->GetDlgCtrlID() == IDC_STATIC50||
		pWnd->GetDlgCtrlID() == IDC_STATIC51||
		pWnd->GetDlgCtrlID() == IDC_STATIC52||
		pWnd->GetDlgCtrlID() == IDC_STATIC48)                             
	{
		pDC->SetBkMode(TRANSPARENT);                                            // 设置背景函数（TRANSPARENT 透明）
		
		if (
//			pWnd->GetDlgCtrlID() == IDC_STATIC21||
// 			pWnd->GetDlgCtrlID() == IDC_STATIC23||
 			pWnd->GetDlgCtrlID() == IDC_STATIC24||
			pWnd->GetDlgCtrlID() == IDC_STATIC25||
			pWnd->GetDlgCtrlID() == IDC_STATIC26||
			pWnd->GetDlgCtrlID() == IDC_STATIC27||
			pWnd->GetDlgCtrlID() == IDC_STATIC28||
			pWnd->GetDlgCtrlID() == IDC_STATIC29||
			pWnd->GetDlgCtrlID() == IDC_STATIC30||
			pWnd->GetDlgCtrlID() == IDC_STATIC31)                             
		{
			//pDC->SetTextColor(RGB(57,182,249));
			pDC->SetTextColor(RGB(57,182,249));
		}else{
			pDC->SetTextColor(RGB(0,0,0));
		}
		
		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}


	if( pWnd->GetDlgCtrlID() == IDC_EDIT1||
		pWnd->GetDlgCtrlID() == IDC_EDIT2||
		pWnd->GetDlgCtrlID() == IDC_EDIT3||
		pWnd->GetDlgCtrlID() == IDC_EDIT4||
		pWnd->GetDlgCtrlID() == IDC_EDIT5||
		pWnd->GetDlgCtrlID() == IDC_EDIT6||
		pWnd->GetDlgCtrlID() == IDC_EDIT7||
		pWnd->GetDlgCtrlID() == IDC_EDIT8||
		pWnd->GetDlgCtrlID() == IDC_EDIT9||
		pWnd->GetDlgCtrlID() == IDC_EDIT10||
		pWnd->GetDlgCtrlID() == IDC_EDIT11||
		pWnd->GetDlgCtrlID() == IDC_EDIT12||
		pWnd->GetDlgCtrlID() == IDC_EDIT13||
		pWnd->GetDlgCtrlID() == IDC_EDIT14||
		pWnd->GetDlgCtrlID() == IDC_EDIT15||
		pWnd->GetDlgCtrlID() == IDC_EDIT16||
		pWnd->GetDlgCtrlID() == IDC_EDIT17||
		pWnd->GetDlgCtrlID() == IDC_EDIT18||
		pWnd->GetDlgCtrlID() == IDC_EDIT19||
		pWnd->GetDlgCtrlID() == IDC_EDIT20||
		pWnd->GetDlgCtrlID() == IDC_EDIT21
		)   
	{
		if (pWnd->GetDlgCtrlID() == IDC_EDIT18||
			pWnd->GetDlgCtrlID() == IDC_EDIT19||
			pWnd->GetDlgCtrlID() == IDC_EDIT20||
			pWnd->GetDlgCtrlID() == IDC_EDIT21)
		{
			COLORREF bkColor1=GetSysColor(COLOR_WINDOW);
bkColor1=RGB(21,104,148);
		
			CRect rcRect1;
			pWnd->GetClientRect(&rcRect1);
			pDC->FillSolidRect(rcRect1,bkColor1);
			pDC->SetBkColor(bkColor1);
		}else{
			COLORREF bkColor=GetSysColor(COLOR_WINDOW);
			bkColor=RGB(40,112,150);
			CRect rcRect;
			pWnd->GetClientRect(&rcRect);
			pDC->FillSolidRect(rcRect,bkColor);
			pDC->SetBkColor(bkColor);
		}

		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CCANFactorySetDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel == 1)
	{
		return ;
	}
	UINT Len = 0;
	CString Str;
	m_Edit_CarName.GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请输入车辆名称"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarNameData,Str.GetLength(),NULL,NULL);
	m_Edit_CarVIN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarVINData,Str.GetLength(),NULL,NULL);
	m_Edit_DiagnoseSN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)DiagnoseSNData,Str.GetLength(),NULL,NULL);
	ECUMakeDate();
	ECUSWDate();
	ECUConfigDate();

	GetDlgItem(IDC_EDIT18)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)SWVersion,Str.GetLength(),NULL,NULL);
	ByteStuffing(SWVersion,(Str.GetLength()),16);
	GetDlgItem(IDC_EDIT19)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)HWVersion,Str.GetLength(),NULL,NULL);
	ByteStuffing(HWVersion,(Str.GetLength()),16);

	GetDlgItem(IDC_EDIT20)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置零件号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)PartNumberBuf,Str.GetLength(),NULL,NULL);
	GetDlgItem(IDC_EDIT21)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置供应商识别号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)SystemSupplierIdBuf,Str.GetLength(),NULL,NULL);
	if (!GetSNDate())
	{
		return ;
	}
	
	fixer.Reset();
	fixer.StartSet();
	CCANSend::Multi_Frame(TestID,0x2E,0xA0,NULL,CarNameData,sizeof(CarNameData));
//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;

	trigger_by_set_key_ = TRUE;
}

void CCANFactorySetDlg::ByteStuffing(BYTE *data/*数据*/,
	UINT DataLen/*数据长度*/,UINT FinalLen/*最终数据长度*/)
{

	for (;DataLen<FinalLen;DataLen++)
	{
		*(data+DataLen) = 0x20;
	}
}

void CCANFactorySetDlg::ECUMakeDate()
{
	CString Dat;
	UINT    iDat;
	UpdateData(TRUE);
	m_Edit_Year0.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUMakeDayData[0]=((iDat/1000)*16)+(iDat/100%10);
	ECUMakeDayData[1]=((iDat/10%100%10)*16)+(iDat%1000%100%10);

	m_Edit_month0.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUMakeDayData[2]=((iDat/10)*16)+(iDat%10);

	m_Edit_Day0.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUMakeDayData[3]=((iDat/10)*16)+(iDat%10);
}

void CCANFactorySetDlg::ECUSWDate()
{
	CString Dat;
	UINT    iDat;
	UpdateData(TRUE);
	m_Edit_Year1.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUSWDayData[0]=((iDat/1000)*16)+(iDat/100%10);
	ECUSWDayData[1]=((iDat/10%100%10)*16)+(iDat%1000%100%10);

	m_Edit_month1.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUSWDayData[2]=((iDat/10)*16)+(iDat%10);

	m_Edit_Day1.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUSWDayData[3]=((iDat/10)*16)+(iDat%10);
}

void CCANFactorySetDlg::ECUConfigDate()
{
	CString Dat;
	UINT    iDat;
	UpdateData(TRUE);
	m_Edit_Year2.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUConfigDayData[0]=((iDat/1000)*16)+(iDat/100%10);
	ECUConfigDayData[1]=((iDat/10%100%10)*16)+(iDat%1000%100%10);

	m_Edit_month2.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUConfigDayData[2]=((iDat/10)*16)+(iDat%10);

	m_Edit_Day2.GetWindowText(Dat);
	iDat=_ttoi(Dat);
	ECUConfigDayData[3]=((iDat/10)*16)+(iDat%10);
}

UINT CCANFactorySetDlg::GetSNDate()
{
	CString Sum;

	/*UINT iSum;*/
	GetDlgItem(IDC_EDIT14)->GetWindowText(Sum);
	if (Sum.IsEmpty())
	{
		MessageBox(_T("请输入产品序列号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return 0;
	}

	iSum=_ttoi(Sum);
	SNData[23] = iSum & 0xff;
	SNData[22] = (iSum>>8)&0xff;
	SNData[21] = (iSum>>16)&0xff;
	SNData[20] = (iSum>>24)&0xff;
	GetDlgItem(IDC_EDIT13)->GetWindowText(Sum);
	if (Sum.IsEmpty())
	{
		MessageBox(_T("请输入产线编号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return 0;
	}

	SNData[19] = (BYTE)*Sum;

	GetDlgItem(IDC_EDIT15)->GetWindowText(Sum);
	SNData[16]=((_ttoi(Sum)/10%100%10)*16)+(_ttoi(Sum)%1000%100%10);
	GetDlgItem(IDC_EDIT16)->GetWindowText(Sum);
	SNData[17]=((_ttoi(Sum)/10)*16)+(_ttoi(Sum)%10);
	GetDlgItem(IDC_EDIT17)->GetWindowText(Sum);
	//sprintf((char *)SNData+17,"%d",_ttoi(Sum));
	SNData[18]=((_ttoi(Sum)/10)*16)+(_ttoi(Sum)%10);
	return 1;
}

// void CCANFactorySetDlg::DisTxData( BYTE *byData,int inum,UINT DLC)
// {
// 	CString sSwapA,sSwapB;
// 
// 	if (inum == 0){
// 		sSwapA = L"Tx车辆名称： "; 
// 	}else if (inum == 1){
// 		sSwapA = L"Tx车辆VIN码： "; 
// 	}else if (inum == 2){
// 		sSwapA = L"Tx诊断仪序列号： "; 
// 	}else if (inum == 3){
// 		sSwapA = L"TxECU制造日期： "; 
// 	}else if (inum == 4){
// 		sSwapA = L"TxECU软件刷新日期： "; 
// 	}else if (inum == 5){
// 		sSwapA = L"TxECU装配日期： "; 
// 	}else if (inum == 6){
// 		sSwapA = L"TxMMI配置功能： "; 
// 	}else if (inum == 7){
// 		sSwapA = L"Tx网络架构配置： ";
// 	}else if (inum == 8){
// 		sSwapA = L"Tx机器序列号： "; 
// 	}else if (inum == 9){
// 	//	sSwapA = L" "; 
// 	}else if (inum == 10)
// 	{
// 		sSwapA = L"Rx： "; 
// 	}else if (inum == 11)
// 	{
// 		sSwapA = L"Tx： "; 
// 	}
// 
// 	for(UINT ii=0;ii<DLC;ii++)
// 	{
// 		sSwapB.Format(_T("%02X "),byData[ii]);
// 		sSwapA += sSwapB;
// 	}
// 	Show_Box(sSwapA);
// }
// 
// 
// 
// void CCANFactorySetDlg::Show_Box(CString dat)
// {
// 	if (dat.GetLength()>20)
// 	{
// 		m_ListBox.SetHorizontalExtent(1000);
// 	}
// 	m_ListBox.InsertString(m_ListBox.GetCount(),dat);
// 	m_ListBox.SetCurSel(m_ListBox.GetCount()-1);
// }


void CCANFactorySetDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] &= 0xBF;
	m_NetworkConfig0.SetIcon(IDI_LEDON);
	m_NetworkConfig1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
 	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2 )
		return;

	NetWorkConfigData[0] |= 0x40;
	m_NetworkConfig1.SetIcon(IDI_LEDON);
	m_NetworkConfig0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2 )
		return;
	NetWorkConfigData[1] &= 0xDF;
	m_TPMS0.SetIcon(IDI_LEDON);
	m_TPMS1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
	//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	NetWorkConfigData[1] |= 0x20;
	m_TPMS1.SetIcon(IDI_LEDON);
	m_TPMS0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
	//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2 )
		return;
	MMIConfigData[0] &= (~0x80);
	m_BCM_Rmirror0.SetIcon(IDI_LEDON);
	m_BCM_Rmirror1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2 )
		return;
	MMIConfigData[0] |= 0x80;
	m_BCM_Rmirror1.SetIcon(IDI_LEDON);
	m_BCM_Rmirror0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[1]&=0x7F;

	m_Auto_CloseWindow0.SetIcon(IDI_LEDON);
	m_Auto_CloseWindow1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}

}


void CCANFactorySetDlg::OnBnClickedButton18()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[1] |= 0x80;
	m_Auto_CloseWindow1.SetIcon(IDI_LEDON);
	m_Auto_CloseWindow0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton25()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] &= 0x8F;
	m_cx_wpp.SetIcon(IDI_LEDON);
	m_cx_3ah.SetIcon(IDI_LED_BLUEOFF);
	m_cx_FE_3AH.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		// SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton26()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] = (MMIConfigData[0]&0x8F)|0x10;
	m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
	m_cx_3ah.SetIcon(IDI_LEDON);
	m_cx_FE_3AH.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnStnClickedStatic31()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_NumAdd.GetCheck())
	{
		m_NumAdd.SetCheck(0);
	}else{
		m_NumAdd.SetCheck(1);
	}
}


// void CCANFactorySetDlg::OnBnClickedButton22()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	CString FilePathName;
// 	CFileDialog filedlg(FALSE,L"txt",L"test",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"Excel CSV File(*.csv)|*.csv|All Files (*.txt*)|*.txt*||");
// 	if(filedlg.DoModal()==IDOK)
// 	{
// 		FilePathName=filedlg.GetPathName();
// 		CString Save,Save1;
// 		for (int i = 0;i<m_ListBox.GetCount();i++)
// 		{
// 			m_ListBox.GetText(i,Save1);
// 			Save1.Replace(L"\r\n",L"");
// 			Save += Save1;
// 			Save += L"\r\n";
// 		}
// 
// 		TRY 
// 		{
// 			CFile file(FilePathName, CFile::modeCreate | CFile::modeWrite);
// 			file.SeekToBegin();//到达文件开头   
// 			file.Write("\xff\xfe", 2); 
// 			file.Write(Save,Save.GetLength()*2);
// 			file.Close();
// 			MessageBox(_T("保存成功"),_T("提示"),MB_OK|MB_ICONQUESTION);
// 		}
// 		CATCH (CMemoryException, e)
// 		{
// 			e->ReportError();
// 			e->Delete();
// 		}
// 		END_CATCH
// 	}
// }


// void CCANFactorySetDlg::OnBnClickedButton21()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	m_ListBox.ResetContent();
// 	m_ListBox.SetHorizontalExtent(10);
// }


// void CCANFactorySetDlg::OnBnClickedButton23()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	if (ToolModel == 0)
// 	{
// 		return ;
// 	}
// 	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
// 	if (ToolModel == 1)
// 	{
// 		GetDlgItem(IDC_EDIT13)->SetWindowText(SNStr);
// 		GetDlgItem(IDC_EDIT14)->SetWindowText(SNStr1);
// 		GetDlgItem(IDC_EDIT1)->SetWindowText(CarNameStr);
// 		GetDlgItem(IDC_EDIT2)->SetWindowText(CarVINStr);
// 		GetDlgItem(IDC_EDIT3)->SetWindowText(DiagnoseSNStr);
// 	}
// 	m_Mode1.SetIcon(IDI_LEDOFF);
// 	m_Mode2.SetIcon(IDI_LEDOFF);
// 	m_Mode3.SetIcon(IDI_LEDOFF);
// 	m_Mode0.SetIcon(IDI_LEDON);
// 	ToolModel = 0;
// 
// 
// 	Year.Format(_T("%02d"),t.GetYear());
// 	Months.Format(_T("%02d"),t.GetMonth());
// 	Date.Format(_T("%02d"),t.GetDay());
// 	GetDlgItem(IDC_EDIT4)->SetWindowText(Year);
// 	GetDlgItem(IDC_EDIT5)->SetWindowText(Months);
// 	GetDlgItem(IDC_EDIT6)->SetWindowText(Date);
// 	GetDlgItem(IDC_EDIT7)->SetWindowText(Year);
// 	GetDlgItem(IDC_EDIT8)->SetWindowText(Months);
// 	GetDlgItem(IDC_EDIT9)->SetWindowText(Date);
// 	GetDlgItem(IDC_EDIT10)->SetWindowText(Year);
// 	GetDlgItem(IDC_EDIT11)->SetWindowText(Months);
// 	GetDlgItem(IDC_EDIT12)->SetWindowText(Date);
// 	GetDlgItem(IDC_EDIT15)->SetWindowText(Year);
// 	GetDlgItem(IDC_EDIT16)->SetWindowText(Months);
// 	GetDlgItem(IDC_EDIT17)->SetWindowText(Date);
// 
// 	GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT11)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT12)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT15)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT16)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT17)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT18)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT19)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT20)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT21)->SetWindowText(L"");
// 
// 	m_NetworkConfig0.SetIcon(IDI_LED_BLUEOFF);
// 	m_NetworkConfig1.SetIcon(IDI_LEDON);
// 	m_TPMS0.SetIcon(IDI_LED_BLUEOFF);
// 	m_TPMS1.SetIcon(IDI_LEDON);
// 	m_ESC_Config0.SetIcon(IDI_LED_BLUEOFF);
// 	m_ESC_Config1.SetIcon(IDI_LEDON);
// 	m_BCM_Rmirror0.SetIcon(IDI_LED_BLUEOFF);
// 	m_BCM_Rmirror1.SetIcon(IDI_LEDON);
// 	m_Auto_CloseWindow0.SetIcon(IDI_LEDON);
// 	m_Auto_CloseWindow1.SetIcon(IDI_LED_BLUEOFF);
// 	m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
// 	m_cx_3ah.SetIcon(IDI_LEDON);
// 
// // 	 memset(CarNameData,0,8);
// // 	 memset(CarVINData,0,17);
// // 	 memset(DiagnoseSNData,0,16);
// // 	 memset(ECUMakeDayData,0,4);
// // 	 memset(ECUSWDayData,0,4);
// // 	 memset(ECUConfigDayData,0,4);
// 	 NetWorkConfigData[0] = 0xFF;
// 	 NetWorkConfigData[1] = 0xFF;
// 	 MMIConfigData[0] = 0xFF;
// 	 MMIConfigData[1] = 0x7F;
// 
// /*     memset(&SNData[16],0,8);*/
// // 	CRect rect;
// // 	GetDlgItem(IDC_STATIC23)->GetWindowRect(rect);
// // 	this->ScreenToClient(rect);
// // 	GetDlgItem(IDC_STATIC23)->SetWindowText(_T("工厂模式"));
// // 	InvalidateRect(&rect);
// 
// 	CRect rect;
// 	for (UINT i = IDC_STATIC23;i<=IDC_STATIC25;i++)
// 	{
// 		GetDlgItem(i)->GetWindowRect(rect);
// 		this->ScreenToClient(rect);
// 		GetDlgItem(i)->SetWindowText(L"");
// 		InvalidateRect(&rect);
// 	}
// 	GetDlgItem(IDC_STATIC23)->SetWindowText(L"工厂模式");
// 	GetDlgItem(IDC_STATIC24)->SetWindowText(L"250060");
// 	GetDlgItem(IDC_STATIC25)->SetWindowText(L"0011702140");
// 	// 	InvalidateRect(&rect);
// 
// }
// 
// 
// void CCANFactorySetDlg::OnBnClickedButton24()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 
// 	GetDlgItem(IDC_EDIT1)->GetWindowText(CarNameStr);
// 	GetDlgItem(IDC_EDIT2)->GetWindowText(CarVINStr);
// 	GetDlgItem(IDC_EDIT3)->GetWindowText(DiagnoseSNStr);
// 	GetDlgItem(IDC_EDIT13)->GetWindowText(SNStr);
// 	GetDlgItem(IDC_EDIT14)->GetWindowText(SNStr1);
// 	GetDlgItem(IDC_EDIT15)->GetWindowText(SNStr2);
// 	GetDlgItem(IDC_EDIT16)->GetWindowText(SNStr3);
// 	GetDlgItem(IDC_EDIT17)->GetWindowText(SNStr4);
// 	   GetDlgItem(IDC_EDIT4)->GetWindowText(ECUMakeDayStr0);
// 	   GetDlgItem(IDC_EDIT5)->GetWindowText(ECUMakeDayStr1);
// 	   GetDlgItem(IDC_EDIT6)->GetWindowText(ECUMakeDayStr2);
// 	   GetDlgItem(IDC_EDIT7)->GetWindowText(ECUSWDayStr0);
// 	   GetDlgItem(IDC_EDIT8)->GetWindowText(ECUSWDayStr1);
// 	   GetDlgItem(IDC_EDIT9)->GetWindowText(ECUSWDayStr1);
// 	   GetDlgItem(IDC_EDIT10)->GetWindowText(ECUSWDayStr1);
// 	   GetDlgItem(IDC_EDIT11)->GetWindowText(ECUSWDayStr1);
// 	   GetDlgItem(IDC_EDIT12)->GetWindowText(ECUSWDayStr1);
//  
// 
// 	if (ToolModel == 1)
// 	{
// 		return ;
// 	}
// 	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
// 	m_Mode0.SetIcon(IDI_LEDOFF);
// 	m_Mode2.SetIcon(IDI_LEDOFF);
// 	m_Mode3.SetIcon(IDI_LEDOFF);
// 	m_Mode1.SetIcon(IDI_LEDON);
// 	ToolModel = 1;
//    
// 	GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT6)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT11)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT12)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT15)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT16)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT17)->EnableWindow(TRUE);
// 
// 	GetDlgItem(IDC_EDIT4)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT5)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT6)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT7)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT8)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT9)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT10)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT11)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT12)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT15)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT16)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT17)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT13)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT14)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT2)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT3)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT1)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT18)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT19)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT20)->SetWindowText(L"");
// 	GetDlgItem(IDC_EDIT21)->SetWindowText(L"");
// 
// 	m_NetworkConfig0.SetIcon(IDI_LED_BLUEOFF);
// 	m_NetworkConfig1.SetIcon(IDI_LED_BLUEOFF);
// 	m_TPMS0.SetIcon(IDI_LED_BLUEOFF);
// 	m_TPMS1.SetIcon(IDI_LED_BLUEOFF);
// 	m_ESC_Config0.SetIcon(IDI_LED_BLUEOFF);
// 	m_ESC_Config1.SetIcon(IDI_LED_BLUEOFF);
// // 	m_BCM_Rmirror0.SetIcon(IDI_LED_BLUEOFF);
// // 	m_BCM_Rmirror1.SetIcon(IDI_LED_BLUEOFF);
// 	m_BCM_Rmirror0.SetIcon(IDI_LED_BLUEOFF);
// 	m_BCM_Rmirror1.SetIcon(IDI_LED_BLUEOFF);
// 	m_Auto_CloseWindow0.SetIcon(IDI_LED_BLUEOFF);
// 	m_Auto_CloseWindow1.SetIcon(IDI_LED_BLUEOFF);
// 	m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
// 	m_cx_3ah.SetIcon(IDI_LED_BLUEOFF);
// 
// 	CRect rect;
// 	for (UINT i = IDC_STATIC23;i<=IDC_STATIC25;i++)
// 	{
// 		GetDlgItem(i)->GetWindowRect(rect);
// 		this->ScreenToClient(rect);
// 		GetDlgItem(i)->SetWindowText(L"");
// 		InvalidateRect(&rect);
// 	}
// 	GetDlgItem(IDC_STATIC23)->SetWindowText(L"读模式");
// // 	GetDlgItem(IDC_STATIC24)->SetWindowText(L"");
// // 	GetDlgItem(IDC_STATIC25)->SetWindowText(L"");
// // 	InvalidateRect(&rect);
// 
// }
// 
// 
// void CCANFactorySetDlg::OnBnClickedButton27()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	if (ToolModel == 1)
// 	{
// 		GetDlgItem(IDC_EDIT1)->SetWindowText(CarNameStr);
// 		GetDlgItem(IDC_EDIT2)->SetWindowText(CarVINStr);
// 		GetDlgItem(IDC_EDIT3)->SetWindowText(DiagnoseSNStr);
// 		GetDlgItem(IDC_EDIT4)->SetWindowText(ECUMakeDayStr0);
// 		GetDlgItem(IDC_EDIT5)->SetWindowText(ECUMakeDayStr1);
// 		GetDlgItem(IDC_EDIT6)->SetWindowText(ECUMakeDayStr2);
// 		GetDlgItem(IDC_EDIT7)->SetWindowText(ECUSWDayStr0);
// 		GetDlgItem(IDC_EDIT8)->SetWindowText(ECUSWDayStr1);
// 		GetDlgItem(IDC_EDIT9)->SetWindowText(ECUSWDayStr1);
// 		GetDlgItem(IDC_EDIT10)->SetWindowText(ECUSWDayStr1);
// 		GetDlgItem(IDC_EDIT11)->SetWindowText(ECUSWDayStr1);
// 		GetDlgItem(IDC_EDIT12)->SetWindowText(ECUSWDayStr1);
// 		GetDlgItem(IDC_EDIT13)->SetWindowText(SNStr);
// 		GetDlgItem(IDC_EDIT14)->SetWindowText(SNStr1);
// 		GetDlgItem(IDC_EDIT15)->SetWindowText(SNStr2);
// 		GetDlgItem(IDC_EDIT16)->SetWindowText(SNStr3);
// 		GetDlgItem(IDC_EDIT17)->SetWindowText(SNStr4);
// 
// 	}
// 
// 	if (ToolModel == 2)
// 	{
// 		return ;
// 	}
// 	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
// 	m_Mode0.SetIcon(IDI_LEDOFF);
// 	m_Mode1.SetIcon(IDI_LEDOFF);
// 	m_Mode3.SetIcon(IDI_LEDOFF);
// 	m_Mode2.SetIcon(IDI_LEDON);
// 	ToolModel = 2;
// 
// // 	Year.Format(_T("%d"),t.GetYear());
// // 	Months.Format(_T("%d"),t.GetMonth());
// // 	Date.Format(_T("%d"),t.GetDay());
// // 	GetDlgItem(IDC_EDIT4)->SetWindowText(Year);
// // 	GetDlgItem(IDC_EDIT5)->SetWindowText(Months);
// // 	GetDlgItem(IDC_EDIT6)->SetWindowText(Date);
// // 	GetDlgItem(IDC_EDIT7)->SetWindowText(Year);
// // 	GetDlgItem(IDC_EDIT8)->SetWindowText(Months);
// // 	GetDlgItem(IDC_EDIT9)->SetWindowText(Date);
// // 	GetDlgItem(IDC_EDIT10)->SetWindowText(Year);
// // 	GetDlgItem(IDC_EDIT11)->SetWindowText(Months);
// // 	GetDlgItem(IDC_EDIT12)->SetWindowText(Date);
// // 	GetDlgItem(IDC_EDIT15)->SetWindowText(Year);
// // 	GetDlgItem(IDC_EDIT16)->SetWindowText(Months);
// // 	GetDlgItem(IDC_EDIT17)->SetWindowText(Date);
// 
// 	GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT6)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT11)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT12)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT15)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT16)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT17)->EnableWindow(TRUE);
// 	m_NetworkConfig0.SetIcon(IDI_LED_BLUEOFF);
// 	m_NetworkConfig1.SetIcon(IDI_LED_BLUEOFF);
// 	m_TPMS0.SetIcon(IDI_LED_BLUEOFF);
// 	m_TPMS1.SetIcon(IDI_LED_BLUEOFF);
// 	m_ESC_Config0.SetIcon(IDI_LED_BLUEOFF);
// 	m_ESC_Config1.SetIcon(IDI_LED_BLUEOFF);
// 	m_BCM_Rmirror0.SetIcon(IDI_LED_BLUEOFF);
// 	m_BCM_Rmirror1.SetIcon(IDI_LED_BLUEOFF);
// 	m_Auto_CloseWindow0.SetIcon(IDI_LED_BLUEOFF);
// 	m_Auto_CloseWindow1.SetIcon(IDI_LED_BLUEOFF);
// 	m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
// 	m_cx_3ah.SetIcon(IDI_LED_BLUEOFF);
// 
//     
// 
// // 	CRect rect;
// // 	GetDlgItem(IDC_STATIC23)->GetWindowRect(rect);
// // 	this->ScreenToClient(rect);
// // 	GetDlgItem(IDC_STATIC23)->SetWindowText(L"测试模式");
// // 	InvalidateRect(&rect);
// 
// 	CRect rect;
// 	for (UINT i = IDC_STATIC23;i<=IDC_STATIC25;i++)
// 	{
// 		GetDlgItem(i)->GetWindowRect(rect);
// 		this->ScreenToClient(rect);
// 		GetDlgItem(i)->SetWindowText(L"");
// 		InvalidateRect(&rect);
// 	}
// 	GetDlgItem(IDC_STATIC23)->SetWindowText(L"测试模式");
// 	GetDlgItem(IDC_STATIC24)->SetWindowText(L"BD6031");
// 	GetDlgItem(IDC_STATIC25)->SetWindowText(L"0011702140");
// 
// }


void CCANFactorySetDlg::OnBnClickedButton19()
{
	
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
	{
		return ;
	}
	GetSNDate();
	CCANSend::Multi_Frame(TestID,0x2E,0x84,NULL,SNData,sizeof(SNData));
//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}


void CCANFactorySetDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
	{
		return ;
	}
	CString Str;
	m_Edit_CarVIN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarVINData,Str.GetLength(),NULL,NULL);
	CCANSend::Multi_Frame(TestID,0x2E,0xA1,NULL,CarVINData,sizeof(CarVINData));
//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}


void CCANFactorySetDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
	{
		return ;
	}
	ECUMakeDate();
	CCANSend::Single_Frame(TestID,0x2E,0xA4,NULL,ECUMakeDayData,sizeof(ECUMakeDayData));
	//SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}


void CCANFactorySetDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
	{
		return ;
	}
	ECUSWDate();
	CCANSend::Single_Frame(TestID,0x2E,0xA5,NULL,ECUSWDayData,sizeof(ECUSWDayData));
//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}


void CCANFactorySetDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
	{
		return ;
	}
	ECUConfigDate();
	CCANSend::Single_Frame(TestID,0x2E,0xA6,NULL,ECUConfigDayData,sizeof(ECUConfigDayData));
//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}


void CCANFactorySetDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
	{
		return ;
	}
	CString Str;
	m_Edit_CarName.GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请输入车辆名称"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarNameData,Str.GetLength(),NULL,NULL);
	CCANSend::Multi_Frame(TestID,0x2E,0xA0,NULL,CarNameData,sizeof(CarNameData));
	//SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}


void CCANFactorySetDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
	{
		return ;
	}
	CString Str;
	m_Edit_DiagnoseSN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)DiagnoseSNData,Str.GetLength(),NULL,NULL);
	CCANSend::Multi_Frame(TestID,0x2E,0xA2,NULL,DiagnoseSNData,sizeof(DiagnoseSNData));
//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}


void CCANFactorySetDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 1 && ToolModel != 3)
		return ;
    CCANSend::Single_Frame(TestID,0x22,0xA0,NULL,NULL,0);
	ReadMode = 1;
//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}


void CCANFactorySetDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	__super::OnCancel();
}


BOOL CCANFactorySetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
// 	if(pMsg->message==WM_KEYDOWN   &&  pMsg->wParam==VK_RETURN)    
// 	{    
// 		///pMsg->wParam=VK_ESCAPE;   //将ESC键的消息替换为回车键的消息，这样，按ESC的时候  
// 		//也会去调用OnOK函数，而OnOK什么也不做，这样ESC也被屏蔽  
// 	    return TRUE ;
// 	}    
	//if (HoneywellFlg==1)
	//{
	//	GetDlgItem(IDC_EDIT23)->SetFocus();
	//	HoneywellFlg=0;
	//}
	
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc按键事件  
			return true;  
		case VK_RETURN: //Enter按键事件  
			//GetDlgItem(IDC_EDIT23)->PostMessage(WM_KILLFOCUS, 0, 0);
			//::SendMessage(GetDlgItem(IDC_EDIT23)-> m_hWnd,WM_KILLFOCUS,-1,0);
			//SendDlgItemMessage(IDC_EDIT23,WM_KILLFOCUS,0,0);
			
			

			//SetFactoryMode();//菜单选择工厂模式
			//OnBnClickedButton20();//信息配置按钮
			//OnBnClickedButton6();//Set按钮

 			//CVersion *dlg2;
 			//dlg2 = new CVersion;
 			//dlg2->OnBnClickedButton1();//Vesion界面文件配置按钮
			//SetReadMode();//菜单选择读模式
			//OnBnClickedButton7();//Read按钮
			//GetEditText();//获取条码框的值
			//
			//savedata();//保存数据到文件（设置数据具体格式）
			return true;  
		default:  
			;  
		}  
	}  
// 	static bool m_ctrl_down = false;
// 	static bool m_Alt_down = false;
// 	if (pMsg->message == WM_KEYDOWN)
// 	{
// 		if (pMsg->wParam == 'P')
// 		{
// 			if (m_ctrl_down&&m_Alt_down)
// 			{
// 				/*MessageBox(L"asd",L"asd");*/
// // 				Anum ++ ;
// // 				if (Anum == 5)
// // 				{
// // 					Anum = 0;
// 					GetDlgItem(IDC_BUTTON27)->ShowWindow(SW_SHOW);
// 				/*}*/
// 				return TRUE ;
// 			}
// 
// 		}
// 		if (pMsg->wParam == VK_CONTROL)
// 		{
// 			m_ctrl_down = true;
// 			return TRUE;
// 		}
// 		if (pMsg->wParam == VK_MENU)
// 		{
// 			m_Alt_down = true;
// 			return TRUE;
// 		}
// 
// 	}
// 	if (pMsg->message == WM_KEYUP)
// 	{
// 		if (pMsg->wParam == VK_CONTROL)
// 		{
// 			m_ctrl_down = false ;
// 			/*Anum = 0;*/
// 			return TRUE;
// 		}
// 		if (pMsg->wParam == VK_MENU)
// 		{
// 			m_Alt_down = false ;
// 		/*	Anum = 0;*/
// 			return TRUE;
// 		}
// 
// 
// 	}


	return __super::PreTranslateMessage(pMsg);
}


void CCANFactorySetDlg::OnBnClickedButton20()
{
	CString FilePathName;
	CString ReadStr,ReadS,Str;
	UINT Index,Len,count = 0;
	char CRead[100];


	CFileDialog filedlg(TRUE,L".ini",L"", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,L"configure File(*.ini)|*.ini||");
	filedlg.m_ofn.lpstrInitialDir = FileFolderUtil::GetInstance().GetExecuteFileDirecotroy();;
	if(filedlg.DoModal()==IDOK)
	{
		FilePathName=filedlg.GetPathName();

		CTestInformationConfigurationDialog configuration;
		if (IDOK == configuration.DoModal(filedlg.GetPathName()))
		{
			SetDlgItemText(IDC_EDIT18, configuration.GetECUSoftwareVersion());
			SetDlgItemText(IDC_EDIT19, configuration.GetECUHardeardVersion());
			SetDlgItemText(IDC_EDIT20, configuration.GetPartNumber());
			SetDlgItemText(IDC_EDIT21, configuration.GetSupplierIdentifier());
		}
	}
}
void CCANFactorySetDlg::OnBnClickedButton28()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	NetWorkConfigData[0] &= 0xFB;
	m_ESC_Config0.SetIcon(IDI_LEDON);
	m_ESC_Config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
	//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}
void CCANFactorySetDlg::OnBnClickedButton29()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2 )
		return;
	NetWorkConfigData[0] |= 0x04;
	m_ESC_Config0.SetIcon(IDI_LED_BLUEOFF);
	m_ESC_Config1.SetIcon(IDI_LEDON);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
	//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}
void CCANFactorySetDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel == 2)
	       return;
	if (m_Config.GetCurSel() == 0)
	{
// 		TuBiaoys();
// 		QHInit();
	}else if (m_Config.GetCurSel() == 1)
	{
// 		TuBiaoys();
// 		QHInit();
	}
	ErrorStr.Empty();
	if (ToolModel == 3)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
	//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
	}

}

void CCANFactorySetDlg::EnterTextMode()
{
	CCANSend::Single_Frame(REQUEST_CAN_ID,0x10,0x01,NULL,NULL,0);
	if (m_NumAdd.GetCheck())
	{
		NumAdd = 1;
	}else {
		NumAdd = 0;
	}

	//SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}

void CCANFactorySetDlg::ExitTextmode()
{
	CCANSend::Single_Frame(REQUEST_CAN_ID,0x10,0x03,NULL,NULL,0);
	//SetTimer(3,0,NULL);
	DataAnalyzeFlg = 1;
}
void CCANFactorySetDlg::QHInit(void){
	NetWorkConfigData[0]=0x7F;
	NetWorkConfigData[1]=0x2D;
	NetWorkConfigData[4]=0x40;
	//MMIConfigData[4] = 0x0C;
}
void CCANFactorySetDlg::TuBiaoys(void){
	//m_eps_config0.SetIcon(IDI_LED_BLUEOFF);
	//m_eps_config1.SetIcon(IDI_LEDON);
	//m_abs_config0.SetIcon(IDI_LED_BLUEOFF);
	//m_abs_config1.SetIcon(IDI_LEDON);
	//m_epb_config0.SetIcon(IDI_LED_BLUEOFF);
	//m_epb_config1.SetIcon(IDI_LEDON);
	//m_BCM_Rmirror0.SetIcon(IDI_LED_BLUEOFF);
	//m_BCM_Rmirror1.SetIcon(IDI_LEDON);
	//m_Auto_CloseWindow0.SetIcon(IDI_LED_BLUEOFF);
	//m_Auto_CloseWindow1.SetIcon(IDI_LEDON);
	//m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
	//m_cx_3ah.SetIcon(IDI_LED_BLUEOFF);
	//m_cx_FE_3AH.SetIcon(IDI_LEDON);
	//m_rooffj1.SetIcon(IDI_LED_BLUEOFF);
	//m_rooffj0.SetIcon(IDI_LEDON);
	//m_gl.SetIcon(IDI_LED_BLUEOFF);
	//m_gw.SetIcon(IDI_LEDON);
	//m_xc.SetIcon(IDI_LEDON);
	//m_xc_w.SetIcon(IDI_LED_BLUEOFF);
	//m_kt0.SetIcon(IDI_LED_BLUEOFF);
	//m_kt1.SetIcon(IDI_LEDON);

	//m_Carmera0.SetIcon(IDI_LED_BLUEOFF);
	//m_Carmera1.SetIcon(IDI_LED_BLUEOFF);
	//m_Console_Ambient_Light0.SetIcon(IDI_LED_BLUEOFF);
	//m_Console_Ambient_Light1.SetIcon(IDI_LEDON);
	m_NetworkConfig1.SetIcon(IDI_LEDON);
	m_NetworkConfig0.SetIcon(IDI_LED_BLUEOFF);
	m_esc_config1.SetIcon(IDI_LEDON);
	m_esc_config0.SetIcon(IDI_LED_BLUEOFF);
	m_tcu_config1.SetIcon(IDI_LEDON);
	m_tcu_config0.SetIcon(IDI_LED_BLUEOFF);
	m_acu_config1.SetIcon(IDI_LEDON);
	m_acu_config0.SetIcon(IDI_LED_BLUEOFF);
	m_ESC_Config0.SetIcon(IDI_LED_BLUEOFF);
	m_ESC_Config1.SetIcon(IDI_LEDON);
	m_ipk_config1.SetIcon(IDI_LEDON);
	m_ipk_config0.SetIcon(IDI_LED_BLUEOFF);
	m_ems_config1.SetIcon(IDI_LEDON);
	m_ems_config0.SetIcon(IDI_LED_BLUEOFF);
	m_TPMS1.SetIcon(IDI_LEDON);
	m_TPMS0.SetIcon(IDI_LED_BLUEOFF);
	m_mmi_config1.SetIcon(IDI_LEDON);
	m_mmi_config0.SetIcon(IDI_LED_BLUEOFF);
	m_ac_config1.SetIcon(IDI_LEDON);
	m_ac_config0.SetIcon(IDI_LED_BLUEOFF);
	m_sas_config1.SetIcon(IDI_LEDON);
	m_sas_config0.SetIcon(IDI_LED_BLUEOFF);
	gw_config1.SetIcon(IDI_LEDON);
	gw_config0.SetIcon(IDI_LED_BLUEOFF);
}

void CCANFactorySetDlg::TuBiaohs(void){

	//m_Carmera0.SetIcon(IDI_LED_BLUEOFF);
	//m_Carmera1.SetIcon(IDI_LED_BLUEOFF);
	//m_Console_Ambient_Light0.SetIcon(IDI_LED_BLUEOFF);
	//m_Console_Ambient_Light1.SetIcon(IDI_LED_BLUEOFF);
	m_NetworkConfig1.SetIcon(IDI_LED_BLUEOFF);
	m_NetworkConfig0.SetIcon(IDI_LED_BLUEOFF);
	m_esc_config1.SetIcon(IDI_LED_BLUEOFF);
	m_esc_config0.SetIcon(IDI_LED_BLUEOFF);
	m_tcu_config1.SetIcon(IDI_LED_BLUEOFF);
	m_tcu_config0.SetIcon(IDI_LED_BLUEOFF);
	m_acu_config1.SetIcon(IDI_LED_BLUEOFF);
	m_acu_config0.SetIcon(IDI_LED_BLUEOFF);
	m_ESC_Config0.SetIcon(IDI_LED_BLUEOFF);
	m_ESC_Config1.SetIcon(IDI_LED_BLUEOFF);
	m_ipk_config1.SetIcon(IDI_LED_BLUEOFF);
	m_ipk_config0.SetIcon(IDI_LED_BLUEOFF);
	m_ems_config1.SetIcon(IDI_LED_BLUEOFF);
	m_ems_config0.SetIcon(IDI_LED_BLUEOFF);
	m_TPMS1.SetIcon(IDI_LED_BLUEOFF);
	m_TPMS0.SetIcon(IDI_LED_BLUEOFF);
	m_mmi_config1.SetIcon(IDI_LED_BLUEOFF);
	m_mmi_config0.SetIcon(IDI_LED_BLUEOFF);
	m_ac_config1.SetIcon(IDI_LED_BLUEOFF);
	m_ac_config0.SetIcon(IDI_LED_BLUEOFF);
	m_sas_config1.SetIcon(IDI_LED_BLUEOFF);
	m_sas_config0.SetIcon(IDI_LED_BLUEOFF);
	gw_config1.SetIcon(IDI_LED_BLUEOFF);
	gw_config0.SetIcon(IDI_LED_BLUEOFF);

	//m_eps_config0.SetIcon(IDI_LED_BLUEOFF);
	//m_eps_config1.SetIcon(IDI_LED_BLUEOFF);
	//m_abs_config0.SetIcon(IDI_LED_BLUEOFF);
	//m_abs_config1.SetIcon(IDI_LED_BLUEOFF);
	//m_epb_config0.SetIcon(IDI_LED_BLUEOFF);
	//m_epb_config1.SetIcon(IDI_LED_BLUEOFF);
	//m_BCM_Rmirror0.SetIcon(IDI_LED_BLUEOFF);
	//m_BCM_Rmirror1.SetIcon(IDI_LED_BLUEOFF);
	//m_Auto_CloseWindow0.SetIcon(IDI_LED_BLUEOFF);
	//m_Auto_CloseWindow1.SetIcon(IDI_LED_BLUEOFF);
	//m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
	//m_cx_3ah.SetIcon(IDI_LED_BLUEOFF);
	//m_cx_FE_3AH.SetIcon(IDI_LED_BLUEOFF);
	//m_rooffj1.SetIcon(IDI_LED_BLUEOFF);
	//m_rooffj0.SetIcon(IDI_LED_BLUEOFF);
	//m_gl.SetIcon(IDI_LED_BLUEOFF);
	//m_gw.SetIcon(IDI_LED_BLUEOFF);
	//m_xc.SetIcon(IDI_LED_BLUEOFF);
	//m_xc_w.SetIcon(IDI_LED_BLUEOFF);
	//m_kt0.SetIcon(IDI_LED_BLUEOFF);
	//m_kt1.SetIcon(IDI_LED_BLUEOFF);
}
void CCANFactorySetDlg::SetFactoryMode(void)
{

	if (ToolModel == 0)
	{
		return ;
	}
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
	if (ToolModel == 1)
	{
		GetDlgItem(IDC_EDIT13)->SetWindowText(SNStr);
		GetDlgItem(IDC_EDIT14)->SetWindowText(SNStr1);
		GetDlgItem(IDC_EDIT1)->SetWindowText(CarNameStr);
		GetDlgItem(IDC_EDIT2)->SetWindowText(CarVINStr);
		GetDlgItem(IDC_EDIT3)->SetWindowText(DiagnoseSNStr);
	}
// 	m_Mode1.SetIcon(IDI_LEDOFF);
// 	m_Mode2.SetIcon(IDI_LEDOFF);
// 	m_Mode3.SetIcon(IDI_LEDOFF);
// 	m_Mode0.SetIcon(IDI_LEDON);
	ToolModel = 0;


	Year.Format(_T("%02d"),t.GetYear());
	Months.Format(_T("%02d"),t.GetMonth());
	Date.Format(_T("%02d"),t.GetDay());
	GetDlgItem(IDC_EDIT4)->SetWindowText(Year);
	GetDlgItem(IDC_EDIT5)->SetWindowText(Months);
	GetDlgItem(IDC_EDIT6)->SetWindowText(Date);
	GetDlgItem(IDC_EDIT7)->SetWindowText(Year);
	GetDlgItem(IDC_EDIT8)->SetWindowText(Months);
	GetDlgItem(IDC_EDIT9)->SetWindowText(Date);
	GetDlgItem(IDC_EDIT10)->SetWindowText(Year);
	GetDlgItem(IDC_EDIT11)->SetWindowText(Months);
	GetDlgItem(IDC_EDIT12)->SetWindowText(Date);
	GetDlgItem(IDC_EDIT15)->SetWindowText(Year);
	GetDlgItem(IDC_EDIT16)->SetWindowText(Months);
	GetDlgItem(IDC_EDIT17)->SetWindowText(Date);

	//GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT11)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT12)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT15)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT16)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT17)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT18)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT19)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT20)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT21)->SetWindowText(L"");



	((CEdit*)GetDlgItem(IDC_EDIT4))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT5))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT6))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT7))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT8))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT9))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT10))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT11))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT12))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT15))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT16))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT17))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT18))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT19))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT20))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT21))->SetReadOnly(TRUE);

	// 	 memset(CarNameData,0,8);
	// 	 memset(CarVINData,0,17);
	// 	 memset(DiagnoseSNData,0,16);
	// 	 memset(ECUMakeDayData,0,4);
	// 	 memset(ECUSWDayData,0,4);
	// 	 memset(ECUConfigDayData,0,4);
	TuBiaoys();
	QHInit();

	/*     memset(&SNData[16],0,8);*/
	// 	CRect rect;
	// 	GetDlgItem(IDC_STATIC23)->GetWindowRect(rect);
	// 	this->ScreenToClient(rect);
	// 	GetDlgItem(IDC_STATIC23)->SetWindowText(_T("工厂模式"));
	// 	InvalidateRect(&rect);

	CRect rect;
	for (UINT i = IDC_STATIC24;i<=IDC_STATIC25;i++)
	{
		GetDlgItem(i)->GetWindowRect(rect);
		this->ScreenToClient(rect);
		GetDlgItem(i)->SetWindowText(L"");
		InvalidateRect(&rect);
	}
/*	GetDlgItem(IDC_STATIC23)->SetWindowText(L"工厂模式");*/
	GetDlgItem(IDC_STATIC24)->SetWindowText(L"250060");
	GetDlgItem(IDC_STATIC25)->SetWindowText(L"0011702140");

	// 	InvalidateRect(&rect);

}

void CCANFactorySetDlg::SetReadMode(void)
{
	GetDlgItem(IDC_EDIT1)->GetWindowText(CarNameStr);
	GetDlgItem(IDC_EDIT2)->GetWindowText(CarVINStr);
	GetDlgItem(IDC_EDIT3)->GetWindowText(DiagnoseSNStr);
	GetDlgItem(IDC_EDIT13)->GetWindowText(SNStr);
	GetDlgItem(IDC_EDIT14)->GetWindowText(SNStr1);
	GetDlgItem(IDC_EDIT15)->GetWindowText(SNStr2);
	GetDlgItem(IDC_EDIT16)->GetWindowText(SNStr3);
	GetDlgItem(IDC_EDIT17)->GetWindowText(SNStr4);
	GetDlgItem(IDC_EDIT4)->GetWindowText(ECUMakeDayStr0);
	GetDlgItem(IDC_EDIT5)->GetWindowText(ECUMakeDayStr1);
	GetDlgItem(IDC_EDIT6)->GetWindowText(ECUMakeDayStr2);
	GetDlgItem(IDC_EDIT7)->GetWindowText(ECUSWDayStr0);
	GetDlgItem(IDC_EDIT8)->GetWindowText(ECUSWDayStr1);
	GetDlgItem(IDC_EDIT9)->GetWindowText(ECUSWDayStr1);
	GetDlgItem(IDC_EDIT10)->GetWindowText(ECUSWDayStr1);
	GetDlgItem(IDC_EDIT11)->GetWindowText(ECUSWDayStr1);
	GetDlgItem(IDC_EDIT12)->GetWindowText(ECUSWDayStr1);
	GetDlgItem(IDC_BUTTON6)->ShowWindow(SW_HIDE);
	if (ToolModel == 1)
	{
		return ;
	}
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
// 	m_Mode0.SetIcon(IDI_LEDOFF);
// 	m_Mode2.SetIcon(IDI_LEDOFF);
// 	m_Mode3.SetIcon(IDI_LEDOFF);
// 	m_Mode1.SetIcon(IDI_LEDON);
	ToolModel = 1;

	//GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT6)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT11)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT12)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT15)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT16)->EnableWindow(TRUE);
	//GetDlgItem(IDC_EDIT17)->EnableWindow(TRUE);


	((CEdit*)GetDlgItem(IDC_EDIT4))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT5))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT6))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT7))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT8))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT9))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT10))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT11))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT12))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT15))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT16))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT17))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT18))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT19))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT20))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT21))->SetReadOnly(TRUE);

	GetDlgItem(IDC_EDIT4)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT5)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT6)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT7)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT8)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT9)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT10)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT11)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT12)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT15)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT16)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT17)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT13)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT14)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT2)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT3)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT1)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT18)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT19)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT20)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT21)->SetWindowText(L"");
	TuBiaohs();

	CRect rect;
	for (UINT i = IDC_STATIC24;i<=IDC_STATIC25;i++)
	{
		GetDlgItem(i)->GetWindowRect(rect);
		this->ScreenToClient(rect);
		GetDlgItem(i)->SetWindowText(L"");
		InvalidateRect(&rect);
	}
/*	GetDlgItem(IDC_STATIC23)->SetWindowText(L"读模式");*/
	// 	GetDlgItem(IDC_STATIC24)->SetWindowText(L"");
	// 	GetDlgItem(IDC_STATIC25)->SetWindowText(L"");
	// 	InvalidateRect(&rect);
}

void CCANFactorySetDlg::SetQAMode(void)
{

	GetDlgItem(IDC_EDIT4)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT5)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT6)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT7)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT8)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT9)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT10)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT11)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT12)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT15)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT16)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT17)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT13)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT14)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT2)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT3)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT1)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT18)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT19)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT20)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT21)->SetWindowText(L"");

	if (ToolModel == 3)
	{
		return ;
	}
// 	m_Mode3.SetIcon(IDI_LEDON);
// 	m_Mode0.SetIcon(IDI_LEDOFF);
// 	m_Mode1.SetIcon(IDI_LEDOFF);
// 	m_Mode2.SetIcon(IDI_LEDOFF);
	ToolModel = 3;
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	m_Config.SetCurSel(0);
	TuBiaoys();
	QHInit();
	ErrorStr.Empty();
	CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
	//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}

void CCANFactorySetDlg::SetTestMode(void)
{
	if (ToolModel == 1)
	{
		GetDlgItem(IDC_EDIT1)->SetWindowText(CarNameStr);
		GetDlgItem(IDC_EDIT2)->SetWindowText(CarVINStr);
		GetDlgItem(IDC_EDIT3)->SetWindowText(DiagnoseSNStr);
		GetDlgItem(IDC_EDIT4)->SetWindowText(ECUMakeDayStr0);
		GetDlgItem(IDC_EDIT5)->SetWindowText(ECUMakeDayStr1);
		GetDlgItem(IDC_EDIT6)->SetWindowText(ECUMakeDayStr2);
		GetDlgItem(IDC_EDIT7)->SetWindowText(ECUSWDayStr0);
		GetDlgItem(IDC_EDIT8)->SetWindowText(ECUSWDayStr1);
		GetDlgItem(IDC_EDIT9)->SetWindowText(ECUSWDayStr1);
		GetDlgItem(IDC_EDIT10)->SetWindowText(ECUSWDayStr1);
		GetDlgItem(IDC_EDIT11)->SetWindowText(ECUSWDayStr1);
		GetDlgItem(IDC_EDIT12)->SetWindowText(ECUSWDayStr1);
		GetDlgItem(IDC_EDIT13)->SetWindowText(SNStr);
		GetDlgItem(IDC_EDIT14)->SetWindowText(SNStr1);
		GetDlgItem(IDC_EDIT15)->SetWindowText(SNStr2);
		GetDlgItem(IDC_EDIT16)->SetWindowText(SNStr3);
		GetDlgItem(IDC_EDIT17)->SetWindowText(SNStr4);
	}
	if (ToolModel == 2)
	{
		return ;
	}
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
// 	m_Mode0.SetIcon(IDI_LEDOFF);
// 	m_Mode1.SetIcon(IDI_LEDOFF);
// 	m_Mode3.SetIcon(IDI_LEDOFF);
// 	m_Mode2.SetIcon(IDI_LEDON);
	ToolModel = 2;

	// 	Year.Format(_T("%d"),t.GetYear());
	// 	Months.Format(_T("%d"),t.GetMonth());
	// 	Date.Format(_T("%d"),t.GetDay());
	// 	GetDlgItem(IDC_EDIT4)->SetWindowText(Year);
	// 	GetDlgItem(IDC_EDIT5)->SetWindowText(Months);
	// 	GetDlgItem(IDC_EDIT6)->SetWindowText(Date);
	// 	GetDlgItem(IDC_EDIT7)->SetWindowText(Year);
	// 	GetDlgItem(IDC_EDIT8)->SetWindowText(Months);
	// 	GetDlgItem(IDC_EDIT9)->SetWindowText(Date);
	// 	GetDlgItem(IDC_EDIT10)->SetWindowText(Year);
	// 	GetDlgItem(IDC_EDIT11)->SetWindowText(Months);
	// 	GetDlgItem(IDC_EDIT12)->SetWindowText(Date);
	// 	GetDlgItem(IDC_EDIT15)->SetWindowText(Year);
	// 	GetDlgItem(IDC_EDIT16)->SetWindowText(Months);
	// 	GetDlgItem(IDC_EDIT17)->SetWindowText(Date);

	GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT6)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT11)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT12)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT15)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT16)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT17)->EnableWindow(TRUE);
	    TuBiaohs();
	// 	CRect rect;
	// 	GetDlgItem(IDC_STATIC23)->GetWindowRect(rect);
	// 	this->ScreenToClient(rect);
	// 	GetDlgItem(IDC_STATIC23)->SetWindowText(L"测试模式");
	// 	InvalidateRect(&rect);

	CRect rect;
	for (UINT i = IDC_STATIC24;i<=IDC_STATIC25;i++)
	{
		GetDlgItem(i)->GetWindowRect(rect);
		this->ScreenToClient(rect);
		GetDlgItem(i)->SetWindowText(L"");
		InvalidateRect(&rect);
	}
/*	GetDlgItem(IDC_STATIC23)->SetWindowText(L"测试模式");*/
	GetDlgItem(IDC_STATIC24)->SetWindowText(L"250060");
	GetDlgItem(IDC_STATIC25)->SetWindowText(L"0011702140");

}

void CCANFactorySetDlg::SendShowData(CString Str)
{
/*	::PostMessage(MainHwnd,WM_SUB_MSG,1,(LPARAM)(&Str));*/
	::SendMessage(MainHwnd,WM_SUB_MSG,1,(LPARAM)(&Str));
}

void CCANFactorySetDlg::SetDlgTrue()
{
	this->EnableWindow(TRUE);
}

//void CCANFactorySetDlg::GetEditText()
//{
//	CString str;
//	GetDlgItem(IDC_EDIT23)->GetWindowText(str);
//	if (str.IsEmpty())
//	{
//		return;
//	}
//	
//	Honeywelltest=str;
//
//}
void CCANFactorySetDlg::dataconfig(void)
{
#if 0
	if (ToolModel == 1)
	{
		return ;
	}
	UINT Len = 0;
	CString Str;
	m_Edit_CarName.GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请输入车辆名称"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarNameData,Str.GetLength(),NULL,NULL);
	m_Edit_CarVIN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarVINData,Str.GetLength(),NULL,NULL);
	m_Edit_DiagnoseSN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)DiagnoseSNData,Str.GetLength(),NULL,NULL);
	ECUMakeDate();
	ECUSWDate();
	ECUConfigDate();

	GetDlgItem(IDC_EDIT18)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)SWVersion,Str.GetLength(),NULL,NULL);
	ByteStuffing(SWVersion,(Str.GetLength()-1),21);
	GetDlgItem(IDC_EDIT19)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)HWVersion,Str.GetLength(),NULL,NULL);
	ByteStuffing(HWVersion,(Str.GetLength()-1),16);

	GetDlgItem(IDC_EDIT20)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置零件号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)PartNumberBuf,Str.GetLength(),NULL,NULL);
	GetDlgItem(IDC_EDIT21)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置供应商识别号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)SystemSupplierIdBuf,Str.GetLength(),NULL,NULL);
	if (!GetSNDate())
	{
		return ;
	}

	CCANSend::Multi_Frame(TestID,0x2E,0xA0,NULL,CarNameData,sizeof(CarNameData));
	//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
#else
	if (ToolModel == 1)
	{
		return ;
	}
	UINT Len = 0;
	CString Str;
	m_Edit_CarName.GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请输入车辆名称"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarNameData,Str.GetLength(),NULL,NULL);
	m_Edit_CarVIN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarVINData,Str.GetLength(),NULL,NULL);
	m_Edit_DiagnoseSN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)DiagnoseSNData,Str.GetLength(),NULL,NULL);
	ECUMakeDate();
	ECUSWDate();
	ECUConfigDate();

	GetDlgItem(IDC_EDIT18)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)SWVersion,Str.GetLength(),NULL,NULL);
	ByteStuffing(SWVersion,(Str.GetLength()),21);
	GetDlgItem(IDC_EDIT19)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)HWVersion,Str.GetLength(),NULL,NULL);
	ByteStuffing(HWVersion,(Str.GetLength()),16);

	GetDlgItem(IDC_EDIT20)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置零件号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)PartNumberBuf,Str.GetLength(),NULL,NULL);
	GetDlgItem(IDC_EDIT21)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置供应商识别号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)SystemSupplierIdBuf,Str.GetLength(),NULL,NULL);
	if (!GetSNDate())
	{
		return ;
	}

	CCANSend::Multi_Frame(TestID,0x2E,0xA0,NULL,CarNameData,sizeof(CarNameData));
	//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
#endif
}

void CCANFactorySetDlg::Dataread(void)
{
	
	strtoarray();
	CString FilePathName;
	CString GEELY_FE_3AH;
	CString ReadStr,ReadS,Str;
	UINT Index,Len,count = 0;
	char CRead[100];

	CFile file;

	HMODULE module = GetModuleHandle(0);   
	TCHAR pFileName[MAX_PATH];   
	GetModuleFileName(module, pFileName, MAX_PATH);   

	CString csFullPath(pFileName);   
	int nPos = csFullPath.ReverseFind( _T('\\') ); 
	csFullPath = csFullPath.Left(nPos);

	CString str1=L"\\GEELY_FE_3AH.ini";
	csFullPath=csFullPath + str1;


	FilePathName=csFullPath;


//	CFileDialog filedlg(TRUE,L".ini",L"GEELY_FE_3AH", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,L"configure File(*.ini)|*.ini||");
	/*	filedlg.m_ofn.lpstrInitialDir = L"\\";*/
#if 0
		TRY 
		{
			CStdioFile file;
			file.Open(FilePathName,CFile::modeRead|CFile::typeBinary);

			while(file.ReadString(Str))
			{
				Index= 0;
				ReadStr.Empty();
				Len=Str.GetLength();
				if (count==5)
					break;
				for (UINT n=0;n<Len;n++)
				{
					CRead[n]=(char)Str.GetAt(n);

					if (Index)
					{
						ReadS.Format(L"%c",CRead[n]);
						ReadStr+=ReadS;
					}
					if (CRead[n] == 0x1A)
					{
						Index = n;
					}

				}

				if (count == 1)
				{
					GetDlgItem(IDC_EDIT18)->SetWindowText(ReadStr);
				}else if (count == 2)
				{
					GetDlgItem(IDC_EDIT19)->SetWindowText(ReadStr);
				}else if (count == 3)
				{
					GetDlgItem(IDC_EDIT20)->SetWindowText(ReadStr);
				}else if (count == 4)
				{
					GetDlgItem(IDC_EDIT21)->SetWindowText(ReadStr);
				}

				count++;
			}
			
		}
		CATCH (CMemoryException, e)
		{
			e->ReportError();
			e->Delete();
		}
		END_CATCH
#else
	loadConfig(FilePathName);
#endif	
}



void CCANFactorySetDlg::savedata()
{
	CString Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12,Str13,Str14,Str15,Str16,Str17,Str18,Str19,Str20,Str21,Str22,Str23,Str24,Str25,Str26,Str27,Str28,Str29,Str30,Str31,Str32,Str33,Str34,Str35,Str36,Str37,Str38,Str39,Str40,Str41,Str42,Str43,Str44,Str45,Str46,Str47,errorwname;
	CString Str01,Str02,Str03,Str04,Str05,Str06,Str07,Str08,Str09,Str010,Str011,Str012,Str013,Str014,Str015,Str016,Str017,Str018,Str019,Str020,Str021,Str022,Str023,Str024,Str025,Str026,Str027,Str028,Str029,Str030,Str031,Str032,Str033,Str034,Str035;

	CString str111,str112,str113,str114,str115,str116,str117,str118,str119,str120,str121;
	CString str211,str212,str213,str214;

	//截取条形码第2和第9-12个字符串
	CString STR1,STR2,STR3,STR4,STR5;
	BYTE m_byte[1]={0},m_byte2[5]={0,0,0,0,0};
	Str13=Honeywelltest.GetAt(2);
	STR1=Honeywelltest.GetAt(9);
	STR2=Honeywelltest.GetAt(10);
	STR3=Honeywelltest.GetAt(11);
	STR4=Honeywelltest.GetAt(12);
	STR5='0';
	
	Str14=STR1+STR2+STR3+STR4;
	GetDlgItem(IDC_EDIT13)->SetWindowText(Str13);
	

	

	WideCharToMultiByte(CP_ACP,0,Str13,Str13.GetLength(),(LPSTR)m_byte,Str13.GetLength(),NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,Str14,Str14.GetLength(),(LPSTR)m_byte2,Str14.GetLength(),NULL,NULL);
	SNData[19]=m_byte[0];

	
	iSum=_ttoi(Str14);
	SNData[23] = iSum & 0xff;
	SNData[22] = (iSum>>8)&0xff;
	SNData[21] = (iSum>>16)&0xff;
	SNData[20] = (iSum>>24)&0xff;
	//Str14=STR5+Str14;
	GetDlgItem(IDC_EDIT14)->SetWindowText(Str14);
	MessageBox(_T("正在保存"),_T("提示"),MB_ICONQUESTION);
		CString FilePathName;
	//	CFileDialog filedlg(FALSE,L"txt",Honeywelltest,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"All Files (*.txt*)|*.txt*||");
		//if(filedlg.DoModal()==IDOK)
		//{
		//	FilePathName=filedlg.GetPathName();
			//CString Save,Save1;
			GetSNDate();
			strtoarray();






			GetDlgItemText(IDC_EDIT1,Str1);
			GetDlgItemText(IDC_EDIT2,Str2);
			GetDlgItemText(IDC_EDIT3,Str3);
			GetDlgItemText(IDC_EDIT4,Str4);
			GetDlgItemText(IDC_EDIT5,Str5);
			GetDlgItemText(IDC_EDIT6,Str6);
			GetDlgItemText(IDC_EDIT7,Str7);
			GetDlgItemText(IDC_EDIT8,Str8);
			GetDlgItemText(IDC_EDIT9,Str9);
			GetDlgItemText(IDC_EDIT10,Str10);
			GetDlgItemText(IDC_EDIT11,Str11);
			GetDlgItemText(IDC_EDIT12,Str12);

			GetDlgItemText(IDC_EDIT13,Str13);
			GetDlgItemText(IDC_EDIT14,Str14);

			GetDlgItemText(IDC_EDIT15,Str15);
			GetDlgItemText(IDC_EDIT16,Str16);
			GetDlgItemText(IDC_EDIT17,Str17);
			GetDlgItemText(IDC_EDIT18,Str18);
			GetDlgItemText(IDC_EDIT19,Str19);
			GetDlgItemText(IDC_EDIT20,Str20);
			GetDlgItemText(IDC_EDIT21,Str21);
			Str22=SNData;
			
			Str04="ECU制造日期：";
			Str05.Replace(L"\r\n",L"");
			Str06.Replace(L"\r\n",L"");
			Str07="软件刷新日期:";
			Str08.Replace(L"\r\n",L"");
			Str09.Replace(L"\r\n",L"");
			Str010="ECU装配日期:";
			Str011.Replace(L"\r\n",L"");
			Str012.Replace(L"\r\n",L"");
			Str013="产线编号：";
			Str014="产品序列号：";
			Str015="机器序列号日期：";
			Str016.Replace(L"\r\n",L"");
			Str017.Replace(L"\r\n",L"");
			Str018="ECU软件版本号：";
			Str019="ECU硬件版本号：";
			Str020="零件号:";
			Str021="供应商识别号:";
			Str01="车辆名称:";
			Str02="车辆VIN码:";
			Str03="诊断仪序列号:";
			Str022="机器序列号:";
			Str45="机器序列号：2500600011702140";
			Str45+=Str15;
			Str45+=Str16;
			Str45+=Str17;
			Str45+=Str13;
			Str45+=Str14;
			Str45+=L"\r\n";

			Str18+= L"\r\n";
			Str19+= L"\r\n";
			Str20+= L"\r\n";
			Str21+= L"\r\n";
			Str1+= L"\r\n";
			Str2+= L"\r\n";
			Str3+= L"\r\n";
			Str6+=L"日\r\n";
			Str9+=L"日\r\n";
			Str12+=L"日\r\n";
			Str17+=L"日\r\n";
			Str13+=L"\r\n";
			Str14+=L"\r\n";
			Str04+=L"";
			Str05+=L"年";
			Str06+=L"月";
			Str07+=L"";
			Str08+=L"年";
			Str09+=L"月";
			Str010+=L"";
			Str011+=L"年";
			Str012+=L"月";
			Str015+=L"";
			Str016+=L"年";
			Str017+=L"月";
			Str22+=L"\r\n";

			Str01+=Str1;
			Str02+=Str2;
			Str03+=Str3;
			Str04+=Str4;
			Str05+=Str5;
			Str06+=Str6;
			Str07+=Str7;
			Str08+=Str8;
			Str09+=Str9;
			Str010+=Str10;
			Str011+=Str11;
			Str012+=Str12;
			Str013+=Str13;
			Str014+=Str14;
			Str015+=Str15;
			Str016+=Str16;
			Str017+=Str17;
			Str018+=Str18;
			Str019+=Str19;
			Str020+=Str20;
			Str021+=Str21;
			Str022+=Str22;
			Str023="[工厂设置]\r\n";
			Str030="\r<未比对错误信息>\r\n";
			Str44="<比对错误信息>\r\n";
			Str31=ErrorStr;
			Str31+="\r\n";
		
			////字节0，第7位EPS
			//if ((NetWorkConfigData[0]&0x80) == 0x80)
			//{
			//	str114=L"EPS配置：EPS配置\r\n";
			//}else if((NetWorkConfigData[0]&0x80) == 0x00){
			//	str114=L"EPS配置：EPS不配置\r\n";
			//}
			////字节0，第6位PEPS
			//if ((NetWorkConfigData[0]&0x40) == 0x40)
			//{
			//	Str024=L"PEPS配置：PEPS组装\r\n";
			//}else if((NetWorkConfigData[0]&0x40) == 0x00){
			//	Str024=L"PEPS配置：PEPS不组装\r\n";
			//}

			////字节0，第5位ESC
			//if ((NetWorkConfigData[0]&0x20) == 0x20)
			//{
			//	Str026=L"ESC配置：ESC配置\r\n";
			//}else if((NetWorkConfigData[0]&0x20) == 0x00){
			//	Str026=L"ESC配置：ESC不配置\r\n";
			//}
			////字节0，第4位ACU
			//if ((NetWorkConfigData[0]&0x10) == 0x10)
			//{
			//	str113=L"ACU配置：ACU配置\r\n";
			//}else if((NetWorkConfigData[0]&0x10) == 0x00){
			//	str113=L"ACU配置：ACU不配置\r\n";
			//}
			////字节0，第3位TCU
			//if ((NetWorkConfigData[0]&0x08) == 0x08)
			//{
			//	str112=L"TCU配置：TCU配置\r\n";
			//}else if((NetWorkConfigData[0]&0x08) == 0x00){
			//	str112=L"TCU配置：TCU不配置\r\n";
			//}
			////字节0，第2位BCM
			//if ((NetWorkConfigData[0]&0x04) == 0x04)
			//{
			//	str111=L"BCM配置：BCM配置\r\n";
			//}else if((NetWorkConfigData[0]&0x04) == 0x00){
			//	str111=L"BCM配置：BCM不配置\r\n";
			//}
			////字节0，第1位IPK
			//if ((NetWorkConfigData[0]&0x02) == 0x02)
			//{
			//	str116=L"IPK配置：IPK配置\r\n";
			//}else if((NetWorkConfigData[0]&0x02) == 0x00){
			//	str116=L"IPK配置：IPK不配置\r\n";
			//}
			////字节0，第0位ems
			//if ((NetWorkConfigData[0]&0x01) == 0x01)
			//{
			//	str115=L"EMS配置：EMS配置\r\n";
			//}else if((NetWorkConfigData[0]&0x01) == 0x00){
			//	str115=L"EMS配置：EMS不配置\r\n";
			//}
			////字节1，第5位TPMS
			//if ((NetWorkConfigData[1]&0x20) == 0x20)
			//{
			//	Str025=L"TPMS配置：TPMS配置\r\n";
			//}else if ((NetWorkConfigData[1]&0x20) == 0x00){
			//	Str025=L"TPMS配置：TPMS不配置\r\n";
			//}
			////字节1，第3位MMI
			//if ((NetWorkConfigData[1]&0x08) == 0x08)
			//{
			//	str117=L"MMI配置：MMI配置\r\n";
			//}else if ((NetWorkConfigData[1]&0x08) == 0x00){
			//	str117=L"MMI配置：MMI不配置\r\n";
			//}
			////字节1，第2位AC
			//if ((NetWorkConfigData[1]&0x04) == 0x04)
			//{
			//	str118=L"AC配置：AC配置\r\n";
			//}else if ((NetWorkConfigData[1]&0x04) == 0x00){
			//	str118=L"AC配置：AC不配置\r\n";
			//}
			////字节1，第0位SAS
			//if ((NetWorkConfigData[1]&0x01) == 0x01)
			//{
			//	str119=L"SAS配置：SAS配置\r\n";
			//}else if ((NetWorkConfigData[1]&0x01) == 0x00){
			//	str119=L"SAS配置：SAS不配置\r\n";
			//}
			////字节2，第5位ABS
			//if ((NetWorkConfigData[2]&0x20) == 0x20)			
			//{
			//	str120=L"ABS配置：ABS配置\r\n";
			//}else if ((NetWorkConfigData[2]&0x20) == 0x00){
			//	str120=L"ABS配置：ABS不配置\r\n";
			//}
			////字节2，第0位EPB
			//if ((NetWorkConfigData[2]&0x01) == 0x01)			
			//{
			//	str121=L"EPB配置：EPB配置\r\n";
			//}else if ((NetWorkConfigData[2]&0x01) == 0x00){
			//	str121=L"EPB配置：EPB不配置\r\n";
			//}

			////字节0，第7位BCM后视镜折叠状态
			//if ((MMIConfigData[0]&0x80) == 0x80)
			//{
			//	Str027=L"BCM后视镜：有后视镜折叠\r\n";
			//}else{
			//	Str027=L"BCM后视镜：无后视镜折叠\r\n";
			//}
			////字节0，第3位全景天窗防夹
			//if ((MMIConfigData[0]&0x08) == 0x08)
			//{
			//	str211=L"全景天窗防夹：有\r\n";
			//}else{
			//	str211=L"全景天窗防夹：无\r\n";
			//}
			////字节0，第2位国五/国六
			//if ((MMIConfigData[0]&0x04) == 0x04)
			//{
			//	str212=L"国五/国六：国六\r\n";
			//}else{
			//	str212=L"国五/国六：国五\r\n";
			//}
			////字节0，第0位行车记录仪
			//if ((MMIConfigData[0]&0x01) == 0x01)
			//{
			//	str213=L"行车记录仪：有\r\n";
			//}else{
			//	str213=L"行车记录仪：无\r\n";
			//}
			////字节1，第7位自动关窗功能
			//if ((MMIConfigData[1]&0x80) == 0x80)
			//{
			//	Str028=L"自动关窗：自动关窗有\r\n";
			//}else{
			//	Str028=L"自动关窗：自动关窗无\r\n";
			//}
			////字节0，第4,5,6位车辆类型
			//if ((MMIConfigData[0]&0x70) == 0x20){//5位为1，4位为0值10
			//	str214=L"车辆类型：FE-FE_3AH\r\n";
			//}else if ((MMIConfigData[0]&0x70) == 0x10){//5位为0，4位为1值01
			//	str214=L"车辆类型：FE-3AH\r\n";
			//}else if ((MMIConfigData[0]&0x70) == 0x00){//5位为0，4位为0值00
			//	str214=L"车辆类型：无匹配车型\r\n";
			//}

			////字节1，第2位空调
			//if ((MMIConfigData[1]&0x04) == 0x04)
			//{
			//	Str028+=L"空调：有\r\n";
			//}else{
			//	Str028+=L"空调：无\r\n";
			//}

			if (ErrorFlg == 0x01)
			{
				//	MessageBox(L"数据发送失败，请检查设备是否连接",L"提醒");
			}else if (ErrorFlg == 0x11){
				Str31=L"服务ID不支持！\r\n";
			}else if (ErrorFlg == 0x12){
				Str32=L"DID不支持！\r\n";
			}else if (ErrorFlg == 0x13){
				Str33=L"报文长度或数据格式错误!\r\n";
			}else if (ErrorFlg == 0x31){
				Str34=L"请求设置的数值超出范围！\r\n";
			}else if (ErrorFlg == 0x32){
				Str35=L"请求数值无效！\r\n";
			}else if (ErrorFlg == 0x35){
				Str36=L"无效的解锁钥匙！\r\n";
			}else if (ErrorFlg == 0x40){
				Str37=L"DID Command 不支持！\r\n";
			}else if (ErrorFlg == 0x41){
				Str37=L"请求设置的功能DVD未完成！\r\n";
			}else if (ErrorFlg == 0x80){
				Str38=L"不在测试模式！\r\n";
			}else if (ErrorFlg == 0x71){
				Str39=L"报文类型错误！\r\n";
			}else if (ErrorFlg == 0x72){
				Str40=L"报文SN错误！\r\n";
			}else if (ErrorFlg == 0x73){
				Str41=L"报文时间超时！\r\n";
			}else if (ErrorFlg == 0x74){
				Str42=L"接收报文长度错误!\r\n";
			}else if (ErrorFlg == 0x75){
				Str43=L"填充字节错误！\r\n";
			}
			if (OverTime==500)
			{
				OverTime=0;
				Str47=L"无应答信号，设置超时!\r\n";
			}

			TRY 
			{
				//FilePathName= "D:\\Release\\";

				HMODULE module = GetModuleHandle(0);   
				TCHAR pFileName[MAX_PATH];   
				GetModuleFileName(module, pFileName, MAX_PATH);   

				CString csFullPath(pFileName);   
				int nPos = csFullPath.ReverseFind( _T('\\') ); 
				csFullPath = csFullPath.Left(nPos);

				CString str1=L"\\日志\\";
				csFullPath=csFullPath + str1;

				FilePathName = csFullPath;

				FilePathName+=Honeywelltest;
				Str46=".txt";
				FilePathName+=Str46;
				CFile file(FilePathName, CFile::modeCreate | CFile::modeWrite);
				file.SeekToBegin();//到达文件开头   
				//file.Write("\xff\xfe", 2); 
				//file.Write(Save,Save.GetLength()*2);
				
				file.Write("\xff\xfe", 2);
				file.Write(Str023,Str023.GetLength()*2);
				file.Write(Str024,Str024.GetLength()*2);
				file.Write(Str025,Str025.GetLength()*2);
				file.Write(Str026,Str026.GetLength()*2);
				file.Write(Str027,Str027.GetLength()*2);
				file.Write(Str028,Str028.GetLength()*2);
				////////////////////////////////////
				file.Write(str111,str111.GetLength()*2);
				file.Write(str112,str112.GetLength()*2);
				file.Write(str113,str113.GetLength()*2);
				file.Write(str114,str114.GetLength()*2);
				file.Write(str115,str115.GetLength()*2);
				file.Write(str116,str116.GetLength()*2);
				file.Write(str117,str117.GetLength()*2);
				file.Write(str118,str118.GetLength()*2);
				file.Write(str119,str119.GetLength()*2);
				file.Write(str120,str120.GetLength()*2);
				file.Write(str121,str121.GetLength()*2);
				////////////////////////////////////
				file.Write(str211,str211.GetLength()*2);
				file.Write(str212,str212.GetLength()*2);
				file.Write(str213,str213.GetLength()*2);
				file.Write(str214,str214.GetLength()*2);
				//file.Write(Str029,Str029.GetLength()*2);
				//file.Write(Str01,Str01.GetLength()*2);
				file.Write(Str02,Str02.GetLength()*2);
				file.Write(Str03,Str03.GetLength()*2);
				file.Write(Str04,Str04.GetLength()*2);
				file.Write(Str05,Str05.GetLength()*2);
				file.Write(Str06,Str06.GetLength()*2);
				file.Write(Str07,Str07.GetLength()*2);
				file.Write(Str08,Str08.GetLength()*2);
				file.Write(Str09,Str09.GetLength()*2);
				file.Write(Str010,Str010.GetLength()*2);
				file.Write(Str011,Str011.GetLength()*2);
				file.Write(Str012,Str012.GetLength()*2);
				file.Write(Str013,Str013.GetLength()*2);
				file.Write(Str014,Str014.GetLength()*2);
				file.Write(Str015,Str015.GetLength()*2);
				file.Write(Str016,Str016.GetLength()*2);
				file.Write(Str017,Str017.GetLength()*2);
				file.Write(Str018,Str018.GetLength()*2);
				file.Write(Str019,Str019.GetLength()*2);
				file.Write(Str020,Str020.GetLength()*2);
				file.Write(Str021,Str021.GetLength()*2);
				file.Write(Str022,Str022.GetLength()*2);
				file.Write(Str45,Str45.GetLength()*2);

				file.Write(Str030,Str030.GetLength()*2);
				file.Write(Str47,Str47.GetLength()*2);
				file.Write(Str32,Str32.GetLength()*2);
				file.Write(Str33,Str33.GetLength()*2);
				file.Write(Str34,Str34.GetLength()*2);
				file.Write(Str35,Str35.GetLength()*2);
				file.Write(Str36,Str36.GetLength()*2);
				file.Write(Str37,Str37.GetLength()*2);
				file.Write(Str38,Str38.GetLength()*2);
				file.Write(Str39,Str39.GetLength()*2);
				file.Write(Str40,Str40.GetLength()*2);
				file.Write(Str41,Str41.GetLength()*2);
				file.Write(Str42,Str42.GetLength()*2);
				file.Write(Str43,Str43.GetLength()*2);
				file.Write(Str44,Str44.GetLength()*2);
				file.Write(Str31,Str31.GetLength()*2);
				
				file.Close();
				
				//GetDlgItem(IDC_EDIT23)->SetWindowText(L"");
			}
			CATCH (CMemoryException, e)
			{
				e->ReportError();
				e->Delete();
			}
			END_CATCH
				
		//}
		
		 if (FALSE == ErrorStr.IsEmpty()||ErrorFlg!=0)//如果有错误文件名后加Error
		 {

			 // pNewName= "D:\\Release\\";
			  HMODULE module = GetModuleHandle(0);   
			  TCHAR pFileName[MAX_PATH];   
			  GetModuleFileName(module, pFileName, MAX_PATH);   

			  CString csFullPath(pFileName);   
			  int nPos = csFullPath.ReverseFind( _T('\\') ); 
			  csFullPath = csFullPath.Left(nPos);

			  CString str1=L"\\日志\\";
			  csFullPath=csFullPath + str1;

			  pNewName=csFullPath;


	pNewName+=Honeywelltest;
			  errorwname="---Error";
			  Str46=".txt";
			  pNewName+=errorwname;
			  pNewName+=Str46;
			  TRY 
			  {
				  DeleteFile(pNewName);
				  CFile::Rename(FilePathName,pNewName);
			  }
			  CATCH (CMemoryException, e)
			  {
				  
				  e->ReportError();
				  e->Delete();
			  }
			  END_CATCH
		 }else{
			 pNewName=FilePathName;
		 }
		

		ErrorFlg = 0;
		ERRORSTR=ErrorStr;
		ErrorStr.Empty();
		
	}

	
	void CCANFactorySetDlg::Honeywell_judgment()
	{
		
	}

void CCANFactorySetDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	__super::OnChar(nChar, nRepCnt, nFlags);
}

void CCANFactorySetDlg::strtoarray()
{

}

void CCANFactorySetDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CCANFactorySetDlg::OnBnClickedButton30()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] &= 0xF7;
	m_tcu_config0.SetIcon(IDI_LEDON);
	m_tcu_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton31()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] |= 0x08;
	m_tcu_config1.SetIcon(IDI_LEDON);
	m_tcu_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}

}


void CCANFactorySetDlg::OnBnClickedButton32()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] &= 0xEF;
	m_acu_config0.SetIcon(IDI_LEDON);
	m_acu_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton33()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] |= 0x10;
	m_acu_config1.SetIcon(IDI_LEDON);
	m_acu_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton34()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] &= 0xDF;
	m_esc_config0.SetIcon(IDI_LEDON);
	m_esc_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton35()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] |= 0x20;
	m_esc_config1.SetIcon(IDI_LEDON);
	m_esc_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton36()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] &= 0x7F;
	m_eps_config0.SetIcon(IDI_LEDON);
	m_eps_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton37()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] |= 0x80;				//20190413: Modified by liufeifei
	m_eps_config1.SetIcon(IDI_LEDON);
	m_eps_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton22()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] &= 0xFE;
	m_ems_config0.SetIcon(IDI_LEDON);
	m_ems_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton23()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] |= 0x01;
	m_ems_config1.SetIcon(IDI_LEDON);
	m_ems_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton24()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] &= 0xFD;
	m_ipk_config0.SetIcon(IDI_LEDON);
	m_ipk_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton27()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[0] |= 0x02;	//20190413: Modified by liufeifei
	m_ipk_config1.SetIcon(IDI_LEDON);
	m_ipk_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton38()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[1] &= 0xF7;
	m_mmi_config0.SetIcon(IDI_LEDON);
	m_mmi_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton39()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[1] |= 0x08;
	m_mmi_config1.SetIcon(IDI_LEDON);
	m_mmi_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton40()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[1] &= 0xFB;
	m_ac_config0.SetIcon(IDI_LEDON);
	m_ac_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton41()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[1] |= 0x04;	//20190413: Modified by liufeifei
	m_ac_config1.SetIcon(IDI_LEDON);
	m_ac_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton42()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[1] &= 0xFE;
	m_sas_config0.SetIcon(IDI_LEDON);
	m_sas_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton43()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[1] |= 0x01;	//20190413: Modified by liufeifei
	m_sas_config1.SetIcon(IDI_LEDON);
	m_sas_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton44()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[2] &= 0xDF;
	m_abs_config0.SetIcon(IDI_LEDON);
	m_abs_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton45()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[2] |= 0x20;
	m_abs_config1.SetIcon(IDI_LEDON);
	m_abs_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton46()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[2] &= 0xFE;
	m_epb_config0.SetIcon(IDI_LEDON);
	m_epb_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton47()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[2] |= 0x01;
	m_epb_config1.SetIcon(IDI_LEDON);
	m_epb_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton48()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] = (MMIConfigData[0]&0x8F)|0x20;
	m_cx_3ah.SetIcon(IDI_LED_BLUEOFF);
	m_cx_wpp.SetIcon(IDI_LED_BLUEOFF);
	m_cx_FE_3AH.SetIcon(IDI_LEDON);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton50()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] &= 0xF7;
	m_rooffj0.SetIcon(IDI_LEDON);
	m_rooffj1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton51()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] |= 0x08;
	m_rooffj1.SetIcon(IDI_LEDON);
	m_rooffj0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton52()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] &= 0xFB;
	m_gw.SetIcon(IDI_LEDON);
	m_gl.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton53()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] |= 0x04;
	m_gl.SetIcon(IDI_LEDON);
	m_gw.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton54()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] |= 0x01;
	m_xc_w.SetIcon(IDI_LED_BLUEOFF);
	m_xc.SetIcon(IDI_LEDON);
	//m_xc.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton55()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[0] &= 0xFE;
	m_xc_w.SetIcon(IDI_LEDON);
	m_xc.SetIcon(IDI_LED_BLUEOFF);
	//m_xc.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}

void CCANFactorySetDlg::doOneKeySet()
{
	UINT Len = 0;
	CString Str;
	m_Edit_CarName.GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请输入车辆名称"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarNameData,Str.GetLength(),NULL,NULL);
	m_Edit_CarVIN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)CarVINData,Str.GetLength(),NULL,NULL);
	m_Edit_DiagnoseSN.GetWindowText(Str);
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)DiagnoseSNData,Str.GetLength(),NULL,NULL);
	ECUMakeDate();
	ECUSWDate();
	ECUConfigDate();

	GetDlgItem(IDC_EDIT18)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)SWVersion,Str.GetLength(),NULL,NULL);
	ByteStuffing(SWVersion,(Str.GetLength()-1),21);
	GetDlgItem(IDC_EDIT19)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)HWVersion,Str.GetLength(),NULL,NULL);
	ByteStuffing(HWVersion,(Str.GetLength()-1),16);

	GetDlgItem(IDC_EDIT20)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置零件号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)PartNumberBuf,Str.GetLength(),NULL,NULL);
	GetDlgItem(IDC_EDIT21)->GetWindowText(Str);
	if (Str.IsEmpty()){
		MessageBox(_T("请配置供应商识别号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)SystemSupplierIdBuf,Str.GetLength(),NULL,NULL);
	if (!GetSNDate())
	{
		return ;
	}

	fixer.Reset();
	fixer.StartSet();
	CCANSend::Multi_Frame(TestID,0x2E,0xA0,NULL,CarNameData,sizeof(CarNameData));
	//	SetTimer(3,2,NULL);
	DataAnalyzeFlg = 1;
}

void CCANFactorySetDlg::loadConfig( const CString &FilePathName )
{
	LoadConfig(FilePathName);
}

void CCANFactorySetDlg::LoadConfig(LPCTSTR cfg_file)
{
	InitHelper::FieldShowIDMap items[] = {
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ECU软件版本号"), IDC_EDIT18, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ECU硬件版本号"), IDC_EDIT19, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("供应商识别号"), IDC_EDIT21, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("零件号"), IDC_EDIT20, _T("")),
	};

	//	GetDlgItem(IDC_EDIT20)->SetWindowText(ReadStr);	//零件号
	InitHelper::LoadConfigUpdateUI(items, ARRAY_SIZE(items), cfg_file, _T("配置信息"), this);
}

void CCANFactorySetDlg::OnBnClickedButton56()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[1] &= 0xFB;
	m_kt0.SetIcon(IDI_LEDON);
	m_kt1.SetIcon(IDI_LED_BLUEOFF);

	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton57()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;
	MMIConfigData[1] |= 0x04;
	m_kt1.SetIcon(IDI_LEDON);
	m_kt0.SetIcon(IDI_LED_BLUEOFF);

	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton49()
{
	if (ToolModel != 2)
		return;
	MMIConfigData[4] = MMIConfigData[4]&0xF3;

	m_Carmera0.SetIcon(IDI_LEDON);
	m_Carmera1.SetIcon(IDI_LED_BLUEOFF);
	m_Console_Ambient_Light0.SetIcon(IDI_LED_BLUEOFF);
	m_Console_Ambient_Light1.SetIcon(IDI_LED_BLUEOFF);

	if (ToolModel == 2)
	{
		//CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton58()
{
	if (ToolModel != 2)
		return;
	MMIConfigData[4] = (MMIConfigData[4]&0xF3)|0x04;

	m_Carmera0.SetIcon(IDI_LED_BLUEOFF);
	m_Carmera1.SetIcon(IDI_LEDON);
	m_Console_Ambient_Light0.SetIcon(IDI_LED_BLUEOFF);
	m_Console_Ambient_Light1.SetIcon(IDI_LED_BLUEOFF);

	if (ToolModel == 2)
	{
		//CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton59()
{
	if (ToolModel != 2)
		return;
	MMIConfigData[4] = (MMIConfigData[4]&0xF3)|0x08;

	m_Carmera0.SetIcon(IDI_LED_BLUEOFF);
	m_Carmera1.SetIcon(IDI_LED_BLUEOFF);
	m_Console_Ambient_Light0.SetIcon(IDI_LEDON);
	m_Console_Ambient_Light1.SetIcon(IDI_LED_BLUEOFF);

	if (ToolModel == 2)
	{
		//CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton60()
{
	if (ToolModel != 2)
		return;
	MMIConfigData[4] = (MMIConfigData[4]&0xF3)|0x0C;

	m_Carmera0.SetIcon(IDI_LED_BLUEOFF);
	m_Carmera1.SetIcon(IDI_LED_BLUEOFF);
	m_Console_Ambient_Light0.SetIcon(IDI_LED_BLUEOFF);
	m_Console_Ambient_Light1.SetIcon(IDI_LEDON);

	if (ToolModel == 2)
	{
		//CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		CCANSend::Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfigData,sizeof(MMIConfigData));
		//	SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton61()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2)
		return;

	NetWorkConfigData[4] &= 0xBF;
	gw_config0.SetIcon(IDI_LEDON);
	gw_config1.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2 )
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}


void CCANFactorySetDlg::OnBnClickedButton62()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ToolModel != 2 )
		return;

	NetWorkConfigData[4] |= 0x40;
	gw_config1.SetIcon(IDI_LEDON);
	gw_config0.SetIcon(IDI_LED_BLUEOFF);
	if (ToolModel == 2)
	{
		CCANSend::Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
		//SetTimer(3,2,NULL);
		DataAnalyzeFlg = 1;
		return ;
	}
}
