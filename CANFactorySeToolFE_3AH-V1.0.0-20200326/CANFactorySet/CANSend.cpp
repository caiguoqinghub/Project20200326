#include "StdAfx.h"
#include "CANSend.h"
#include "ExceptFrameFixer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IS_SET_BUTTON_VALID_MODE() \
	(0 == ToolModel || 2 == ToolModel || 3 == ToolModel)

// #define  E_OK 1
// #define  E_NOT_OK 0
CString Honeywelltest=0;
UINT HoneywellFlg=1;
UINT CanDev_opended = 0;
UINT CanDev_connect = 0;

UINT ErrorStrFlg = 0; 
UINT OverTime = 0;

BYTE Seed[4] = {0,0,0,0};
TxMsgType TxMsgBuffer[20];
	

RxMsgType RxMsgBuffer[20];
CFType    CFtype[20]; //多帧连续帧类型
UINT TxMsgIncount  = 0;
UINT TxMsgOutcount = 0;
UINT TxMsgCount = 0;//Tx信息计数
UINT RxMsgIncount = 0;
UINT TestFlg = 0;
UINT TestFlg1 = 0;
UINT SetFlg = 0;
UINT ReadFlg = 0;
UINT SWVersionFlg = 0;
UINT HWVersionFlg = 0;
UINT ErrorFlg = 0x0;
UINT ShowRxFlg = 0;
UINT ShowTxFlg = 0;
BYTE RxShowData[8];
BYTE TxShowData[8];
BYTE RxCarName[8];
BYTE RxCarVIN[17];
BYTE RxDiagnoseSN[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
BYTE RxECUMakeDate[4];
BYTE RxECUSWDate[4];
BYTE RxECUConfigDate[4];
BYTE RxMMIConfig[16];
BYTE RxNetWorkConfig[8];
BYTE RxDVDSN[24];
BYTE RxMapSpace[2];
BYTE RxSatelliteNumber[1];
//----------------------------------------------------------软件版本号-------------------------------------------------------
//主板软件版本号
Version MBSWVersion[50];
//前板系统版本号
Version FPOSSWVersion[50];
//前板软件版本号
Version FPAPPSWVersion[50];
//蓝牙软件版本号
Version BTSWVersion[50];
//导航软件版本号
Version NAVISWVersion[50];
//伺服软件版本号
Version SVSWVersion[50];
//CAN软件版本号
Version CANSWVersion[50];
//ECU软件版本号
Version ECUSWVersion[50];
//--------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------硬件版本号------------------------------------------------------------
//主板硬件版本号
Version MBHWVersion[50];
//前板硬件版本号
Version FPHWVersion[50];
//蓝牙硬件版本号
Version BTHWVersion[50];
//导航硬件版本号
Version NAVIHWVersion[50];
//伺服硬件版本号
Version SVHWVersion[50];
//ECU硬件版本号
Version ECUHWVersion[50];
//-----------------------------------------------------------------------------------------------------------------------------------------



BYTE PartNumber[10];
BYTE SystemSupplierId[6];
////////////////////////////////////////////////////////////////////////////////////////////////
UINT DataAnalyzeFlg = 0;
UINT Time4 = 0;
//
UINT ToolModel = 0;
////////////////////////////////////
//主窗口句柄
HWND MainHwnd;
/////////////////////////

CCANSend::CCANSend(void)
{
	Send_Msg.TimeStamp = 0;
	Send_Msg.TimeFlag = 0;
	Send_Msg.SendType = 1;
	Send_Msg.RemoteFlag = 0;//是否是远程帧
	Send_Msg.ExternFlag = 0;//是否是扩展帧
	Send_Msg.DataLen = 8;
}
 
CCANSend::~CCANSend(void)
{

}

void CCANSend::Open_VCIPort()
{
	if (CanDev_connect == E_OK)
	{
		return ;
	}

	VCI_INIT_CONFIG init_config;
#if 0
	init_config.AccCode = 0xD1000000;//0x000000000;
#else
	init_config.AccCode = CalculateStandardFrameAccCode(HOST_CAN_ID);
#endif
	//init_config.AccCode =0X000000000;
	init_config.AccMask =  0x001FFFFF;
	init_config.Filter = 0x01;
	init_config.Mode = 0x00;
	init_config.Timing0 = 0x00;
	init_config.Timing1 = 0x1c;

	if (VCI_OpenDevice(VCI_USBCAN1,0,0) != STATUS_OK)
	{
		MessageBox(0,L"VCI设备打开失败！",L"消息",0);
		TRACE(L"设备打开失败\n");
		return;
	}
	if (VCI_InitCAN(VCI_USBCAN1,0,0,&init_config) != STATUS_OK)
	{
		MessageBox(0,L"VCI设备初始化失败！",L"消息",0);
		TRACE(L"设备初始化失败\n");
		return;
	}
	if (VCI_StartCAN(VCI_USBCAN1,0,0))
	{
		TRACE("设备连接成功！\n");
		CanDev_opended = E_OK;
		CanDev_connect = E_OK;
	}
	else{
		MessageBox(0,L"VCI设备连接失败！",L"消息",0);
	}
}
void CCANSend::Close_VCIPort()
{
	if(CanDev_connect == E_OK)
	{
		VCI_CloseDevice(VCI_USBCAN1,0);
		CanDev_opended = E_NOT_OK;
		CanDev_connect = E_NOT_OK;
	}
}

//----------------------------------------------------------------
//
//	TxMsgIncountprocess()当前发送报文一共有所需栈进入个数
//
//----------------------------------------------------------------

void CCANSend::TxMsgIncountprocess()
{
    TxMsgIncount ++;
	if (TxMsgIncount>=20)
	{
		TxMsgIncount = 0;
	}
}

//----------------------------------------------------------------
//
//	TxMsgIncountprocess()当前发送报文一共有所需栈发出个数
//
//----------------------------------------------------------------

void CCANSend::TxMsgOutcountprocess()
{
	TxMsgOutcount ++;
	if (TxMsgOutcount>=20)
	{
		TxMsgOutcount = 0;
	}
}

void CCANSend::RxMsgIncountprocess()
{
	RxMsgIncount ++;
	if (RxMsgIncount>=20)
	{
		RxMsgIncount = 0;
 	}
}


void CCANSend::Transmit()
{
	if (TxMsgCount != 0)
	{
		UINT count;
		for ( count = 0;count<20;count++)
		{
			if (TxMsgBuffer[TxMsgOutcount].TxMsg_Status == TxMsg_NOTOK)
			{
				Send_Msg.ID = TxMsgBuffer[TxMsgOutcount].ID;
				memcpy(Send_Msg.Data,TxMsgBuffer[TxMsgOutcount].TxMsg,8);

				break;
			}
			TxMsgOutcountprocess();
		}
	
		if (VCI_Transmit(VCI_USBCAN1,0,0,&Send_Msg,1)  == STATUS_OK)
		{
			/*OK*/
			ShowTxFlg = 1;
			memcpy(TxShowData,Send_Msg.Data,8);
			TxMsgBuffer[TxMsgOutcount].TxMsg_Status = TxMsg_OK;
			memset(TxMsgBuffer[TxMsgOutcount].TxMsg,0x0,8);
			TxMsgCount --;
			TxMsgOutcountprocess();
		}
		else
		{/*Fail*//*ErrorFlg = 0x01;*/}
	}

}

void CCANSend::ResetSendBuffer()
{
	for (UINT count = 0;count<20;count++) {
		TxMsgBuffer[count].TxMsg_Status = TxMsg_OK;
	}
	TxMsgIncount = 0;
	TxMsgOutcount = 0;
	TxMsgCount = 0;
}

void CCANSend::Single_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD,BYTE *dat,UINT Len)
 {
 	UINT Num;
		fixer.SetSingleFrameRestoreData(ID, SID, DID, DID_CMD, dat, Len);
	for (UINT count = 0;count<20;count++)
	{
		if (TxMsgBuffer[TxMsgIncount].TxMsg_Status == TxMsg_OK)
		{
			break;
		}
		TxMsgIncountprocess();
	}

	TxMsgBuffer[TxMsgIncount].ID = ID;
	TxMsgBuffer[TxMsgIncount].TxMsg_Status = TxMsg_NOTOK;
	TxMsgBuffer[TxMsgIncount].TxMsg[0] = 0x3A;
	TxMsgBuffer[TxMsgIncount].TxMsg[2] = SID;
	TxMsgBuffer[TxMsgIncount].TxMsg[3] = DID;

	if (DID_CMD!=NULL)
	{
		TxMsgBuffer[TxMsgIncount].TxMsg[4] = DID_CMD;
		TxMsgBuffer[TxMsgIncount].TxMsg[1] = Len+3;
		Num = 5;
		Len +=5;
	}else
	{
		TxMsgBuffer[TxMsgIncount].TxMsg[1] = Len+2;
		Num = 4;
		Len+=4;

	}

	if (Len!=0)
	{
		for(;Num<Len;Num++)
		{
			TxMsgBuffer[TxMsgIncount].TxMsg[Num] = *dat;
			dat ++;
		}
	}

	if (Num<8)
	{
		for (;Num<8;Num++)
		{
			TxMsgBuffer[TxMsgIncount].TxMsg[Num] = 0xAA;
		}
	}

	TxMsgIncountprocess();
	if (TxMsgCount<20)
		TxMsgCount++;

 }
 
