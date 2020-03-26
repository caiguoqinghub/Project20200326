// Main.cpp : 实现文件
//

//#include "shlwapi.h"
//#pragma comment(lib,"shlwapi.lib")
#include "stdafx.h"
#include "CANFactorySet.h"
#include "Main.h"
#include "afxdialogex.h"
#include "ExceptFrameFixer.h"
#include "PasswordVerifyDialog.h"
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Main 对话框

IMPLEMENT_DYNAMIC(Main, CDialog)

Main::Main(CWnd* pParent /*=NULL*/)
	: CDialog(Main::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CANDlg  = new CCANSend;
	
}

Main::~Main()
{
}

void Main::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_STATIC_MACHINE_KIND, machine_kind_);
}


BEGIN_MESSAGE_MAP(Main, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_COMMAND(ID_32776, &Main::On32776)
	ON_COMMAND(ID_32777, &Main::On32777)
	ON_COMMAND(ID_32778, &Main::On32778)
	ON_COMMAND(ID_32779, &Main::On32779)
	ON_COMMAND(ID_32780, &Main::On32780)
	ON_COMMAND(ID_32781, &Main::On32781)
	ON_COMMAND(ID_32782, &Main::On32782)
	ON_COMMAND(ID_32783, &Main::On32783)
	ON_MESSAGE(WM_SUB_MSG,&Main::OnSubMsg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_32771, &Main::On32771)
	ON_COMMAND(ID_32775, &Main::On32775)
	ON_COMMAND(ID_32784, &Main::On32784)
END_MESSAGE_MAP()


// Main 消息处理程序


BOOL Main::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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

	// TODO:  在此添加额外的初始化
	m_TabCtrl.InsertItem(0,L"工厂设置");
	m_TabCtrl.InsertItem(1,L"版本对比");
	m_TabCtrl.Init();
	CANDlg->Open_VCIPort();

	VCI_CAN_OBJ SendInit;
	SendInit.TimeStamp = 0;
	SendInit.TimeFlag = 0;
	SendInit.SendType = 1;
	SendInit.RemoteFlag = 0;//是否是远程帧
	SendInit.ExternFlag = 0;//是否是扩展帧
	SendInit.DataLen = 8;
	SendInit.ID = 0x00;
	memset(SendInit.Data,0x00,8);
	if (VCI_Transmit(VCI_USBCAN1,0,0,&SendInit,1)  == STATUS_OK)
	{	/*OK*/	}
	else
	{/*Fail*//*ErrorFlg = 0x01;*/}

	MainHwnd = this->GetSafeHwnd();
	ShowTxFlg = 0;

	if (CanDev_connect == E_OK)
	{
		SetTimer(TxMsgTimer,5,NULL);
		SetTimer(RxMsgTimer,1,NULL);
		SetTimer(3,5,NULL);
	}

//	SetTimer(4,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Main::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
	
}


void Main::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	KillTimer(TxMsgTimer);
	KillTimer(RxMsgTimer);
	KillTimer(3);
	KillTimer(4);
	CANDlg->Close_VCIPort();
	CDialog::OnClose();
}


