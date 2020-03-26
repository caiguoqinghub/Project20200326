// Version.cpp : 实现文件
//

#include "stdafx.h"
#include "CANSend.h"
#include "CANFactorySet.h"
#include "Version.h"
#include "afxdialogex.h"

#include "FileFolderUtil.h"
#include "VersionInformationConfigurationDialog.h"
#include "IniHelper.h"

#define  VERSIONDLG     3

//----------------------------------------------------------软件版本号-------------------------------------------------------
//主板软件版本号
BYTE *GetMBSWVersion = new BYTE[100];
//前板系统版本号
BYTE *GetFPOSSWVersion = new BYTE[100];
//前板软件版本号
BYTE *GetFPAPPSWVersion = new BYTE[100];
//蓝牙软件版本号
BYTE *GetBTSWVersion = new BYTE[100];
//导航软件版本号
BYTE *GetNAVISWVersion = new BYTE[100];
//伺服软件版本号
BYTE *GetSVSWVersion = new BYTE[100];
//CAN软件版本号
BYTE *GetCANSWVersion = new BYTE[100];
//ECU软件版本号
BYTE *GetECUSWVersion = new BYTE[100];
//--------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------硬件版本号------------------------------------------------------------
//主板硬件版本号
BYTE *GetMBHWVersion = new BYTE[100];
//前板硬件版本号
BYTE *GetFPHWVersion = new BYTE[100];
//蓝牙硬件版本号
BYTE *GetBTHWVersion = new BYTE[100];
//导航硬件版本号
BYTE *GetNAVIHWVersion = new BYTE[100];
//伺服硬件版本号
BYTE *GetSVHWVersion = new BYTE[100];
//ECU硬件版本号
BYTE *GetECUHWVersion = new BYTE[100];
//-----------------------------------------------------------------------------------------------------------------------------------------

BYTE MMIConfig[16];
BYTE NetWorkConfig[8];
float GetMapSpaceMin;
float GetMapSpaceMax;

CString ErrorStr1;

UINT AC = 0;

UINT NSatellite = 0;
// CVersion 对话框

IMPLEMENT_DYNAMIC(CVersion, CDialogEx)

CVersion::CVersion(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVersion::IDD, pParent)
{

}

CVersion::~CVersion()
{
}

void CVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON7, m_But_AC_ON);
	DDX_Control(pDX, IDC_BUTTON8, m_But_AC_OFF);
}


BEGIN_MESSAGE_MAP(CVersion, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CVersion::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVersion::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CVersion::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CVersion::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CVersion::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CVersion::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CVersion::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CVersion::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CVersion::OnBnClickedButton9)
END_MESSAGE_MAP()


// CVersion 消息处理程序


void CVersion::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CString ReadStr,ReadS,Str;
	UINT Index,Len,count = 0;
	char CRead[100];
	CFileDialog filedlg(TRUE,L".ini",L"", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,L"configure File(*.ini)|*.ini||");
	filedlg.m_ofn.lpstrInitialDir = FileFolderUtil::GetInstance().GetExecuteFileDirecotroy();
	if(filedlg.DoModal()==IDOK)
	{
		CVersionInformationConfigurationDialog configuration;
		if (IDOK == configuration.DoModal(filedlg.GetPathName()))
		{
		SetDlgItemText(IDC_EDIT1, configuration.GetMainBoardSoftwareVersion());
		SetDlgItemText(IDC_EDIT2, configuration.GetFrontBoardSystemVersion());
		SetDlgItemText(IDC_EDIT3, configuration.GetFrontBoardSoftwareVersion());
		SetDlgItemText(IDC_EDIT5, configuration.GetNavSoftwareVersion());

		SetDlgItemText(IDC_EDIT8, configuration.GetSupplierSoftwareVersion());
		SetDlgItemText(IDC_EDIT9, configuration.GetMainBoardHardwareVersion());
		SetDlgItemText(IDC_EDIT14, configuration.GetSupplierHardwareVersion());

		SetDlgItemText(IDC_EDIT15, configuration.GetMinMapResourceSize());
		SetDlgItemText(IDC_EDIT16, configuration.GetMaxMapResourceSize());
		SetDlgItemText(IDC_EDIT155, configuration.GetNsatelliteNumber());
		//NSatellite = _ttoi(configuration.GetNsatelliteNumber());
		}
	}
	::SendMessage(MainHwnd,WM_SUB_MSG,2,NULL);
}


