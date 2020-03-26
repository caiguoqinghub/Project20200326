#ifndef CONTROLCAN_H
#define CONTROLCAN_H

//接口卡类型定义
#define VCI_PCI		0
#define VCI_232CAN		1
#define VCI_485CAN		2
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4


//CAN错误码
#define	ERR_CAN_OVERFLOW			0x0001	//CAN控制器内部FIFO溢出
#define	ERR_CAN_ERRALARM			0x0002	//CAN控制器错误报警
#define	ERR_CAN_PASSIVE				0x0004	//CAN控制器消极错误
#define	ERR_CAN_LOSE				0x0008	//CAN控制器仲裁丢失
#define	ERR_CAN_BUSERR				0x0010	//CAN控制器总线错误
#define ERR_CAN_BUSOFF				0x0020 //总线关闭错误
//通用错误码
#define	ERR_DEVICEOPENED			0x0100	//设备已经打开
#define	ERR_DEVICEOPEN				0x0200	//打开设备错误
#define	ERR_DEVICENOTOPEN			0x0400	//设备没有打开
#define	ERR_BUFFEROVERFLOW			0x0800	//缓冲区溢出
#define	ERR_DEVICENOTEXIST			0x1000	//此设备不存在
#define	ERR_LOADKERNELDLL			0x2000	//装载动态库失败
#define ERR_CMDFAILED				0x4000	//执行命令失败错误码
#define	ERR_BUFFERCREATE			0x8000	//内存不足


//函数调用返回状态值
#define	STATUS_OK					1
#define STATUS_ERR					0
	
#define CMD_DESIP			0
#define CMD_DESPORT			1
#define CMD_CHGDESIPANDPORT		2
#define CMD_SRCPORT			2		
#define CMD_TCP_TYPE		4					//tcp 工作方式，服务器:1 或是客户端:0
#define TCP_CLIENT			0
#define TCP_SERVER			1
//服务器方式下有效
#define CMD_CLIENT_COUNT    5					//连接上的客户端计数
#define CMD_CLIENTS			6					//连接上的客户端
#define CMD_DISCONN_CLINET  7					//断开一个连接

typedef struct tagRemoteClient{
	int iIndex;
	DWORD port;
	HANDLE hClient;
	char szip[32];
}REMOTE_CLIENT;


//1.ZLGCAN系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
		USHORT	hw_Version;
		USHORT	fw_Version;
		USHORT	dr_Version;
		USHORT	in_Version;
		USHORT	irq_Num;
		BYTE	can_Num;
		CHAR	str_Serial_Num[20];
		CHAR	str_hw_Type[40];
		USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
	UINT	ID;
	UINT	TimeStamp;
	BYTE	TimeFlag;
	BYTE	SendType;
	BYTE	RemoteFlag;//是否是远程帧
	BYTE	ExternFlag;//是否是扩展帧
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
	UCHAR	ErrInterrupt;
	UCHAR	regMode;
	UCHAR	regStatus;
	UCHAR	regALCapture;
	UCHAR	regECCapture; 
	UCHAR	regEWLimit;
	UCHAR	regRECounter; 
	UCHAR	regTECounter;
	DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.定义错误信息的数据类型。
