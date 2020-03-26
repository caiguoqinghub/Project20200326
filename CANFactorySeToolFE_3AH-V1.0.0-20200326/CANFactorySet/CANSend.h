#pragma once

#ifndef  __CANSEND_H__
#define  __CANSEND_H__
#include "ECanVci.H"


#define TestID		REQUEST_CAN_ID

#define  SF3   0x30  //单帧请求报文
#define  FF8   0x80  //多帧第一帧（第一种）请求报文
#define  FF9   0x90  //多帧第一帧（第二种）请求报文
#define  FFA   0xA0  //多帧第一帧（第三种）请求报文
#define  FFB   0xB0  //多帧第一帧（第四种）请求报文
#define  CFC   0xC0  //多帧连续帧（第一种）请求报文
#define  CFD   0xD0  //多帧连续帧（第二种）请求报文
#define  CFE   0xE0  //多帧连续帧（第三种）请求报文
#define  CFF   0xF0  //多帧连续帧（第四种）请求报文
#define WM_SUB_MSG  WM_USER+0x02001


#define  E_OK 1
#define  E_NOT_OK 0

extern UINT CanDev_opended;
extern UINT CanDev_connect;
extern CString Honeywelltest;
//判断pass fail文件重命名
extern CString pNewName;
extern CString pNewName1;
extern CString ErrorStr;
extern CString ERRORSTR;
void Delay(int  time); //将程序的执行暂停一段时间(毫秒)
typedef enum
{
	TxMsg_OK = 0,
	TxMsg_NOTOK,
	TxMsg_BUSY
}TxMsgStatus;

typedef enum
{
	RxMsg_FREE = 0,
	RxMsg_NOFREE,
	RxMsg_NOCONFIRM,
	RxMsg_OUTTIME
}RxMsgStatus;

typedef struct
{
	TxMsgStatus TxMsg_Status;
	UINT    FFDataSt; //多帧第一帧数据状态
	UINT    ID;
	BYTE    TxMsg[8];
}TxMsgType;

typedef struct
{
	UINT               ID;
	UINT               DLC;//当前报文长度
	UINT               DataLen;//总有效数据长度
	UINT               RxLen;//当前接收到数据长度
	UINT               SN;//多帧偏移量
	RxMsgStatus        RxMsg_Status;
	BYTE               RxMsg[30];
}RxMsgType;  

typedef struct 
{
	UINT          ID;
	RxMsgStatus   CFStatus;
	UINT          CFcount;
}CFType; //多帧连续帧类型

typedef struct
{
	BYTE data[50];
	UINT Len;
}Version;//数据存储


extern UINT ErrorStrFlg; 
extern UINT OverTime;

extern UINT HoneywellFlg;//扫码枪只设置一次设置焦点
extern BYTE Seed[4];
extern TxMsgType TxMsgBuffer[20];
extern RxMsgType RxMsgBuffer[20];
extern CFType    CFtype[20];
extern UINT TxMsgIncount; //发送进栈计数
extern UINT TxMsgOutcount;//发送出栈计数
extern UINT TxMsgCount;//发送计数
extern UINT RxMsgIncount;
extern UINT TestFlg;
extern UINT TestFlg1; //扫码读回车键时判断是否在测试模式
extern UINT SetFlg;
extern UINT ReadFlg;
extern UINT SWVersionFlg;
extern UINT HWVersionFlg;
extern UINT ErrorFlg;
extern UINT ShowRxFlg;
extern UINT ShowTxFlg;
extern BYTE RxShowData[8];
extern BYTE TxShowData[8];