UINT CVersion::Compare(BYTE *dat0,BYTE *dat1,UINT Len)
{
	if (Len == 0)
	{
#if 1
		if ('\0' == *dat1) {
			return 1;
		}
#endif
		return 0;
	}
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

void CVersion::SendShowData(CString Str)
{
	/*	::PostMessage(MainHwnd,WM_SUB_MSG,1,(LPARAM)(&Str));*/
	::SendMessage(MainHwnd,WM_SUB_MSG,1,(LPARAM)(&Str));
}

void CVersion::DataCmp(UINT VersionID,UINT Num)
{
	CString Str,Str1;
	if (VersionID == 0x81)
	{
		switch (Num)
		{
		        //if (0x01 == Num)
		case 0x01:
			{
		 		if (Compare(MBSWVersion->data,GetMBSWVersion,MBSWVersion->Len)){
		 			SendShowData(L"主板软件版本号正确");
		 		}else {
		 			SendShowData(L"主板软件版本号错误");
					ErrorStr1 += L"主板软件版本号错误\r\n";
					ErrorStrFlg = 1;
		 		}
		 	  }break;
	             //else if (0x02 == Num)
		case 0x02:
			{
				  if (Compare(FPOSSWVersion->data,GetFPOSSWVersion,FPOSSWVersion->Len)){
					  SendShowData(L"前板系统版本号正确");
				  }else {
					  SendShowData(L"前板系统版本号错误");
					  ErrorStr1 += L"前板系统版本号错误\r\n";
					  ErrorStrFlg = 1;
				  }
		 	  }break;
	           //else if (0x03 == Num)
		case 0x03:
			{
				  if (Compare(FPAPPSWVersion->data,GetFPAPPSWVersion,FPAPPSWVersion->Len)){
					  SendShowData(L"前板软件版本号正确");
				  }else {
					  SendShowData(L"前板软件版本号错误");
					  ErrorStr1 += L"前板软件版本号错误\r\n";
					  ErrorStrFlg = 1;
				  }
		 	  }break;
	                  //else if (0x04 == Num)
		case 0x04:
			{
				  if (Compare(BTSWVersion->data,GetBTSWVersion,BTSWVersion->Len)){
					  SendShowData(L"蓝牙软件版本号正确");
				  }else {
					  SendShowData(L"蓝牙软件版本号错误");
					  ErrorStr1 += L"蓝牙软件版本号错误\r\n";
					  ErrorStrFlg = 1;
				  }
		 	  }break;
	             //else if (0x05 == Num)
		case  0x05:
			{

				  if (Compare(NAVISWVersion->data,GetNAVISWVersion,NAVISWVersion->Len)){
					  SendShowData(L"导航软件版本号正确");
				  }else {
					  SendShowData(L"导航软件版本号错误");
					  ErrorStr1 += L"导航软件版本号错误\r\n";
					  ErrorStrFlg = 1;
				  }
		 	  }break;
	              //else if (0x06 == Num)
		case 0x06 :
		       {
				  if (Compare(SVSWVersion->data,GetSVSWVersion,SVSWVersion->Len)){
					  SendShowData(L"伺服软件版本号正确");
				  }else {
					  SendShowData(L"伺服软件版本号错误");
					  ErrorStr1 += L"伺服软件版本号错误\r\n";
					  ErrorStrFlg = 1;
				  }
		 	  }break;
	               //else if (0x07 == Num)
		case 0x07:
			{
				  if (Compare(CANSWVersion->data,GetCANSWVersion,CANSWVersion->Len)){
					  SendShowData(L"CAN软件版本号正确");
				  }else {
					  SendShowData(L"CAN软件版本号错误");
					  ErrorStr1 += L"CAN软件版本号错误\r\n";
					  ErrorStrFlg = 1;
				  }
		 	  }break;
	         //else if (0x08 == Num)
		case 0x08:
			{
				  if (Compare(ECUSWVersion->data,GetECUSWVersion,ECUSWVersion->Len)){
					  SendShowData(L"ECU软件版本号正确");
				  }else {
					  SendShowData(L"ECU软件版本号错误");
					  ErrorStr1 += L"ECU软件版本号错误\r\n";
					  ErrorStrFlg = 1;
				  }
		 	  }break;
		default:break;
	  }
	}else if (VersionID == 0x82)
	{
		if (0x01 == Num)
		{
			if (Compare(MBHWVersion->data,GetMBHWVersion,MBHWVersion->Len)){
				SendShowData(L"主板硬件版本号正确");
			}else {
				SendShowData(L"主板硬件版本号错误");
				ErrorStr1 += L"主板硬件版本号错误\r\n";
				ErrorStrFlg = 1;
			}

		}else if (0x02 == Num)
		{
			if (Compare(FPHWVersion->data,GetFPHWVersion,FPHWVersion->Len)){
				SendShowData(L"前板硬件版本号正确");
			}else {
				SendShowData(L"前板硬件版本号错误");
				ErrorStr1 += L"前板硬件版本号错误\r\n";
				ErrorStrFlg = 1;
			}
		}else if (0x03 == Num)
		{
			if (Compare(BTHWVersion->data,GetBTHWVersion,BTHWVersion->Len)){
				SendShowData(L"蓝牙硬件版本号正确");
			}else {
				SendShowData(L"蓝牙硬件版本号错误");
				ErrorStr1 += L"蓝牙硬件版本号错误\r\n";
				ErrorStrFlg = 1;
			}
		}else if (0x04 == Num)
		{
			if (Compare(NAVIHWVersion->data,GetNAVIHWVersion,NAVIHWVersion->Len)){
				SendShowData(L"蓝牙硬件版本号正确");
			}else {
				SendShowData(L"蓝牙硬件版本号错误");
				ErrorStr1 += L"蓝牙硬件版本号错误\r\n";
				ErrorStrFlg = 1;
			}
		}else if (0x05 == Num)
		{
			if (Compare(SVHWVersion->data,GetSVHWVersion,SVHWVersion->Len)){
				SendShowData(L"伺服硬件版本号正确");
			}else {
				SendShowData(L"伺服硬件版本号错误");
				ErrorStr1 += L"伺服硬件版本号错误\r\n";
				ErrorStrFlg = 1;
			}
		}else if (0x08 == Num)
		{
			if (Compare(ECUHWVersion->data,GetECUHWVersion,ECUHWVersion->Len)){
				SendShowData(L"ECU硬件版本号正确");
			}else {
				SendShowData(L"ECU硬件版本号错误");
				ErrorStr1 += L"ECU硬件版本号错误\r\n";
				ErrorStrFlg = 1;
			}
		}
	}else if (VersionID == 0x26)
	{

		float Storage = (float)(RxMapSpace[0]+(RxMapSpace[1]*0.01));
		if ((GetMapSpaceMin<=Storage)&&(Storage<=GetMapSpaceMax))
		{
			SendShowData(L"地图资源大小正确");
			// MessageBox(L"地图资源大小正确",L"提示！");
		}else {
			SendShowData(L"地图资源大小错误");
			ErrorStr1 += L"地图资源大小错误\r\n";
			ErrorStrFlg = 1;
		}



	}else if (VersionID == 0x23)
	{
		 	CString Str;
			if (RxSatelliteNumber[0]<NSatellite)
			{
				Str.Format(L"当前搜星不合格 搜星个数:%d个",RxSatelliteNumber[0]);
				ErrorStr1 += Str;
				ErrorStrFlg = 1;
				SendShowData(Str);
			}else if (RxSatelliteNumber[0]>NSatellite)
			{
				Str.Format(L"当前搜星合格  搜星个数:%d个",RxSatelliteNumber[0]);
				SendShowData(Str);
			}

			if (triggered_by_key_) {
				triggered_by_key_ = FALSE;
				ErrorStr = _T("");
				ERRORSTR = _T("");
			}
	}else{
		if (Num == 0xA8)
		{
			if (Compare(RxMMIConfig,MMIConfig,16)){
				SendShowData(L"MMI正确");
				OnBnClickedButton9();
				MessageBox(L"MMI正确",L"提醒");
			}else {
				SendShowData(L"MMI错误");
               MessageBox(L"MMI错误",L"提醒");
			}
		}else if (Num == 0xA9)
		{
			if (AC == 0)
			{
				if (Compare(RxNetWorkConfig,NetWorkConfig,8)){

					SendShowData(L"网络架构正确");
					MessageBox(L"网络架构正确",L"提醒");
				}else {
					SendShowData(L"网络架构错误");
					MessageBox(L"网络架构错误",L"提醒");
				}
			}else if (AC == 1)
			{
				if (Compare(RxNetWorkConfig,NetWorkConfigData,8)){

					SendShowData(L"网络架构正确");
					MessageBox(L"网络架构正确",L"提醒");
				}else {
					SendShowData(L"网络架构错误");
					MessageBox(L"网络架构错误",L"提醒");
				}
			}

		}

	}

}


void CVersion::DataAnalyze()
{

	OverTime ++ ;

	if (ErrorFlg != 0x00)
	{
		//	KillTimer(3);
		DataAnalyzeFlg = 0;
		if (ErrorFlg == 0x01)
		{
			//	MessageBox(L"数据发送失败，请检查设备是否连接",L"提醒");
		}else if (ErrorFlg == 0x11){
			MessageBox(L"服务ID不支持！",L"提示");
		}else if (ErrorFlg == 0x12){
			MessageBox(L"DID不支持！",L"提示");
		}else if (ErrorFlg == 0x13){
			MessageBox(L"报文长度或数据格式错误！",L"提示");
		}else if (ErrorFlg == 0x31){
			MessageBox(L"请求设置的数值超出范围！",L"提示");
		}else if (ErrorFlg == 0x32){
			MessageBox(L"请求数值无效！",L"提示");
		}else if (ErrorFlg == 0x35){
			MessageBox(L"无效的解锁钥匙！",L"提示");
		}else if (ErrorFlg == 0x40){
			MessageBox(L"DID Command 不支持！",L"提示");
		}else if (ErrorFlg == 0x41){
			MessageBox(L"请求设置的功能DVD未完成！",L"提示");
		}else if (ErrorFlg == 0x80){
			MessageBox(L"不在测试模式！",L"提示");
			TestFlg1=0;
		}else if (ErrorFlg == 0x71){
			MessageBox(L"报文类型错误！",L"提示");
		}else if (ErrorFlg == 0x72){
			MessageBox(L"报文SN错误！",L"提示");
		}else if (ErrorFlg == 0x73){
			MessageBox(L"报文时间超时！",L"提示");
		}else if (ErrorFlg == 0x74){
			MessageBox(L"接收报文长度错误！",L"提示");
		}else if (ErrorFlg == 0x75){
			MessageBox(L"填充字节错误！",L"提示");
		}
		OverTime = 0;
		return ;
	}

	if (SetFlg!=0)
	{
		switch (SetFlg)
		{
		case  0xA8:
			{
				Sleep(200);
				CANSendDlg.Single_Frame(TestID,0x22,0xA8);
				TRACE(L"0xA8\r\n");
			}break;
		case  0xA9:
			{
				Sleep(200);
				CANSendDlg.Single_Frame(TestID,0x22,0xA9);
				TRACE(L"0xA9\r\n");
			}break;
		case 0x0A01:
			{
				DataAnalyzeFlg = 0x00;
			}
			
		}
		SetFlg = 0;
	}

			if (ReadFlg!=0)
			{
				if (ReadFlg == 0x23)
				{

				
					DataAnalyzeFlg = 0;
					ReadFlg = 0;
					if (ErrorStrFlg == 0x01)
					{
						ErrorStrFlg = 0;
						Dlg_Error->ShowWindow(SW_SHOW);
						CString ERRORSTR1=ErrorStr1;
						CString name=L"《工厂设置错误》\r\n";
						
						ERRORSTR+="\r\n《版本比对错误》\r\n";
						
						 ERRORSTR+=ERRORSTR1;
						 name+=ERRORSTR;
						Dlg_Error->SetErrorMsg(VERSIONDLG,name);
						this->EnableWindow(FALSE);
					}else{
						ErrorStrFlg = 0;
						Dlg_PASS->ShowWindow(SW_SHOW);
						Dlg_PASS->SetPASSMSG(VERSIONDLG,L"查询正常");
						this->EnableWindow(FALSE);
					}

					if (triggered_by_key_) {
						triggered_by_key_ = FALSE;
						ErrorStr = _T("");
						ERRORSTR = _T("");
					}
					

// 					CString Str;
// 					if (NSatellite == 0)
// 					{
// 						Str.Format(L"当前搜星个数：\n            %d个",RxSatelliteNumber[0]);
// 						MessageBox(Str,L"提醒",MB_ICONASTERISK);
// 					}else {
// 						if (RxSatelliteNumber[0]<NSatellite)
// 						{
// 							Str.Format(L"当前搜星不合格\r\n搜星个数:%d个",RxSatelliteNumber[0]);
// 							Dlg_Error->ShowWindow(SW_SHOW);
// 							Dlg_Error->SetErrorMsg(VERSIONDLG,Str);
// 							this->EnableWindow(FALSE);
// 						}else if (RxSatelliteNumber[0]>NSatellite)
// 						{
// 							Str.Format(L"当前搜星合格\r\n搜星个数:%d个",RxSatelliteNumber[0]);
// 							Dlg_PASS->ShowWindow(SW_SHOW);
// 							Dlg_PASS->SetPASSMSG(VERSIONDLG,Str);
// 							this->EnableWindow(FALSE);
// 						}
				//	}

				}else if (ReadFlg == 0x26)
				{
#if 0
					DataCmp(0x26,0);
#endif
					CANSendDlg.Single_Frame(TestID,0x22,0x23,0x03);

				//	DataAnalyzeFlg = 0;
// 					if (ErrorStrFlg == 1)
// 					{
// 						ErrorStrFlg = 0;
// 						Dlg_Error->ShowWindow(SW_SHOW);
// 						Dlg_Error->SetErrorMsg(VERSIONDLG,ErrorStr1);
// 						this->EnableWindow(FALSE);
// 					}else{
// 						ErrorStrFlg = 0;
// 						Dlg_PASS->ShowWindow(SW_SHOW);
// 						Dlg_PASS->SetPASSMSG(VERSIONDLG,L"查询正常");
// 						this->EnableWindow(FALSE);
// 					}
// 					float Storage = (float)(RxMapSpace[0]+(RxMapSpace[1]*0.01));
// 					   if ((GetMapSpaceMin<=Storage)&&(Storage<=GetMapSpaceMax))
// 					   {
// 						   SendShowData(L"地图资源大小正确");
// 						   Dlg_PASS->ShowWindow(SW_SHOW);
// 						   Dlg_PASS->SetPASSMSG(VERSIONDLG,L"地图大小正确");
// 						   this->EnableWindow(FALSE);
// 						  // MessageBox(L"地图资源大小正确",L"提示！");
// 					   }else {
// 						   SendShowData(L"地图资源大小错误");
// 						   Dlg_Error->ShowWindow(SW_SHOW);
// 						   Dlg_Error->SetErrorMsg(VERSIONDLG,L"地图资源大小错误");
// 						   this->EnableWindow(FALSE);
// 						//   MessageBox(L"地图资源大小错误",L"提示！");
// 					   }
					ReadFlg = 0;
				}else if (ReadFlg == 0x81)
				{
					if (SWVersionFlg == 0x01)
					{
						DataCmp(0x81,0x01);
						CANSendDlg.Single_Frame(TestID,0x22,0x81,0x02,NULL,0);
					}else if (SWVersionFlg == 0x02)
					{
						DataCmp(0x81,0x02);
						CANSendDlg.Single_Frame(TestID,0x22,0x81,0x03,NULL,0);
					}else if (SWVersionFlg == 0x03)
					{
						DataCmp(0x81,0x03);
						CANSendDlg.Single_Frame(TestID,0x22,0x81,0x05,NULL,0);
					}else if (SWVersionFlg == 0x05)
					{
#if 0
						DataCmp(0x81,0x05);
#endif

						CANSendDlg.Single_Frame(TestID,0x22,0x81,0x08,NULL,0);
					}else if (SWVersionFlg == 0x08)
					{
						DataCmp(0x81,0x08);
						CANSendDlg.Single_Frame(TestID,0x22,0x82,0x01,NULL,0);
					}
					SWVersionFlg = 0x00;
					ReadFlg = 0;
				}else  if (ReadFlg == 0x82)
				{ 
					if (HWVersionFlg == 0x01)
					{
						DataCmp(0x82,0x01);
						CANSendDlg.Single_Frame(TestID,0x22,0x82,0x08,NULL,0);
					}else if (HWVersionFlg == 0x08)
					{
						DataCmp(0x82,0x08);
				///		DataAnalyzeFlg = 0;
						CANSendDlg.Single_Frame(TestID,0x22,0x26);

// 						if (ErrorStrFlg == 1)
// 						{
// 							ErrorStrFlg = 0;
// 						//	MessageBox(ErrorStr1,L"提示!");
// 							Dlg_Error->ShowWindow(SW_SHOW);
// 							Dlg_Error->SetErrorMsg(VERSIONDLG,ErrorStr1);
// 							this->EnableWindow(FALSE);
// 							ErrorStr1.Empty();
// 						}else {
// 						//	MessageBox(L"版本号正确",L"提示!");
// 							Dlg_PASS->ShowWindow(SW_SHOW);
// 							Dlg_PASS->SetPASSMSG(VERSIONDLG,L"版本号正确");
// 							this->EnableWindow(FALSE);
// 							ErrorStr1.Empty();
// 						}
					}
					HWVersionFlg = 0x00;
					ReadFlg = 0;
				}else if (ReadFlg == 0xA8)
				{
					DataAnalyzeFlg = 0;
					DataCmp(0,0xA8);
					ReadFlg = 0;
				}else if (ReadFlg == 0xA9)
				{
					DataAnalyzeFlg = 0;
					DataCmp(0,0xA9);
					ReadFlg = 0;
				}

			//	KillTimer(3);

	
		ReadFlg = 0;
		OverTime = 0;
		return ;
	}
	//	OverTime -= 2;
 	if (OverTime == 500)
 	{
 		//	KillTimer(3);
 		DataAnalyzeFlg = 0;
 		OverTime = 0;
 		MessageBox(L"无应答信号，设置超时",L"提醒");
 	}

}

#define ENABLE_EMPTY_EDIT_TEST (FALSE)

void CVersion::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(Str);
	Str=Str+' ';
#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	UINT Temp0=WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetMBSWVersion,Str.GetLength(),NULL,NULL);
	GetMBSWVersion[Temp0]='\0';
	//*(GetMBSWVersion+(Str.GetLength()-1)) = 0x20;

	Str.Empty();
	GetDlgItem(IDC_EDIT2)->GetWindowText(Str);

#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	UINT Temp1=WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetFPOSSWVersion,Str.GetLength(),NULL,NULL);
	GetFPOSSWVersion[Temp1]='\0';
	Str.Empty();
	GetDlgItem(IDC_EDIT3)->GetWindowText(Str);

#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	UINT Temp2=WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetFPAPPSWVersion,Str.GetLength(),NULL,NULL);
	GetFPAPPSWVersion[Temp2]='\0';
// 
// 	Str.Empty();
// 	GetDlgItem(IDC_EDIT4)->GetWindowText(Str);
// 	if (Str.IsEmpty()){
// 		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
// 		return;
// 	}
// 	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetBTSWVersion,Str.GetLength(),NULL,NULL);

	Str.Empty();
	GetDlgItem(IDC_EDIT5)->GetWindowText(Str);

#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	UINT Temp3=WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetNAVISWVersion,Str.GetLength(),NULL,NULL);
	GetNAVISWVersion[Temp3]='\0';

// 	Str.Empty();
// 	GetDlgItem(IDC_EDIT6)->GetWindowText(Str);
// 	if (Str.IsEmpty()){
// 		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
// 		return;
// 	}
// 	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetSVSWVersion,Str.GetLength(),NULL,NULL);
// 
// 
// 	Str.Empty();
// 	GetDlgItem(IDC_EDIT7)->GetWindowText(Str);
// 	if (Str.IsEmpty()){
// 		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
// 		return;
// 	}
// 	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetCANSWVersion,Str.GetLength(),NULL,NULL);

	Str.Empty();
	GetDlgItem(IDC_EDIT8)->GetWindowText(Str);

#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	UINT d = CStringA(Str).GetLength();
	UINT Temp4=WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetECUSWVersion,Str.GetLength(),NULL,NULL);
	GetECUSWVersion[Temp4]='\0';
	ByteStuffing(GetECUSWVersion,Str.GetLength(),16);

	Str.Empty();
	GetDlgItem(IDC_EDIT9)->GetWindowText(Str);