//  void CCANSend::Frist_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD /* = NULL */,BYTE *dat /* = NULL */,UINT Frame_DLC /* 第一帧的数据长度是传输所有数据的长度 */)
//  {
//  	UINT Num;
//  	memset(Send_Msg.Data,0,sizeof(Send_Msg.Data));
//  	Send_Msg.ID = ID;
//  	Send_Msg.Data[0] = 0x80;
//  
//  	Send_Msg.Data[2] = SID;
//  	Send_Msg.Data[3] = DID;
//  	if (DID_CMD!=NULL)
//  	{
//  		Send_Msg.Data[4] = DID_CMD;
//  		Send_Msg.Data[1] = Frame_DLC+3;
//  		Num = 5;
//  	}else
//  	{
//  		Send_Msg.Data[1] = Frame_DLC+2;
//  		Num = 4;
//  	}
//  	for(;Num<8;Num++)
//  	{
//  		Send_Msg.Data[Num] = *dat;
//  		dat ++;
//  	}
//  
//  	//Transmit();
//  }
 
//  void CCANSend::Consecutive_Frame(UINT ID,UINT SN,BYTE *dat,UINT Len/*连续帧当前帧所需要传输的数据长度*/)
//  {
//  	memset(Send_Msg.Data,0,sizeof(Send_Msg.Data));
//  	UINT Num = 1;
//  	Send_Msg.ID = ID;
//  	Send_Msg.Data[0] = (0xC0|(SN&0x0F));
//  
//  	for(;Num<(Len+1);Num++)
//  	{
//  		Send_Msg.Data[Num] = *dat;
//  		dat ++;
//  	}
//  	if (Num<8)
//  	{
//  		for (;Num<8;Num++)
//  		{
//  			Send_Msg.Data[Num] = 0xAA;
//  		}
//  	}
//  
//  	//Transmit();
//  }
 void CCANSend::Multi_Frame(UINT ID,UINT SID,UINT DID,UINT DID_CMD /* = NULL */,BYTE *dat /* = NULL */,UINT Len /* = 0 */)
 {
	  fixer.SetMultiFrameRestoreData(ID, SID, DID, DID_CMD, dat, Len);
	 UINT Consecutive;//计算连续帧有几行      
	 UINT i,Num;
	 UINT dat_Len;
	 UINT SN = 0x40;
	 UINT messagetype = 0;

	 messagetype = TxMsgUseType();
	 if (messagetype == 1)
	 {
		 return ;
	 }

	 for (UINT count = 0;count<20;count++)
	 {
		 if (TxMsgBuffer[TxMsgIncount].TxMsg_Status == TxMsg_OK)
		 {
			 break;
		 }
		 TxMsgIncountprocess();
	 }
	 TxMsgBuffer[TxMsgIncount].ID = ID;
	 TxMsgBuffer[TxMsgIncount].FFDataSt = messagetype;
	 TxMsgBuffer[TxMsgIncount].TxMsg_Status = TxMsg_NOTOK;
	 TxMsgBuffer[TxMsgIncount].TxMsg[0] = messagetype;
	 TxMsgBuffer[TxMsgIncount].TxMsg[2] = SID;
	 TxMsgBuffer[TxMsgIncount].TxMsg[3] = DID;
	 if (DID_CMD!=NULL)
	 {
		 TxMsgBuffer[TxMsgIncount].TxMsg[4] = DID_CMD;
		 TxMsgBuffer[TxMsgIncount].TxMsg[1] = Len+3;
		 Consecutive =((Len-3)-(Len-3)%7)/7;
		 dat_Len =(Len-3)%7;
		 Num = 5;
	 }else
	 {
		 TxMsgBuffer[TxMsgIncount].TxMsg[1] = Len+2;
		 Consecutive =((Len-4)-(Len-4)%7)/7;
		 dat_Len =(Len-4)%7;
		 Num = 4;
	 }
	 for(;Num<8;Num++)
	 {
		 TxMsgBuffer[TxMsgIncount].TxMsg[Num] = *dat;
		 dat ++;
	 }
	 TxMsgIncountprocess();
	 if (TxMsgCount<20)
		 TxMsgCount++;

	 for(i=0;i < Consecutive;i++)
	 {
		 if (TxMsgBuffer[TxMsgIncount].TxMsg_Status==TxMsg_OK)
		 {
			 TxMsgBuffer[TxMsgIncount].ID = ID;
			 TxMsgBuffer[TxMsgIncount].TxMsg_Status = TxMsg_NOTOK;
			 TxMsgBuffer[TxMsgIncount].TxMsg[0] = messagetype+SN;
			 memcpy(&TxMsgBuffer[TxMsgIncount].TxMsg[1],dat,7);
			 TxMsgIncountprocess();
			 //	Consecutive_Frame(ID,i,dat,7);
			 dat+=7;
			 SN++;
			 if (TxMsgCount<20)
				 TxMsgCount++;
		 }else{
			 return;
		 }
	 }
	 if (dat_Len <= 0)
	 {
		 return ;
	 }else{
		 if (TxMsgBuffer[TxMsgIncount].TxMsg_Status==TxMsg_OK)
		 {
			 TxMsgBuffer[TxMsgIncount].ID = ID;
			 TxMsgBuffer[TxMsgIncount].TxMsg_Status = TxMsg_NOTOK;
			 TxMsgBuffer[TxMsgIncount].TxMsg[0] = messagetype+SN;
			 memcpy(&TxMsgBuffer[TxMsgIncount].TxMsg[1],dat,dat_Len);

			 for (UINT Pad = (dat_Len+1);Pad<8;Pad++)
			 {
				 TxMsgBuffer[TxMsgIncount].TxMsg[Pad] = 0xAA;
			 }
			 TxMsgIncountprocess();
			 if (TxMsgCount<20)
				 TxMsgCount++;
		 }else{
			 return;
		 }
	 }
	 //Consecutive_Frame(ID,i,dat,dat_Len);
 }

 
 UINT CCANSend::TxMsgUseType()
 {
 	UINT messagetype = 0;
 	UINT typ[4] = {0,0,0,0};
 	for (UINT count = 0;count<20;count++)
 	{
 		if (TxMsgBuffer[count].TxMsg_Status == TxMsg_NOTOK)
 		{
 			switch (TxMsgBuffer[count].FFDataSt)
 			{
 			case  FF8 :
 				typ[0] = FF8;
 				break;
 			case FF9 :
 				typ[1] = FF9;
 				break;
 			case FFA :
 				typ[2] = FFA;
 				break;
 			case FFB :
 				typ[3] = FFB;
 				break;
 			default:
 				break;
 			}
 
 		}
 
 		if (typ[0] == FF8 && typ[1] == FF9
 		  &&typ[2] == FFA && typ[3] == FFB)
 		{
 			messagetype = 0x01;
 			break;
 		}
 	}
 
 	if (messagetype!=0x01)
 	{
 		if (typ[0] == 0x00)
 		{
 			messagetype = FF8;
 		}else if (typ[1] == 0x00)
 		{
 			messagetype = FF9;
 		}else if (typ[2] == 0x00)
 		{
 			messagetype = FFA;
 		}else if (typ[3] == 0x00)
 		{
 			messagetype = FFB;
 		}
 	}
 	return messagetype;
 
 }
 
 void CCANSend::DCMNegative_Frame(UINT SID,UINT NRC,UINT DID,UINT DID_CMD/* = NULL*/)
 {
 	UINT Num;
	for (UINT count = 0;count<20;count++)
	{
		if (TxMsgBuffer[TxMsgIncount].TxMsg_Status == TxMsg_OK)
		{
			break;
		}
		TxMsgIncountprocess();
	}

	TxMsgBuffer[TxMsgIncount].ID = 0x680;
	TxMsgBuffer[TxMsgIncount].TxMsg_Status = TxMsg_NOTOK;
	TxMsgBuffer[TxMsgIncount].TxMsg[0] = 0x3A;
 	if (DID_CMD == NULL)
 	{
 		Num = 6;
 		TxMsgBuffer[TxMsgIncount].TxMsg[1] = 0x04;
		TxMsgBuffer[TxMsgIncount].TxMsg[6] = 0xAA;
 
 	}else{
 		Num = 7;
 		TxMsgBuffer[TxMsgIncount].TxMsg[1] = 0x05;
 		TxMsgBuffer[TxMsgIncount].TxMsg[6] = DID_CMD;
 	}
 	TxMsgBuffer[TxMsgIncount].TxMsg[2] = 0x7F;
 	TxMsgBuffer[TxMsgIncount].TxMsg[3] = SID;
 	TxMsgBuffer[TxMsgIncount].TxMsg[4] = NRC;
 	TxMsgBuffer[TxMsgIncount].TxMsg[5] = DID;
    TxMsgBuffer[TxMsgIncount].TxMsg[7] = 0xAA;
	TxMsgIncountprocess();
	if (TxMsgCount<20)
		TxMsgCount++;
 }

 void CCANSend::TPNegative_Frame(UINT NRC)
 {
//	 UINT Num;
	 for (UINT count = 0;count<20;count++)
	 {
		 if (TxMsgBuffer[TxMsgIncount].TxMsg_Status == TxMsg_OK)
		 {
			 break;
		 }
		 TxMsgIncountprocess();
	 }

	 TxMsgBuffer[TxMsgIncount].ID = 0x680;
	 TxMsgBuffer[TxMsgIncount].TxMsg_Status = TxMsg_NOTOK;
	 TxMsgBuffer[TxMsgIncount].TxMsg[0] = 0x3A;
	 TxMsgBuffer[TxMsgIncount].TxMsg[1] = 0x02;
	 TxMsgBuffer[TxMsgIncount].TxMsg[2] = 0x7F;
	 TxMsgBuffer[TxMsgIncount].TxMsg[3] = NRC;
	 TxMsgBuffer[TxMsgIncount].TxMsg[4] = 0xAA;
	 TxMsgBuffer[TxMsgIncount].TxMsg[5] = 0xAA;
	 TxMsgBuffer[TxMsgIncount].TxMsg[6] = 0xAA;
	 TxMsgBuffer[TxMsgIncount].TxMsg[7] = 0xAA;
	 TxMsgIncountprocess();
	 if (TxMsgCount<20)
		 TxMsgCount++;
 }

 void CCANSend::Deal_ReceiveFrameMsg()
 {
	 VCI_CAN_OBJ  Receive_Buf;
	 VCI_ERR_INFO Error_Info;
	 int len = 1;	
	 len = VCI_Receive(VCI_USBCAN1,0,0,&Receive_Buf,1,NULL);
	 if (len <= 0){
		 VCI_ReadErrInfo(VCI_USBCAN1,0,0,&Error_Info);
		 return;
	 }
	 if (Receive_Buf.ID == HOST_CAN_ID)
	 {
		 ShowRxFlg = 1;
		 memcpy(RxShowData,Receive_Buf.Data,8);
		 switch (Receive_Buf.Data[0]&0xF0)
		 {
		 case SF3:
			 Rx_SingleFrame(Receive_Buf.ID,Receive_Buf.Data,Receive_Buf.DataLen);
			 break;
		 case FF8:
		 case FF9:
		 case FFA:
		 case FFB:
			 Rx_FristFrame(Receive_Buf.ID,Receive_Buf.Data,Receive_Buf.DataLen);
			 break;
		 case CFC:
		 case CFD:
		 case CFE:
		 case CFF:
			 Rx_ConsecutiveFrame(Receive_Buf.ID,Receive_Buf.Data,Receive_Buf.DataLen);
			 break;
		 default:
			 break;
		 }

	 }
	 return;
 }

 void CCANSend::Rx_SingleFrame(UINT RxID,BYTE *RxData,UINT DLC)
 {
	 if (*RxData != 0x3A)
	 {
		 TPNegative_Frame(0x71);
		 return ;
	 }
	 if ((*(RxData+1) >=7)||(*(RxData+1) < 2))
	 {
		   TPNegative_Frame(0x74);
		 return ;
	 }

	 for (UINT count = 0;count < 20; count++)
	 {
		 if (RxMsgBuffer[RxMsgIncount].RxMsg_Status == RxMsg_FREE)
		 {
			break;
		 }
		 RxMsgIncountprocess();
	 }
	 RxMsgBuffer[RxMsgIncount].ID = RxID;
	 RxMsgBuffer[RxMsgIncount].DLC = DLC;
	 if (!CheckPadData(RxData,*(RxData+1)))
	 {
		 memcpy(RxMsgBuffer[RxMsgIncount].RxMsg,(RxData+1),*(RxData+1)+1);
		 RxMsgBuffer[RxMsgIncount].RxMsg_Status = RxMsg_NOFREE;
		 RxDataAnalyze(RxMsgIncount);
	 }else{
		 return ;
	 }

	 RxMsgIncountprocess();
 }