typedef struct _VCI_ERR_INFO{
		UINT	ErrCode;
		BYTE	Passive_ErrData[3];
		BYTE	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.定义初始化CAN的数据类型
typedef struct _VCI_INIT_CONFIG{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	Reserved;
	UCHAR	Filter;
	UCHAR	Timing0;	
	UCHAR	Timing1;	
	UCHAR	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

typedef struct _tagChgDesIPAndPort
{
	char szpwd[10];
	char szdesip[20];
	int desport;
	BYTE blistenonly;
}CHGDESIPANDPORT;

///////// new add struct for filter /////////
typedef struct _VCI_FILTER_RECORD{
	DWORD ExtFrame;	//是否为扩展帧
	DWORD Start;
	DWORD End;
}VCI_FILTER_RECORD,*PVCI_FILTER_RECORD;


typedef struct MsgTx6012_Safe_Fault_MsgTypeTag
{ /*Safe_Fault*/
	UINT unused0			          : 8; 
	UINT unused1                   : 8;
	UINT unused2                   : 8; 
	UINT unused3                   : 8;
	UINT Safe_Fault				  : 2;
	UINT unused4                   : 6;
	UINT unused5                   : 8;
	UINT unused6                   : 8; 
	UINT unused7                   : 8;


}MsgTx6012_Safe_Fault_MsgType;

typedef struct MsgTx6012_Park_Status_MsgTypeTag
{ /*ParkStatus*/
	UINT unused0             : 8;
	UINT unused1             : 8;
	UINT Oil_Warning_Flag    : 1;
	UINT unused2             : 6;
	UINT ParkStatus          : 1;
	UINT Car_Speed_Byte1     : 8;
	UINT unused3             : 8;
	UINT unused4             : 8;
	UINT Car_Speed_Byte2     : 8; 
	UINT unused6             : 8;
} MsgTx6012_Park_Status_MsgType;

typedef struct MsgTx6012_light_Control_1MsgTypeTag
{ /*light_COntrol*/
	UINT unused0             : 8;  
	UINT light_COntrol_Byte1 : 8;
	UINT light_COntrol_Byte  : 8;
	UINT unused1             : 8;
	UINT unused2             : 8;
	UINT unused3             : 8;
	UINT unused4             : 8;
	UINT unused5             : 8;

} MsgTx6012_Light_Control_1_MsgType;

typedef struct MsgTx6012_KeySt_MsgTypeTag
{ /*KeySt*/
	UINT unused0			    : 8;   
	UINT unused1             : 8;
	UINT KeySt               : 2;
	UINT unused2             : 6;
	UINT unused3             : 8;  
} MsgTx6012_KeySt_MsgType;

typedef struct MsgTx6012_Oil_Pressure_MsgTypeTag
{ /*Oil_Pressure*/
	UINT unused0			          : 8;
	UINT unused1                   : 8;
	UINT unused2                   : 8;
	UINT unused3                   : 8; 
	UINT unused4                   : 8;  
	UINT unused5                   : 8;
	UINT unused6                   : 2;
	UINT Engine_Temp_High          : 1;
	UINT unused7                   : 5;
	UINT Machine_Oil_Low           : 1;
	UINT unused8                   : 3;
	UINT Oil_Pressure              : 1;
	UINT unused9                   : 3;
} MsgTx6012_Oil_Pressure_MsgType;


typedef struct MsgTx6012_ST_GearLeverPos_MsgTypeTag
{
	UINT unused0			                   : 8; 
	UINT unused1                            : 8;
	UINT NMC_Engine_Speed_Byte0             : 8;
	UINT NMC_Engine_Speed_Byte1             : 8;
	UINT unused6                            : 8;
	UINT ST_GearLeverPos                    : 4;
	UINT unused5                            : 4;
	UINT unused7                            : 8; 
	UINT unused8                            : 8; 
} MsgTx6012_ST_GearLeverPos_MsgType;

typedef struct MsgTx6012_Safety_MsgTypeTag
{ /*Safety*/
	UINT unused0		    : 8; 
	UINT unused1             : 8;
	UINT unused2             : 8;
	UINT unused3             : 8;
	UINT Safety              : 4;
	UINT unused5             : 4;
	UINT unused6             : 8;
	UINT unused7             : 8; 
	UINT unused8             : 8; 
} MsgTx6012_Safety_MsgType;

typedef struct MsgTx6012_Car_Door_Status_MsgTypeTag
{ /*Car_Door_Status*/
	UINT unused0			    : 8; 
	UINT unused1                : 8;
	UINT unused2                : 8;
	UINT Car_Door_Status_Byte   : 8;
	UINT Car_Door_Status_Byte1  : 8;
	UINT unused3                : 8;
	UINT unused4                : 8;
	UINT unused5                : 8;

} MsgTx6012_Car_Door_Status_MsgType;

typedef struct MsgTx6012_High_Car_Door_Status_MsgTypeTag
{ /*Car_Door_Status*/
	UINT unused0			    : 8; 
	UINT unused1                : 8;

	UINT Car_Door_Status_Byte   : 8;
	UINT Car_Door_Status_Byte1  : 8;
    UINT unused2                : 8;
	UINT unused3                : 8;
	UINT unused4                : 8;
	UINT unused5                : 8;

} MsgTx6012_High_Car_Door_Status_MsgType;


typedef struct MsgTx6012_Out_Temperature_MsgTypeTag
{ /*Out_Temperature*/
	UINT Out_Temperature     : 8;
	UINT unused0			    : 8; 
	UINT unused1             : 8;
	UINT unused2             : 8; 
	UINT unused3             : 8;
	UINT unused4             : 8;
	UINT unused5             : 8;
	UINT unused6             : 8;  
}MsgTx6012_Out_Temperature_MsgType;

typedef struct MsgTx6012_Back_Light_MsgTypeTag
{ /*Backlight*/
	UINT unused0			                : 8; 
	UINT Back_Light                      : 8;
	UINT unused2                         : 8; 
	UINT unused3                         : 8;
	UINT unused4                         : 8;
	UINT unused5                         : 8;
	UINT unused6                         : 8;  
	UINT unused7                         : 8;
}MsgTx6012_Back_Light_MsgType;
typedef struct MsgTx6012_Water_Temperature_MsgTypeTag
{ /*Water_Temperature*/
	UINT Water_Temperature               : 8;
	UINT unused0                         : 8;
	UINT unused1                         : 8;
	UINT unused2                         : 8; 
	UINT unused3                         : 8;
	UINT unused4                         : 8;
	UINT unused5                         : 8;
	UINT unused6                         : 8;  
}MsgTx6012_Water_Temperature_MsgType;
typedef struct MsgTx6012_Tire_Bug_MsgTypeTag
{
	UINT unused0                          : 8;
	UINT unused1                          : 8;
	UINT unused2                          : 8;
	UINT Tire_Bug                         : 1;
	UINT unused3                          : 7;
	UINT unused4                          : 8;
	UINT unused5                          : 8;
	UINT unused6                          : 8;
	UINT unused7                          : 8;

}MsgTx6012_Tire_Bug_MsgType;
typedef struct MsgTx6012_Light_Control_MsgTypeTag
{
	UINT unused0						    : 8;
	UINT unused1						    : 8;
	UINT Cooling_Fluid_Low                : 1;
	UINT unused2						    : 7;
	UINT Light_Control                    : 8;
	UINT unused3                          : 8;
	UINT unused4                          : 8;
	UINT unused5                          : 8;
	UINT unused6                          : 8;

}MsgTx6012_Light_Control_MsgType;
typedef struct MsgTx6012_Bat_Data_MsgTypeTag
{
	UINT Bat_Data1						  : 8;
	UINT Bat_Data2						  : 8;
	UINT Bat_Data3                          : 8;
	UINT Bat_Data4                          : 8;
	UINT Bat_Data5                          : 8;
	UINT Bat_Data6                          : 8;
	UINT Bat_Data7                          : 8;
	UINT Bat_Data8                          : 8;

}MsgTx6012_Bat_Data_MsgType;
typedef struct MsgTx6012_Dir_Control_MsgTypeTag
{
	UINT Data1				     : 8; 
	UINT Data2					 : 8;
	UINT Data3					 : 8;
	UINT Data4	                 : 8;


}MsgTx6012_Dir_Control_MsgType;

typedef struct MsgTx6012_VW_NMC_HAND_PARK_MsgTypeTag
{
	UINT unused0				     : 8; 
	UINT unused1					 : 1;
	UINT VW_NMC_HAND_PARK           : 1;
	UINT unused2					 : 6;
	UINT unused3	                 : 8;
	UINT unused4		             : 8;
	UINT unused5		             : 8;
	UINT unused6		             : 8;
	UINT unused7		             : 8;
	UINT unused8					 : 8;



}MsgTx6012_VW_NMC_HAND_PARK_MsgType;

typedef struct MsgTx6012_Engine_Speed_MsgTypeTag
{
	UINT unused0						    : 8;
	UINT unused1						    : 8;
	UINT unused2						    : 8;
	UINT Engine_Speed_Byte1               : 8;
	UINT Engine_Speed_Byte2               : 4;
	UINT unused3                          : 4;
	UINT unused4                          : 8;
	UINT unused5                          : 8;
	UINT unused6                          : 8;
	UINT unused7                          : 8;

}MsgTx6012_Engine_Speed_MsgType;

typedef struct MsgTx6012_Driver_Mileage_MsgTypeTag
{

	UINT Driver_Mileage_Byte1			    : 8;
	UINT Driver_Mileage_Byte2		        : 8;
	UINT Driver_Mileage_Byte3		        : 4;
	UINT unused0                            : 4;
	UINT unused1                          : 8;
	UINT unused2                          : 8;
	UINT unused4                          : 8;
	UINT unused5                          : 8;
	UINT Car_Out_Temp                     : 8;

}MsgTx6012_Driver_Mileage_MsgType;
typedef struct MsgTx6012_Light_Fault_MsgTypeTag
{

	UINT Light_Fault                   :8;
	UINT unused0                       :8;
	UINT unused1                       :8;
	UINT unused2                       :8;
	UINT unused3                       :8;
	UINT unused4                       :8;
	UINT unused5                       :8; 
	UINT unused6                       :8;

}MsgTx6012_Light_Fault_MsgType;

typedef struct MsgTx6012_Front_Rear_Radar_MsgTypeTag
{
	UINT unused0                        : 8;       
	UINT Front_Rear                     : 8;
	UINT Radar_Data1                    : 8;
	UINT Radar_Data2                    : 8;
	UINT Radar_Data3                    : 8;
	UINT Radar_Data4                    : 8;
	UINT Radar_Color2                   : 4;
	UINT Radar_Color1                   : 4;
	UINT Radar_Color4                   : 4;
	UINT Radar_Color3                   : 4;
}MsgTx6012_Front_Rear_Radar_MsgType;

typedef struct MsgTx6012_Side_Radar_MsgTypeTag
{
	UINT unused0                        : 8;
	UINT Left_Right                     : 8;
	UINT Radar_Data1                    : 8;
	UINT Radar_Data2                    : 8;
	UINT Radar_Data3                    : 8;
	UINT Radar_Data4                    : 8;
	UINT Radar_Color2                   : 4;
	UINT Radar_Color1                   : 4;
	UINT Radar_Color4                   : 4;
	UINT Radar_Color3                   : 4;

}MsgTx6012_Side_Radar_MsgType;


#define EXTERNC		extern "C"

EXTERNC DWORD __stdcall VCI_OpenDevice(DWORD DeviceType,DWORD DeviceInd,DWORD Reserved);
EXTERNC DWORD __stdcall VCI_CloseDevice(DWORD DeviceType,DWORD DeviceInd);
EXTERNC DWORD __stdcall VCI_InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);

EXTERNC DWORD __stdcall VCI_ReadBoardInfo(DWORD DeviceType,DWORD DeviceInd,PVCI_BOARD_INFO pInfo);
EXTERNC DWORD __stdcall VCI_ReadErrInfo(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_ERR_INFO pErrInfo);
EXTERNC DWORD __stdcall VCI_ReadCANStatus(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_STATUS pCANStatus);

EXTERNC DWORD __stdcall VCI_GetReference(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,DWORD RefType,PVOID pData);
EXTERNC DWORD __stdcall VCI_SetReference(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,DWORD RefType,PVOID pData);

EXTERNC ULONG __stdcall VCI_GetReceiveNum(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC DWORD __stdcall VCI_ClearBuffer(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);

EXTERNC DWORD __stdcall VCI_StartCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);
EXTERNC DWORD __stdcall VCI_ResetCAN(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd);

EXTERNC ULONG __stdcall VCI_Transmit(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pSend,ULONG Len);
EXTERNC ULONG __stdcall VCI_Receive(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime=-1);


#endif