#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	UINT Temp5=WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetMBHWVersion,Str.GetLength(),NULL,NULL);
	GetMBHWVersion[Temp5]='\0';

// 	Str.Empty();
// 	GetDlgItem(IDC_EDIT10)->GetWindowText(Str);
// 	if (Str.IsEmpty()){
// 		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
// 		return;
// 	}
// 	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetFPHWVersion,Str.GetLength(),NULL,NULL);
// 
// 
// 
// 	Str.Empty();
// 	GetDlgItem(IDC_EDIT11)->GetWindowText(Str);
// 	if (Str.IsEmpty()){
// 		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
// 		return;
// 	}
// 	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetBTHWVersion,Str.GetLength(),NULL,NULL);
// 
// 
// 	Str.Empty();
// 	GetDlgItem(IDC_EDIT12)->GetWindowText(Str);
// 	if (Str.IsEmpty()){
// 		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
// 		return;
// 	}
// 	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetNAVIHWVersion,Str.GetLength(),NULL,NULL);
// 
// 	Str.Empty();
// 	GetDlgItem(IDC_EDIT13)->GetWindowText(Str);
// 	if (Str.IsEmpty()){
// 		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
// 		return;
// 	}
// 	WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetSVHWVersion,Str.GetLength(),NULL,NULL);


	Str.Empty();
	GetDlgItem(IDC_EDIT14)->GetWindowText(Str);