//填充位是否正确
UINT CCANSend::CheckPadData(BYTE *data,UINT Len)
 {

	 for (UINT Pad = (Len+2);Pad<8;Pad++)
	 {
	      if (data[Pad] != 0xAA)
	      {
			  TPNegative_Frame(0x75);  
			  return 1 ;
			  break;
	      }
	 }
	 return 0 ;
 }

void CCANSend::Rx_FristFrame(UINT RxID,BYTE *RxData,UINT DLC)
{
	UINT count;
	if ((*RxData&0x0F) != 0x00)//第一帧 低四位为 0
	{
		TPNegative_Frame(0x71);
		return ;
	}else if (*(RxData+1)<2)
	{
		TPNegative_Frame(0x74);
		return ;
	}

	for ( count = 0;count<20;count++)
	{
		if (RxMsgBuffer[RxMsgIncount].RxMsg_Status == RxMsg_FREE)
		{
			break;
		}
		RxMsgIncountprocess();
	}

	RxMsgBuffer[RxMsgIncount].ID = RxID;
	RxMsgBuffer[RxMsgIncount].DLC = DLC;
    if (*(RxData+1)<=6)
    {
		if (!CheckPadData(RxData,*(RxData+1)))
		{
			memcpy(RxMsgBuffer[RxMsgIncount].RxMsg,(RxData+1),*(RxData+1)+1);
			RxMsgBuffer[RxMsgIncount].RxMsg_Status = RxMsg_NOFREE;
			RxDataAnalyze(RxMsgIncount);
		}else{

		}
    }else{
			memcpy(RxMsgBuffer[RxMsgIncount].RxMsg,(RxData+1),DLC-1);
			RxMsgBuffer[RxMsgIncount].DataLen = *(RxData+1)+1;
			RxMsgBuffer[RxMsgIncount].RxLen = (DLC-1);
			RxMsgBuffer[RxMsgIncount].SN = *RxData+0x40;
			RxMsgBuffer[RxMsgIncount].RxMsg_Status = RxMsg_NOCONFIRM;
		 for (count = 0; count<20;count++)
		 {
			 if (CFtype[count].CFStatus == RxMsg_FREE)
			 {
				 CFtype[count].ID = RxID;
				 CFtype[count].CFStatus = RxMsg_NOFREE ;
				 CFtype[count].CFcount = RxMsgIncount;
				 break;
			 }
		 }
	}

	RxMsgIncountprocess();
}