void Main::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TxMsgTimer :				//5ms
		//if (SetFlg=0xA8)
		//{
		//	//
		//}
		//else{
			CANDlg->Transmit();
			if (ShowTxFlg == 1)
			{
				ShowTxFlg = 0;
				DisTxData(TxShowData,11,8);
			}
		/*}*/
		break;
	case RxMsgTimer:				//1ms
		CANDlg->Deal_ReceiveFrameMsg();
		if (ShowRxFlg == 1)
		{
			ShowRxFlg = 0;
			DisTxData(RxShowData,10,8);
		}
		break;
	case 3 :						//5ms
		{
			if (DataAnalyzeFlg == 1)
			{
				m_TabCtrl.dlg1->DataAnalyze();
				if (Time4)
				{
					SetTimer(4,1000,NULL);
					Time4 = 0;
				}
			}else if (DataAnalyzeFlg == 2)
			{
				m_TabCtrl.dlg2->DataAnalyze();
			}
		}
		
		break;
	case 4:
			CANDlg->Single_Frame(TestID,0x22,0xA0,NULL,NULL,0);
			KillTimer(4);
			break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void Main::DisTxData( BYTE *byData,int inum,UINT DLC)
{
	CString sSwapA,sSwapB;

	if (inum == 0){
		sSwapA = L"Tx车辆名称： "; 
	}else if (inum == 1){
		sSwapA = L"Tx车辆VIN码： "; 
	}else if (inum == 2){
		sSwapA = L"Tx诊断仪序列号： "; 
	}else if (inum == 3){
		sSwapA = L"TxECU制造日期： "; 
	}else if (inum == 4){
		sSwapA = L"TxECU软件刷新日期： "; 
	}else if (inum == 5){
		sSwapA = L"TxECU装配日期： "; 
	}else if (inum == 6){
		sSwapA = L"TxMMI配置功能： "; 
	}else if (inum == 7){
		sSwapA = L"Tx网络架构配置： ";
	}else if (inum == 8){
		sSwapA = L"Tx机器序列号： "; 
	}else if (inum == 9){
		//	sSwapA = L" "; 
	}else if (inum == 10)
	{
		sSwapA = L"Rx： "; 
	}else if (inum == 11)
	{
		sSwapA = L"Tx： "; 
	}

	for(UINT ii=0;ii<DLC;ii++)
	{
		sSwapB.Format(_T("%02X "),byData[ii]);
		sSwapA += sSwapB;
	}
	Show_Box(sSwapA);
}



void Main::Show_Box(CString dat)
{
	if (dat.GetLength()>20)
	{
		m_ListBox.SetHorizontalExtent(1000);
	}
	m_ListBox.InsertString(m_ListBox.GetCount(),dat);
	m_ListBox.SetCurSel(m_ListBox.GetCount()-1);
}

void Main::On32776()
{
	// TODO: 在此添加命令处理程序代码
	m_TabCtrl.dlg1->EnterTextMode();
}


void Main::On32777()
{
	// TODO: 在此添加命令处理程序代码
	m_TabCtrl.dlg1->ExitTextmode();
}


void Main::On32778()
{
	// TODO: 在此添加命令处理程序代码
	m_ListBox.ResetContent();
}


void Main::On32779()
{
	// TODO: 在此添加命令处理程序代码
	CString FilePathName;
	CFileDialog filedlg(FALSE,L"txt",Honeywelltest,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"Excel CSV File(*.csv)|*.csv|All Files (*.txt*)|*.txt*||");
	if(filedlg.DoModal()==IDOK)
	{
		FilePathName=filedlg.GetPathName();
		CString Save,Save1;
		for (int i = 0;i<m_ListBox.GetCount();i++)
		{
			m_ListBox.GetText(i,Save1);
			Save1.Replace(L"\r\n",L"");
			Save += Save1;
			Save += L"\r\n";
		}

		TRY 
		{
			CFile file(FilePathName, CFile::modeCreate | CFile::modeWrite);
			file.SeekToBegin();//到达文件开头   
			file.Write("\xff\xfe", 2); 
			file.Write(Save,Save.GetLength()*2);
			file.Close();
			MessageBox(_T("保存成功"),_T("提示"),MB_OK|MB_ICONQUESTION);
		}
		CATCH (CMemoryException, e)
		{
			e->ReportError();
			e->Delete();
		}
		END_CATCH
	}
}