#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty()){
		MessageBox(_T("请配置版本号"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	UINT Temp6=WideCharToMultiByte(CP_ACP,0,Str,Str.GetLength(),(LPSTR)GetECUHWVersion,Str.GetLength(),NULL,NULL);
	GetECUHWVersion[Temp6]='\0';
	ByteStuffing(GetECUHWVersion,Str.GetLength(),16);

	Str.Empty();
	GetDlgItem(IDC_EDIT15)->GetWindowText(Str);

#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty())
	{
		MessageBox(_T("请配置地图资源大小"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	Str.Replace(L"G",NULL);
	GetMapSpaceMin = (float)_ttof(Str);
	Str.Empty();
	GetDlgItem(IDC_EDIT16)->GetWindowText(Str);

#if ENABLE_EMPTY_EDIT_TEST
	if (Str.IsEmpty())
	{
		MessageBox(_T("请配置地图资源大小"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
#endif

	Str.Replace(L"G",NULL);
	GetMapSpaceMax = (float)_ttof(Str);
	Str.Empty();

	ErrorStr1.Empty();
	CANSendDlg.Single_Frame(TestID,0x22,0x81,0x01);
	ReadFlg = 0;
	DataAnalyzeFlg = 2;

	triggered_by_key_ = TRUE;
}

void CVersion::ByteStuffing(BYTE *data/*数据*/,UINT DataLen/*数据长度*/,UINT FinalLen/*最终数据长度*/)
{

	for (;DataLen<FinalLen;DataLen++)
	{
		*(data+DataLen) = 0x20;
	}
}

void CVersion::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Str;
	GetDlgItem(IDC_EDIT15)->GetWindowText(Str);
	if (Str.IsEmpty())
	{
		MessageBox(_T("请配置地图资源大小"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	Str.Replace(L"G",NULL);
    GetMapSpaceMin = (float)_ttof(Str);
	Str.Empty();
	GetDlgItem(IDC_EDIT16)->GetWindowText(Str);
	if (Str.IsEmpty())
	{
		MessageBox(_T("请配置地图资源大小"),_T("提醒"),MB_OK|MB_ICONQUESTION);
		return;
	}
	Str.Replace(L"G",NULL);
	GetMapSpaceMax = (float)_ttof(Str);
	
	CANSendDlg.Single_Frame(TestID,0x22,0x26);

	DataAnalyzeFlg = 2;
}

int CVersion::HexToDem(CString str)
{
	int dem=0;
	for(int i=0;i<str.GetLength();i++)
	{
		dem=dem*16;
		if((str[i]<='9')&&(str[i]>='0'))        //0~9之间的字符
			dem+=str[i]-'0';
		else if((str[i]<='F')&&(str[i]>='A'))   //A~F之间的字符
			dem+=str[i]-'A'+10;
		else if((str[i]<='f')&&(str[i]>='a'))   //a~f之间的字符
			dem+=str[i]-'a'+10;
		else
			return -1;                          //出错时返回-1
	}
	return dem;     
}

void CVersion::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CANSendDlg.Single_Frame(TestID,0x22,0x23,0x03);
	DataAnalyzeFlg = 2;
}

void CVersion::StrToByte(BYTE *byte,CString Str)
{
	UINT j = 0;
	CString Str1;
	Str.Replace(L" ",L"");
	for (UINT i=0;i<(Str.GetLength()/2);i++)
	{
		Str1.Empty();
		Str1 = Str.Mid(j,2);
		byte[i] =  HexToDem(Str1);
		j+=2;
		TRACE(L"000");
	}
}

void CVersion::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Str;
	GetDlgItem(IDC_EDIT17)->GetWindowText(Str);
	memset(MMIConfig,0,16);
	StrToByte(MMIConfig,Str);
	CANSendDlg.Multi_Frame(TestID,0x2E,0xA8,NULL,MMIConfig,sizeof(MMIConfig));
	DataAnalyzeFlg = 2;
}


void CVersion::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Str;
	GetDlgItem(IDC_EDIT22)->GetWindowText(Str);
	memset(NetWorkConfig,0,8);
	StrToByte(NetWorkConfig,Str);
	CANSendDlg.Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfig,sizeof(NetWorkConfig));
	AC = 0;
	DataAnalyzeFlg = 2;
}


BOOL CVersion::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ButInit();
	GetDlgItem(IDC_EDIT17)->SetWindowText(L"00 00 00 00 00 00 00 00");
	GetDlgItem(IDC_EDIT22)->SetWindowText(L"00 00 00 00 00 00 00 00");
	Dlg_Error = new CError;
	Dlg_Error->Create(IDD_ERROR,this);
	Dlg_Error->CenterWindow();
	Dlg_PASS = new CPASS;
	Dlg_PASS->Create(IDD_PASS,this);
	Dlg_PASS->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CVersion::ButInit()
{
	// AC配置
	m_But_AC_ON.SetIcon(IDI_LEDON);
	m_But_AC_OFF.SetIcon(IDI_LEDOFF);
}

void CVersion::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	NetWorkConfigData[1]|=0x04;
	m_But_AC_ON.SetIcon(IDI_LEDON);
	m_But_AC_OFF.SetIcon(IDI_LEDOFF);
	CANSendDlg.Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
	AC = 1;
	DataAnalyzeFlg = 2;
}


void CVersion::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	NetWorkConfigData[1]&=0xFB;
	m_But_AC_ON.SetIcon(IDI_LEDOFF);
	m_But_AC_OFF.SetIcon(IDI_LEDON);
	CANSendDlg.Multi_Frame(TestID,0x2E,0xA9,NULL,NetWorkConfigData,sizeof(NetWorkConfigData));
	AC = 1;
	DataAnalyzeFlg = 2;
}

void CVersion::SWModeShow()
{
	if (ToolModel == 2)
	{
		GetDlgItem(IDC_EDIT17)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT22)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STC_AC)->ShowWindow(SW_SHOW);
	    GetDlgItem(IDC_BUTTON9)->ShowWindow(SW_SHOW);
	}else{
		GetDlgItem(IDC_EDIT17)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT22)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STC_AC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON9)->ShowWindow(SW_HIDE);
	}

}

