#pragma once

#ifndef  __CANSEND_H__
#define  __CANSEND_H__
#include "ECanVci.H"


#define TestID		REQUEST_CAN_ID

#define  SF3   0x30  //��֡������
#define  FF8   0x80  //��֡��һ֡����һ�֣�������
#define  FF9   0x90  //��֡��һ֡���ڶ��֣�������
#define  FFA   0xA0  //��֡��һ֡�������֣�������
#define  FFB   0xB0  //��֡��һ֡�������֣�������
#define  CFC   0xC0  //��֡����֡����һ�֣�������
#define  CFD   0xD0  //��֡����֡���ڶ��֣�������
#define  CFE   0xE0  //��֡����֡�������֣�������
#define  CFF   0xF0  //��֡����֡�������֣�������
#define WM_SUB_MSG  WM_USER+0x02001


#define  E_OK 1
#define  E_NOT_OK 0

extern UINT CanDev_opended;
extern UINT CanDev_connect;
extern CString Honeywelltest;
//�ж�pass fail�ļ�������
extern CString pNewName;
extern CString pNewName1;
extern CString ErrorStr;
extern CString ERRORSTR;
void Delay(int  time); //�������ִ����ͣһ��ʱ��(����)
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
	UINT    FFDataSt; //��֡��һ֡����״̬
	UINT    ID;
	BYTE    TxMsg[8];
}TxMsgType;

typedef struct
{
	UINT               ID;
	UINT               DLC;//��ǰ���ĳ���
	UINT               DataLen;//����Ч���ݳ���
	UINT               RxLen;//��ǰ���յ����ݳ���
	UINT               SN;//��֡ƫ����
	RxMsgStatus        RxMsg_Status;
	BYTE               RxMsg[30];
}RxMsgType;  

typedef struct 
{
	UINT          ID;
	RxMsgStatus   CFStatus;
	UINT          CFcount;
}CFType; //��֡����֡����

typedef struct
{
	BYTE data[50];
	UINT Len;
}Version;//���ݴ洢


extern UINT ErrorStrFlg; 
extern UINT OverTime;

extern UINT HoneywellFlg;//ɨ��ǹֻ����һ�����ý���
extern BYTE Seed[4];
extern TxMsgType TxMsgBuffer[20];
extern RxMsgType RxMsgBuffer[20];
extern CFType    CFtype[20];
extern UINT TxMsgIncount; //���ͽ�ջ����
extern UINT TxMsgOutcount;//���ͳ�ջ����
extern UINT TxMsgCount;//���ͼ���
extern UINT RxMsgIncount;
extern UINT TestFlg;
extern UINT TestFlg1; //ɨ����س���ʱ�ж��Ƿ��ڲ���ģʽ
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
//----------------------------------------------------------����汾��-------------------------------------------------------
//��������汾��
extern Version MBSWVersion[50];
//ǰ��ϵͳ�汾��
extern Version FPOSSWVersion[50];
//ǰ������汾��
extern Version FPAPPSWVersion[50];
//��������汾��
extern Version BTSWVersion[50];
//��������汾��
extern Version NAVISWVersion[50];
//�ŷ�����汾��
extern Version SVSWVersion[50];
//CAN����汾��
extern Version CANSWVersion[50];
//ECU����汾��
extern Version ECUSWVersion[50];
//--------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------Ӳ���汾��------------------------------------------------------------
//����Ӳ���汾��
extern Version MBHWVersion[50];
//ǰ��Ӳ���汾��
extern Version FPHWVersion[50];
//����Ӳ���汾��
extern Version BTHWVersion[50];
//����Ӳ���汾��
extern Version NAVIHWVersion[50];
//�ŷ�Ӳ���汾��
extern Version SVHWVersion[50];
//ECUӲ���汾��
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
//�����ھ��
extern HWND MainHwnd;
/////////////////////////



class CCANSend
{
	// ����
public:
	CCANSend(void);	// ��׼���캯��
    virtual ~CCANSend(void);	// ��׼���캯��
private  :
	VCI_CAN_OBJ Send_Msg;

public:
	void Open_VCIPort(void);
    void Close_VCIPort(void);
	void TxMsgIncountprocess(void);//Tx��ջ��������
	void TxMsgOutcountprocess(void);//Tx��ջ��������
	void RxMsgIncountprocess(void);//Rx��ջ��������
	void Transmit(void);
	void Deal_ReceiveFrameMsg(void);
	void ResetSendBuffer();// ��շ��Ͷ���
	void Single_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD = NULL,BYTE *dat = NULL,UINT Len = 0);
// 	void Frist_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD = NULL,BYTE *dat = NULL,UINT Frame_DLC = 0);
// 	void Consecutive_Frame(UINT ID,UINT SN,BYTE *dat,UINT Len);
	void Multi_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD = NULL,BYTE *dat = NULL,UINT Len = 0);
	UINT TxMsgUseType(void);   //��֡����֡Byte0�����ж�
	void Rx_SingleFrame(UINT RxID,BYTE *RxData,UINT DLC);
	void Rx_FristFrame(UINT RxID,BYTE *RxData,UINT DLC);
	void Rx_ConsecutiveFrame(UINT RxID,BYTE *RxData,UINT DLC);
	void RxDataAnalyze(UINT Num);
	void NRSID_7FAnalyze(UINT Num);
	void SID_50Analyze(UINT Num);//�������ģʽ����
	void SID_6EAnalyze(UINT Num);
	void DID_6E_0AAnalyze(UINT Num);
	void SID_62Analyze(UINT Num);
	void DID_62_81Analyze(UINT Num);//����汾�ŷ���
	void DID_62_82Analyze(UINT Num);//Ӳ���汾�ŷ���
	void DID_62_23Analyze(UINT Num);//��ȡ����������������
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