//工厂模式
void Main::On32780()
{
	// TODO: 在此添加命令处理程序代码
// 	if (ToolModel == 0)
// 		return ;
	m_TabCtrl.dlg1->SetFactoryMode();
	m_TabCtrl.dlg2->SWModeShow();
	CMenu *pMainMenu     =      new   CMenu;
	pMainMenu->LoadMenu(IDR_MENU1);
	CMenu *pSubMenu     =     NULL;    
	pSubMenu     =  pMainMenu->GetSubMenu(5); //‘1’为对应菜单栏第几个，从0开始，我就是这里错误的
	SetMenu(pMainMenu);
	pSubMenu->CheckMenuItem(ID_32780,MF_CHECKED); 
	pSubMenu->CheckMenuItem(ID_32781,MF_UNCHECKED); 
	pSubMenu->CheckMenuItem(ID_32782,MF_UNCHECKED); 
	pSubMenu->CheckMenuItem(ID_32783,MF_UNCHECKED); 
	GetDlgItem(IDC_STC_FactoryMode)->SetWindowText(L"GEELY_FE_3AH工厂设置工具：工厂模式");
}
//读模式
void Main::On32781()
{
	// TODO: 在此添加命令处理程序代码
// 	if (ToolModel == 1)
// 		return ;
	m_TabCtrl.dlg1->SetReadMode();
	m_TabCtrl.dlg2->SWModeShow();
	CMenu *pMainMenu     =      new   CMenu;
	pMainMenu->LoadMenu(IDR_MENU1);
    CMenu *pSubMenu     =     NULL;    
 	pSubMenu     =  pMainMenu->GetSubMenu(5); //‘1’为对应菜单栏第几个，从0开始，我就是这里错误的
 	SetMenu(pMainMenu);
	pSubMenu->CheckMenuItem(ID_32781,MF_CHECKED); 
 	pSubMenu->CheckMenuItem(ID_32780,MF_UNCHECKED); 
	pSubMenu->CheckMenuItem(ID_32782,MF_UNCHECKED); 
	pSubMenu->CheckMenuItem(ID_32783,MF_UNCHECKED); 
	GetDlgItem(IDC_STC_FactoryMode)->SetWindowText(L"GEELY_FE_3AH工厂设置工具：读模式");
}
//QA模式 
void Main::On32782()
{
	// TODO: 在此添加命令处理程序代码
// 	if (ToolModel == 3)
// 		return ;
	m_TabCtrl.dlg1->SetQAMode();
	m_TabCtrl.dlg2->SWModeShow();
	CMenu *pMainMenu     =      new   CMenu;
	pMainMenu->LoadMenu(IDR_MENU1);
	CMenu *pSubMenu     =     NULL;    
	pSubMenu     =  pMainMenu->GetSubMenu(5); //‘1’为对应菜单栏第几个，从0开始，我就是这里错误的
	SetMenu(pMainMenu);
	pSubMenu->CheckMenuItem(ID_32782,MF_CHECKED); 
	pSubMenu->CheckMenuItem(ID_32780,MF_UNCHECKED); 
	pSubMenu->CheckMenuItem(ID_32781,MF_UNCHECKED); 
	pSubMenu->CheckMenuItem(ID_32783,MF_UNCHECKED); 
	GetDlgItem(IDC_STC_FactoryMode)->SetWindowText(L"GEELY_FE_3AH工厂设置工具：QA模式");
}
//测试模式
void Main::On32783()
{
	// TODO: 在此添加命令处理程序代码
// 	if (ToolModel == 2)
// 		return ;

	CPasswordVerifyDialog dlg;
	if (IDOK != dlg.DoModal()) {
		return;
	}


	m_TabCtrl.dlg1->SetTestMode();
	m_TabCtrl.dlg2->SWModeShow();
	CMenu *pMainMenu     =      new   CMenu;
	pMainMenu->LoadMenu(IDR_MENU1);
	CMenu *pSubMenu     =     NULL;    
	pSubMenu     =  pMainMenu->GetSubMenu(5); //‘1’为对应菜单栏第几个，从0开始，我就是这里错误的
	SetMenu(pMainMenu);
	pSubMenu->CheckMenuItem(ID_32783,MF_CHECKED); 
	pSubMenu->CheckMenuItem(ID_32780,MF_UNCHECKED); 
	pSubMenu->CheckMenuItem(ID_32782,MF_UNCHECKED); 
	pSubMenu->CheckMenuItem(ID_32781,MF_UNCHECKED); 
	GetDlgItem(IDC_STC_FactoryMode)->SetWindowText(L"GEELY_FE_3AH工厂设置工具：测试模式");
}