BOOL CVersion::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc按键事件  
			return true;  
		case VK_RETURN: //Enter按键事件  
			return true;  
		default:  
			;  
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CVersion::SetDlgTure()
{
	this->EnableWindow(TRUE);
	return;
}

void CVersion::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CANSendDlg.Single_Frame(TestID,0x2E,0x0A,0x01);
	//DataAnalyzeFlg = 2;
}

void CVersion::savedata()//保存数据到文件（设置数据具体格式）
{
	CString FilePathName;
	//CFileDialog filedlg(FALSE,L"txt",Honeywelltest,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"All Files (*.txt*)|*.txt*||");
	//if(filedlg.DoModal()==IDOK)
	//{
//		FilePathName=filedlg.GetPathName();
		//CString Save,Save1;
		CString Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12,Str13,Str14,Str15,Str16,Str17,Str18,Str19,Str20,Str21,Str22,Str23,Str24,Str31,Str32,Str33,Str34,Str35,Str36,Str37,Str38,Str39,Str40,Str41,Str42,Str43,Str46,errorwname;
		CString Str01,Str02,Str03,Str04,Str05,Str06,Str07,Str08,Str09,Str010,Str011,Str012,Str013,Str014,Str015,Str016,Str017,Str018,Str019,Str020,Str021,Str022;
 MessageBox(_T("正在保存"),_T("提示"),MB_OK|MB_ICONQUESTION);  
		GetDlgItemText(IDC_EDIT1,Str1);
		GetDlgItemText(IDC_EDIT2,Str2);
		GetDlgItemText(IDC_EDIT3,Str3);
		GetDlgItemText(IDC_EDIT5,Str5);
		GetDlgItemText(IDC_EDIT8,Str8);
		GetDlgItemText(IDC_EDIT9,Str9);

		GetDlgItemText(IDC_EDIT14,Str14);
		GetDlgItemText(IDC_EDIT15,Str15);
		GetDlgItemText(IDC_EDIT16,Str16);

		



		//for (int i = 0;i<m_ListBox.GetCount();i++)
		//{
		//	m_ListBox.GetText(i,Save1);
		//	Save1.Replace(L"\r\n",L"");
		//	Save += Save1;
		//	Save += L"\r\n";
		//}




		Str22=L"[版本比对]\r\n";
		Str23=L"<未比对错误信息>\r\n";
		Str24=L"<比对错误信息>\r\n";

		Str01=L"主板软件版本号：";
		Str02=L"前板系统版本号：";
		Str03=L"前板软件版本号：";
		Str05=L"导航软件版本号：";
		Str08=L"供应商ECU软件版本号：";
		Str1+= L"\r\n";
		Str2+= L"\r\n";
		Str3+= L"\r\n";
		Str5+= L"\r\n";
		Str8+= L"\r\n";
		Str01+=Str1;
		Str02+=Str2;
		Str03+=Str3;
		Str05+=Str5;
		Str08+=Str8;

		Str09=L"主板硬件版本号：";
		Str014=L"供应商ECU硬件版本号：";
		Str9+=L"\r\n";
		Str14+=L"\r\n";
		Str09+=Str9;
		Str014+=Str14;

		Str015=L"地图资源大小：";
		Str15+=L"-->";
		Str015+=Str15;
		
		Str16+=L"\r\n";
		Str016+=Str16;

#if 0	//20190417: Modified by liufeifei
		char *temp;
		Str17=_itoa(RxSatelliteNumber[0],temp,10);
#else
		Str17.Format(_T("%d"), RxSatelliteNumber[0]);
#endif
		Str017=L"搜星个数：";
		Str17+=L"\r\n";
		Str017+=Str17;

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


		TRY 
		{
			HMODULE module = GetModuleHandle(0);   
			TCHAR pFileName[MAX_PATH];   
			GetModuleFileName(module, pFileName, MAX_PATH);   

			CString csFullPath(pFileName);   
			int nPos = csFullPath.ReverseFind( _T('\\') ); 
			csFullPath = csFullPath.Left(nPos);

			CString str1=L"\\日志\\";
			csFullPath=csFullPath + str1;

			FilePathName=csFullPath;
			//FilePathName= "D:\\Release\\";
			FilePathName+=Honeywelltest;
			Str46=".txt";
			FilePathName+=Str46;
			CFile file(pNewName, CFile::modeNoTruncate | CFile::modeWrite);
			file.SeekToEnd();//到达文件结尾   
			
			file.Write("\xff\xfe", 2);
			file.Write(Str22,Str22.GetLength()*2);
			file.Write(Str01,Str01.GetLength()*2);
			file.Write(Str02,Str02.GetLength()*2);
			file.Write(Str03,Str03.GetLength()*2);
			
			file.Write(Str05,Str05.GetLength()*2);
	
			file.Write(Str08,Str08.GetLength()*2);
			file.Write(Str09,Str09.GetLength()*2);

			file.Write(Str014,Str014.GetLength()*2);
			file.Write(Str015,Str015.GetLength()*2);
			file.Write(Str016,Str016.GetLength()*2);
			file.Write(Str017,Str017.GetLength()*2);
			file.Write(Str23,Str23.GetLength()*2);
			file.Write(Str31,Str31.GetLength()*2);
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
			file.Write(Str24,Str24.GetLength()*2);
			file.Write(ErrorStr1,ErrorStr1.GetLength()*2);
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

	if (FALSE == ErrorStr1.IsEmpty()||ErrorFlg!=0)//如果有错误文件名后加Error
	{
		HMODULE module = GetModuleHandle(0);   
		TCHAR pFileName[MAX_PATH];   
		GetModuleFileName(module, pFileName, MAX_PATH);   

		CString csFullPath(pFileName);   
		int nPos = csFullPath.ReverseFind( _T('\\') ); 
		csFullPath = csFullPath.Left(nPos);

		CString str1=L"\\日志\\";
		csFullPath=csFullPath + str1;



//		pNewName1= "D:\\Release\\"; //文件保存位置

		pNewName1=csFullPath;
		pNewName1+=Honeywelltest;
		errorwname="--Error";
		Str46=".txt";
		pNewName1+=errorwname;
		pNewName1+=Str46;
		TRY 
		{
			// DeleteFile(L"D:\\Release\\"+Honeywelltest+"--Error.txt");
			DeleteFile(pNewName1+Honeywelltest+"--Error.txt");
			CFile::Rename(pNewName,pNewName1);
		}
		CATCH (CMemoryException, e)
		{
			e->ReportError();
			e->Delete();
		}
		END_CATCH
 	}
}

void CVersion::Dataread(void)
{
	CString FilePathName;
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

	CString str1=L"\\GEELY_FE_3AH_Version.ini";
	csFullPath=csFullPath + str1;
//	FilePathName= "..\\Release\\Version.ini";
	FilePathName=csFullPath;
#if 0
	//	CFileDialog filedlg(TRUE,L".ini",L"GEELY_FE_3AH", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,L"configure File(*.ini)|*.ini||");
	/*	filedlg.m_ofn.lpstrInitialDir = L"\\";*/
	TRY 
	{
		CStdioFile file;
		file.Open(FilePathName,CFile::modeRead|CFile::typeBinary);

		while(file.ReadString(Str))
		{
			Index= 0;
			ReadStr.Empty();
			Len=Str.GetLength();
			if (count==11)
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
				GetDlgItem(IDC_EDIT1)->SetWindowText(ReadStr);
			}else if (count == 2)
			{
				GetDlgItem(IDC_EDIT2)->SetWindowText(ReadStr);
			}else if (count == 3)
			{
				GetDlgItem(IDC_EDIT3)->SetWindowText(ReadStr);
			}else if (count == 4)
			{
				GetDlgItem(IDC_EDIT5)->SetWindowText(ReadStr);
				//	GetDlgItem(IDC_EDIT4)->SetWindowText(ReadStr);
			}else if (count == 5)
			{
				GetDlgItem(IDC_EDIT8)->SetWindowText(ReadStr);
				//	GetDlgItem(IDC_EDIT5)->SetWindowText(ReadStr);
			}else if (count == 6)
			{
				GetDlgItem(IDC_EDIT9)->SetWindowText(ReadStr);
				//	GetDlgItem(IDC_EDIT6)->SetWindowText(ReadStr);
			}else if (count == 7)
			{
				GetDlgItem(IDC_EDIT14)->SetWindowText(ReadStr);
				//GetDlgItem(IDC_EDIT7)->SetWindowText(ReadStr);
			}else if (count == 8)
			{
				GetDlgItem(IDC_EDIT15)->SetWindowText(ReadStr);
			}else if (count == 9)
			{
				GetDlgItem(IDC_EDIT16)->SetWindowText(ReadStr);
			}else if (count == 10)
			{
				NSatellite = _ttoi(ReadStr);
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
	InitHelper::FieldShowIDMap items[] = {
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("主板软件版本号"), IDC_EDIT1, _T("")),
#if FRONT_BOARD_SYSTEM_VERSION_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("前板系统版本号"), IDC_EDIT2, _T("")),
#endif

#if FRONT_BOARD_SOFTWARE_VERSION_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("前板软件版本号"), IDC_EDIT3, _T("")),
#endif

#if NAVIGATE_SOFTWARE_VERSION_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("导航软件版本号"), IDC_EDIT5, _T("")),
#endif
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ECU软件版本号"), IDC_EDIT8, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("主板硬件版本号"), IDC_EDIT9, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("ECU硬件版本号"), IDC_EDIT14, _T("")),

#if MAP_RESOURCE_CHECK_ENABLED
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("地图资源大小（最小值）"), IDC_EDIT15, _T("")),
		STRING_FIELD_SHOW_ID_MAP_ITEM(_T("地图资源大小（最大值）"), IDC_EDIT16, _T("")),
#endif
		#if NSATELLITE_CHECK_ENABLED
				STRING_FIELD_SHOW_ID_MAP_ITEM(_T("搜星最小值"), IDC_EDIT_SX, _T("")),
		#endif
	};

	//	GetDlgItem(IDC_EDIT20)->SetWindowText(ReadStr);	//零件号
	InitHelper::LoadConfigUpdateUI(items, ARRAY_SIZE(items), FilePathName, _T("配置信息"), this);
#endif
}