extern BYTE RxCarName[8];
extern BYTE RxCarVIN[17];
extern BYTE RxDiagnoseSN[16];
extern BYTE RxECUMakeDate[4];
extern BYTE RxECUSWDate[4];
extern BYTE RxECUConfigDate[4];
extern BYTE RxMMIConfig[16];
extern BYTE RxNetWorkConfig[8];
extern BYTE RxDVDSN[24];
extern BYTE RxMapSpace[2];
extern BYTE RxSatelliteNumber[1];
//----------------------------------------------------------软件版本号-------------------------------------------------------
//主板软件版本号
extern Version MBSWVersion[50];
//前板系统版本号
extern Version FPOSSWVersion[50];
//前板软件版本号
extern Version FPAPPSWVersion[50];
//蓝牙软件版本号
extern Version BTSWVersion[50];
//导航软件版本号
extern Version NAVISWVersion[50];
//伺服软件版本号
extern Version SVSWVersion[50];
//CAN软件版本号
extern Version CANSWVersion[50];
//ECU软件版本号
extern Version ECUSWVersion[50];
//--------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------硬件版本号------------------------------------------------------------
//主板硬件版本号
extern Version MBHWVersion[50];
//前板硬件版本号
extern Version FPHWVersion[50];
//蓝牙硬件版本号
extern Version BTHWVersion[50];
//导航硬件版本号
extern Version NAVIHWVersion[50];
//伺服硬件版本号
extern Version SVHWVersion[50];
//ECU硬件版本号
extern Version ECUHWVersion[50];
//-----------------------------------------------------------------------------------------------------------------------------------------


extern BYTE PartNumber[10];
extern BYTE SystemSupplierId[6];
////////////////////////////////////////////////////////////////////////////////////////////////
extern UINT DataAnalyzeFlg;
extern UINT Time4;
//
extern UINT ToolModel;
//////////////
//主窗口句柄
extern HWND MainHwnd;
/////////////////////////



class CCANSend
{
	// 构造
public:
	CCANSend(void);	// 标准构造函数
    virtual ~CCANSend(void);	// 标准构造函数
private  :
	VCI_CAN_OBJ Send_Msg;

public:
	void Open_VCIPort(void);
    void Close_VCIPort(void);
	void TxMsgIncountprocess(void);//Tx进栈计数过程
	void TxMsgOutcountprocess(void);//Tx出栈计数过程
	void RxMsgIncountprocess(void);//Rx进栈计数过程
	void Transmit(void);
	void Deal_ReceiveFrameMsg(void);
	void ResetSendBuffer();// 清空发送队列
	void Single_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD = NULL,BYTE *dat = NULL,UINT Len = 0);
// 	void Frist_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD = NULL,BYTE *dat = NULL,UINT Frame_DLC = 0);
// 	void Consecutive_Frame(UINT ID,UINT SN,BYTE *dat,UINT Len);
	void Multi_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD = NULL,BYTE *dat = NULL,UINT Len = 0);
	UINT TxMsgUseType(void);   //多帧连续帧Byte0数据判定
	void Rx_SingleFrame(UINT RxID,BYTE *RxData,UINT DLC);
	void Rx_FristFrame(UINT RxID,BYTE *RxData,UINT DLC);
	void Rx_ConsecutiveFrame(UINT RxID,BYTE *RxData,UINT DLC);
	void RxDataAnalyze(UINT Num);
	void NRSID_7FAnalyze(UINT Num);
	void SID_50Analyze(UINT Num);//进入测试模式命令
	void SID_6EAnalyze(UINT Num);
	void DID_6E_0AAnalyze(UINT Num);
	void SID_62Analyze(UINT Num);
	void DID_62_81Analyze(UINT Num);//软件版本号分析
	void DID_62_82Analyze(UINT Num);//硬件版本号分析
	void DID_62_23Analyze(UINT Num);//获取卫星数量反馈分析
	void DID_50_01(UINT Num);
	void DID_50_02(UINT Num);
	void DID_50_03(UINT Num);
	void DCMNegative_Frame(UINT SID,UINT NRC,UINT DID,UINT DID_CMD = NULL);
	void TPNegative_Frame(UINT NRC);
	UINT CheckPadData(BYTE *data,UINT Len);
	//void Positive_Frame();

	DWORD CalculateStandardFrameAccCode( UINT msg_id )
	{
		return ((msg_id & 0x7FF) << 17) << 4;
	}
};

#endif