afx_msg LRESULT Main::OnSubMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		CString *ShowData;
		ShowData = (CString *) lParam;
		Show_Box(*ShowData);
	}else if (wParam == 2)
	{
		m_TabCtrl.dlg1->SetDlgTrue();
	}else if (wParam == 3)
	{
		m_TabCtrl.dlg2->SetDlgTure();
	}

	return 0;
}


void Main::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


HBRUSH Main::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	CFont   f2;
	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_LIST1)
	{
		pDC->SetTextColor(RGB(0,0,0));//字体颜色
	//	pDC->SetBkColor(RGB(255, 0, ));//字体背景色

		LOGFONT logFont1;
		logFont1.lfHeight=20; //字体高度
		logFont1.lfWidth=20;   //字体间距

		//logFont.lfEscapement=0; 
		//logFont.lfOrientation=0; 
		//logFont.lfWeight=FW_NORMAL; 
		//logFont.lfItalic=0;    //是否斜体字体
		//logFont.lfStrikeOut=0; //是否加删除线
		//logFont.lfCharSet=CHINESEBIG5_CHARSET; //设置字体类型
		//logFont.lfOutPrecision=OUT_DEFAULT_PRECIS; 
		//logFont.lfClipPrecision=OUT_DEFAULT_PRECIS; 
		//logFont.lfQuality=DEFAULT_QUALITY; 
		//logFont.lfPitchAndFamily=DEFAULT_PITCH|FF_ROMAN; 

		f2.CreateFontIndirect(&logFont1); 
		pWnd->SetFont(&f2,FALSE); //只画一次
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void Main::On32771()
{
	// TODO: 在此添加命令处理程序代码
	CANDlg->Open_VCIPort();
	SetTimer(TxMsgTimer,5,NULL);
	SetTimer(RxMsgTimer,1,NULL);
	SetTimer(3,2,NULL);
}


void Main::On32775()
{
	// TODO: 在此添加命令处理程序代码
	if(CanDev_connect == E_NOT_OK)
		return;

	CANDlg->Close_VCIPort();
	MessageBox(L"CAN关闭成功",L"提醒！");
	KillTimer(TxMsgTimer);
	KillTimer(RxMsgTimer);
	KillTimer(3);
	KillTimer(4);
}


void Main::On32784()
{
	// TODO: 在此添加命令处理程序代码
//	this->OnLButtonDown(NULL, NULL);
	
	TCHAR path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	CString filePath(path);

	filePath += _T("\\help.doc");

	if (PathFileExists(filePath))
	{
		ShellExecute(this->m_hWnd, _T("open"), filePath, _T(""), _T(""), SW_SHOW);
		return;
	}


	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(IDR_WORDFILESHELP), _T("WORDFILES"));
	if (!hRes)
	{
		AfxMessageBox(_T("找不到帮助文档资源!"));
		return;
	}
	DWORD len = SizeofResource(hInst, hRes);
	BYTE* lpRes = (BYTE*)LoadResource(hInst, hRes); //加载到内存
	CFile file(_T("help.doc"), CFile::modeWrite|CFile::modeCreate); //打开文件,这里的文件名自己修改
	file.Write(lpRes, len); //写
	file.Close();

	//   SetFileAttributes(filePath, FILE_ATTRIBUTE_READONLY);

	ShellExecute(this->m_hWnd, _T("open"), filePath, _T(""), _T(""), SW_SHOW);

}