void CCANSend::Rx_ConsecutiveFrame(UINT RxID,BYTE *RxData,UINT DLC)
{
	UINT count ;
	for ( count = 0;count < 20;count++)
	{
		if ( CFtype[count].CFStatus == RxMsg_NOFREE
			&&RxMsgBuffer[CFtype[count].CFcount].RxMsg_Status == RxMsg_NOCONFIRM
			&&RxMsgBuffer[CFtype[count].CFcount].SN == *RxData 
			&&RxMsgBuffer[CFtype[count].CFcount].ID ==  RxID)
		{
			break;
		}
	}
	if (CFtype[count].CFStatus != RxMsg_NOFREE)
	{
		return ;
	}

	if (RxMsgBuffer[CFtype[count].CFcount].DataLen>RxMsgBuffer[CFtype[count].CFcount].RxLen+(DLC-1))
	{
		memcpy(&RxMsgBuffer[CFtype[count].CFcount].RxMsg[RxMsgBuffer[CFtype[count].CFcount].RxLen],RxData+1,(DLC-1));
		RxMsgBuffer[CFtype[count].CFcount].RxLen = RxMsgBuffer[CFtype[count].CFcount].RxLen+(DLC-1);
		if ((RxMsgBuffer[CFtype[count].CFcount].SN&0x0F)<0x0F)
		RxMsgBuffer[CFtype[count].CFcount].SN++;

	}else if (RxMsgBuffer[CFtype[count].CFcount].DataLen<=RxMsgBuffer[CFtype[count].CFcount].RxLen+(DLC-1))
	{
		if (!CheckPadData(RxData,(RxMsgBuffer[CFtype[count].CFcount].DataLen - RxMsgBuffer[CFtype[count].CFcount].RxLen)-1))
		{
			memcpy(&RxMsgBuffer[CFtype[count].CFcount].RxMsg[RxMsgBuffer[CFtype[count].CFcount].RxLen],RxData+1,(RxMsgBuffer[CFtype[count].CFcount].DataLen - RxMsgBuffer[CFtype[count].CFcount].RxLen));
			CFtype[count].CFStatus = RxMsg_FREE;
			RxMsgBuffer[CFtype[count].CFcount].RxMsg_Status = RxMsg_NOFREE;
			RxDataAnalyze(CFtype[count].CFcount);
		}
	}




}

void CCANSend::RxDataAnalyze(UINT Num)
{
	if ((RxMsgBuffer[Num].RxMsg[0]+1)>30)
	{
		return ;
	}
	switch(RxMsgBuffer[Num].RxMsg[1])
	{
	case 0x21:
		//
		break;
	case 0x50 :
		SID_50Analyze(Num);
		break;
	case 0x6E:
		SID_6EAnalyze(Num);
		break;
	case 0x62:
		SID_62Analyze(Num);
		break;
	case 0x7F:
		//
		NRSID_7FAnalyze(Num);
		break;
	default:
       DCMNegative_Frame(RxMsgBuffer[Num].RxMsg[1],0x11,RxMsgBuffer[Num].RxMsg[2],NULL);
		break;
	}

}

void CCANSend::NRSID_7FAnalyze(UINT Num)
{
	if (RxMsgBuffer[Num].RxMsg[0] == 2)
	{
		switch(RxMsgBuffer[Num].RxMsg[2])
		{
		case 0x71:
             ErrorFlg = 0x71;
			 break;
		case 0x72:
			ErrorFlg = 0x72;
			break;
		case 0x73:
			ErrorFlg = 0x73;
			break;
		case 0x74:
			ErrorFlg = 0x74;
			break;
		case 0x75:
			ErrorFlg = 0x75;
			break;
		default:
			break;
		}
	}else{
		switch(RxMsgBuffer[Num].RxMsg[3])
		{
		case 0x11:
			ErrorFlg = 0x11;
			break;
		case 0x12:
			ErrorFlg = 0x12;
			break;
		case 0x40:
			ErrorFlg = 0x40;
			break;
		case 0x13:
			ErrorFlg = 0x13;
			break;
		case 0x31:
			ErrorFlg = 0x31;
			break;
		case 0x32:
			ErrorFlg = 0x32;
			break;
		case 0x35:
			ErrorFlg = 0x35;
			break;
		case 0x41:
			ErrorFlg = 0x41;
			break;
		case 0x80:
			ErrorFlg = 0x80;
			break;
		default:
			break;
		}
	}
	RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;

}