BOOL Main::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)  
	{  
		if (HoneywellFlg==1)
		{
			GetDlgItem(IDC_EDIT1)->SetFocus();
			HoneywellFlg=0;
		}
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc按键事件  
			return true;  
		case VK_RETURN: //Enter按键事件  
			GetDlgItem(IDC_EDIT1)->PostMessage(WM_KILLFOCUS, 0, 0);
			::SendMessage(GetDlgItem(IDC_EDIT1)-> m_hWnd,WM_KILLFOCUS,-1,0);
			SendDlgItemMessage(IDC_EDIT1,WM_KILLFOCUS,0,0);
			//On32776();
			if (TestFlg1==1)
			{
				CString str = L"D:/GEELY";
				if (!PathIsDirectory(str))
				{
					::CreateDirectory(str, NULL);//创建目录,已有的话不影响
				}
				str += L"/FE_3AH";
				if (!PathIsDirectory(str))
				{
					::CreateDirectory(str, NULL);//创建目录,已有的话不影响
				}
				str += L"/工厂设置工具/";
				if (!PathIsDirectory(str))
				{
					::CreateDirectory(str, NULL);//创建目录,已有的话不影响
				}

				CString FileName;
				GetDlgItem(IDC_EDIT1)->GetWindowText(FileName);
				CString FilePathName,FileTime;
				CTime m_time;  
				m_time=CTime::GetCurrentTime();             //获取当前时间日期  
				FileTime=m_time.Format(_T("_%Y%m%d_%H%M%S"));   //格式化日期时间 
				FilePathName=str+FileName+FileTime+_T(".txt");
				//CFile file(FilePathName, CFile::modeCreate | CFile::modeWrite);
				//--CFileDialog filedlg(FALSE,L"txt",Honeywelltest,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"Excel CSV File(*.csv)|*.csv|All Files (*.txt*)|*.txt*||");
				//CFile file(L"D:/GEELY/FE_3AH/工厂设置工具/2020-01-03 134218 Friday.txt", CFile::modeCreate | CFile::modeWrite);
				//--file.Close();


				m_TabCtrl.dlg1-> SetFactoryMode();//菜单选择工厂模式
				m_TabCtrl.dlg1->Dataread();//读数据到动态文本框中
				//	m_TabCtrl.dlg1->OnBnClickedButton6();//Set按钮

				fixer.Reset();
				fixer.StartSet();
				m_TabCtrl.dlg1->dataconfig();
				//m_TabCtrl.dlg1->DataAnalyze();//数据分析
				GetEditText();//获取条码框的值
				m_TabCtrl.dlg1->savedata();//保存数据到文件（设置数据具体格式）

				m_TabCtrl.dlg2->Dataread();//读数据到动态文本框中
				m_TabCtrl.dlg2->OnBnClickedButton2();//Vesion界面Start按钮
				GetEditText();//获取条码框的值
				m_TabCtrl.dlg2->savedata();

				//
				//if(filedlg.DoModal()==IDOK)
				//{
				//	FilePathName=file.GetPathName();
					CString Save,Save1;
					for (int i = 0;i<m_ListBox.GetCount();i++)
					{
						m_ListBox.GetText(i,Save1);
						Save1.Replace(L"\r\n",L"");
						Save += Save1;
						Save += L"\r\n";
					}

					TRY 
					{
						CFile file(FilePathName, CFile::modeCreate | CFile::modeWrite);
						file.SeekToBegin();//到达文件开头   
						file.Write("\xff\xfe", 2); 
						file.Write(Save,Save.GetLength()*2);
						file.Close();
						MessageBox(_T("保存成功"),_T("提示"),MB_OK|MB_ICONQUESTION);
					}
					CATCH (CMemoryException, e)
					{
						e->ReportError();
						e->Delete();
					}
					END_CATCH
				//}

				
				GetDlgItem(IDC_EDIT1)->SetWindowText(L"");//扫码后文本框自动清空
				//TestFlg=0;
			}else{
				MessageBox(L"请点击进入测试模式！");
				//TestFlg=0;
			};

			return true;  
		case VK_F8:
			{
				CMenu *pMainMenu     =      new   CMenu;
				pMainMenu->LoadMenu(IDR_MENU1);
				CMenu *pSubMenu     =     NULL;    
				pSubMenu     =  pMainMenu->GetSubMenu(5); //‘1’为对应菜单栏第几个，从0开始，我就是这里错误的
				SetMenu(pMainMenu);
			//	pSubMenu->CheckMenuItem(ID_32783,MF_CHECKED); 
                pSubMenu->EnableMenuItem(ID_32783,FALSE);
				On32783();
			}break;
		default:  
			;  
		}  
	}  
	return CDialog::PreTranslateMessage(pMsg);
}
void Main::FileCreate()
{


}
void Main::GetEditText()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	if (str.IsEmpty())
	{
		return;
	}

	Honeywelltest=str;

}