void CCANSend::SID_50Analyze(UINT Num)
{
	switch (RxMsgBuffer[Num].RxMsg[2])
	{
	case 0x01:
		DID_50_01(Num);
		break;
	case 0x02:
		DID_50_02(Num);
		break;
	case 0x03:
		DID_50_03(Num);
		break;
	default:
	 DCMNegative_Frame(RxMsgBuffer[Num].RxMsg[1],0x12,RxMsgBuffer[Num].RxMsg[2],NULL);
	 break;

	}
}

void CCANSend::SID_6EAnalyze(UINT Num)
{
	switch(RxMsgBuffer[Num].RxMsg[2])
	{
	case  0x84:
		//
		{
			SetFlg = 0x84;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0xA0:
		//
		{
			SetFlg = 0xA0;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0xA1:
		//
		{
			SetFlg = 0xA1;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0xA2:
		//
		{
			SetFlg = 0xA2;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0xA3:
		//
		{
			SetFlg = 0xA3;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0xA4:
		//
		{
			SetFlg = 0xA4;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0xA5:
		//
		{
			SetFlg = 0xA5;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0xA6:
		//
		{
			SetFlg = 0xA6;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0xA7:
		//
		{
			SetFlg = 0xA7;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	//case  0xA8:
	//	//
	//	{
	//		SetFlg = 0xA8;
	//		RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
	//	}
	//	break;
	case  0xA9:
		//
		{
			SetFlg = 0xA9;
			RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
		}
		break;
	case  0x0A:
		//
		{
		//	DID_6E_0AAnalyze(Num);
		}break;
	default:
		//
		break;
	}
}

void CCANSend::DID_6E_0AAnalyze(UINT Num)
{
	switch (RxMsgBuffer[Num].RxMsg[3])
	{
	case 0x00:
		break;
	case 0x01:
		SetFlg = 0x0A01;
		break;
	case 0x02:
		break;
	default:
		break;
	}
	RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
}

void CCANSend::SID_62Analyze(UINT Num)
{
	switch(RxMsgBuffer[Num].RxMsg[2])
	{
	case 0x23:
		//获取卫星数量反馈
		{
			DID_62_23Analyze(Num);
			ReadFlg = 0x23;
		}
		 break;
	case 0x26:
		//Read蓝牙设备地址回应
		{
			ReadFlg = 0x26;
			memcpy(RxMapSpace,&RxMsgBuffer[Num].RxMsg[3],2);
		}
		break;
	case 0x81:
		//读软件版本信息反馈
		{
			DID_62_81Analyze(Num);
			ReadFlg = 0x81;
		}break;
	case 0x82:
		//读硬件版本信息反馈
		{
			DID_62_82Analyze(Num);
			ReadFlg = 0x82;
		}
		break;
	case 0x84:
		//读机器序列号反馈
		{
			ReadFlg = 0x84;
			memcpy(RxDVDSN,&RxMsgBuffer[Num].RxMsg[3],24);
		}
		break;
	case  0xA0:
		//读车辆名称反馈
		{
			ReadFlg = 0xA0;
			memcpy(RxCarName,&RxMsgBuffer[Num].RxMsg[3],8);
		}
		break;
	case  0xA1:
		//读车辆VIN码反馈
		{
			ReadFlg = 0xA1;
			memcpy(RxCarVIN,&RxMsgBuffer[Num].RxMsg[3],17);
		}
		break;
	case  0xA2:
		//诊断仪序列号
		{
			ReadFlg = 0xA2;
			memcpy(RxDiagnoseSN,&RxMsgBuffer[Num].RxMsg[3],16);
		}
		break;
	case  0xA3:
		//
		break;
	case  0xA4:
		//ECU制造日期回应
		{
			ReadFlg = 0xA4;
			memcpy(RxECUMakeDate,&RxMsgBuffer[Num].RxMsg[3],4);
		}
		break;
	case  0xA5:
		//ECU软件刷新日期回应
		{
			ReadFlg = 0xA5;
			memcpy(RxECUSWDate,&RxMsgBuffer[Num].RxMsg[3],4);
		}
		break;
	case  0xA6:
		//ECU装配日期回应
		{
			ReadFlg = 0xA6;
			memcpy(RxECUConfigDate,&RxMsgBuffer[Num].RxMsg[3],4);
		}
		break;
	case  0xA7:
		//
		break;
	//case  0xA8:
	//	//MMI配置功能反馈
	//	{
	//		ReadFlg = 0xA8;
	//		memcpy(RxMMIConfig,&RxMsgBuffer[Num].RxMsg[3],16);
	//	}
	//	break;
	case  0xA9:
		//网络架构配置反馈
		{
			ReadFlg = 0xA9;
			memcpy(RxNetWorkConfig,&RxMsgBuffer[Num].RxMsg[3],8);
		}
		break;
	case  0xAA:
		// 零件号/零件BOM号
		{
			ReadFlg = 0xAA;
			memcpy(PartNumber,&RxMsgBuffer[Num].RxMsg[3],10);
		}
		break;
	case  0xAB:
		//供应商识别号
		{
			ReadFlg = 0xAB;
			memcpy(SystemSupplierId,&RxMsgBuffer[Num].RxMsg[3],6);
		}
		break;
	default:
		//
		break;
	}
}

void CCANSend::DID_62_81Analyze(UINT Num)
{
	switch (RxMsgBuffer[Num].RxMsg[3])
	{
	case 0x00:
		//
		break;
	case 0x01:
		//主板软件版本号
		{
			memcpy(MBSWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			MBSWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			SWVersionFlg = 1;
		}
		break;
	case 0x02:
		//前板系统版本号
		{
			memcpy(FPOSSWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			FPOSSWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			SWVersionFlg = 2;
		}
		break;
	case 0x03:
		//前板软件版本号
		{
			memcpy(FPAPPSWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			FPAPPSWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			SWVersionFlg = 3;
		}
		break;
	case 0x04:
		//蓝牙软件版本号
		{
			memcpy(BTSWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			BTSWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			SWVersionFlg = 4;
		}
		break;
	case 0x05:
		//导航软件版本号

		{
			memcpy(NAVISWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			NAVISWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			SWVersionFlg = 5;
		}
		break;
	case 0x06:
		//伺服软件版本号

		{
			memcpy(SVSWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			SVSWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			SWVersionFlg = 6;
		}
		break;
	case 0x07:
		//CAN软件版本号

		{
			memcpy(CANSWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			CANSWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			SWVersionFlg = 7;
		}
		break;
	case 0x08:
		// 供应商ECU软件版本号

		{
			memcpy(ECUSWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			ECUSWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			SWVersionFlg = 8;
		}
		break;
	default:
		break;
	}
   RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
}


void CCANSend::DID_62_82Analyze(UINT Num)
{
	switch (RxMsgBuffer[Num].RxMsg[3])
	{
	case 0x00:
		//
		break;
	case 0x01:
		//主板硬件版本号
		{
			memcpy(MBHWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			MBHWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			HWVersionFlg = 1;
		}
		break;
	case 0x02:
		//前板硬件版本号
		{
			memcpy(FPHWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			FPHWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			HWVersionFlg = 2;
		}
		break;
	case 0x03:
		//蓝牙硬件版本号
		{
			memcpy(BTHWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			BTHWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			HWVersionFlg = 3;
		}
		break;
	case 0x04:
		//导航硬件版本号
		{
			memcpy(NAVIHWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			NAVIHWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			HWVersionFlg = 4;
		}
		break;
	case 0x05:
		//伺服硬件版本号
		{
			memcpy(SVHWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			SVHWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			HWVersionFlg = 5;
		}
		break;
	case 0x06:
		//
		break;
	case 0x07:
		//
		break;
	case 0x08:
		//ECU硬件版本号
		{
			memcpy(ECUHWVersion->data,&RxMsgBuffer[Num].RxMsg[4],(RxMsgBuffer[Num].RxMsg[0]-3));
			ECUHWVersion->Len = (RxMsgBuffer[Num].RxMsg[0]-3);
			HWVersionFlg = 8;
		}
		break;
	default:
		break;
	}
	RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
}

void CCANSend::DID_50_01(UINT Num)
{
   if (RxMsgBuffer[Num].RxMsg[0]!=0x06)
   {
	   return ;
   }
       memcpy(Seed,&RxMsgBuffer[Num].RxMsg[3],4);
	   TestFlg = 0x01;
	   RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
	   return ;
 
}

void CCANSend::DID_50_02(UINT Num)
{
	switch (RxMsgBuffer[Num].RxMsg[3])
	{
	case 0x00:
		//
		TestFlg = 0x020;
		break;
	case  0x01:
		//
		TestFlg = 0x021;
		break;
	case 0x02:
		//
		TestFlg = 0x022;
		break;
	}
	  RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
	  return ;
}

void CCANSend::DID_50_03(UINT Num)
{
	TestFlg = 0x03;
	RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
	return ;
}


void   Delay(int   time)//time*1000为秒数 
{ 
	clock_t   now   =   clock(); 

	while(   clock()   -   now   <   time   ); 
} 

void CCANSend::DID_62_23Analyze(UINT Num)
{
	switch (RxMsgBuffer[Num].RxMsg[3])
	{
	case 0x00:
		//
		break;
	case 0x01:
		//
		break;
	case 0x02:
		//
		break;
	case 0x03:
		//
		{
			memcpy(RxSatelliteNumber,&RxMsgBuffer[Num].RxMsg[4],1);
		}
		break;
	case 0x04:
		//
		break;
	default:
		break;
	}
	RxMsgBuffer[Num].RxMsg_Status = RxMsg_FREE ;
}