#pragma once

#include <set>
#include "CANSend.h"

/*!
*	\brief MAX_FRAME_DATA_SIZE 一个帧的最大长度（这儿主要指多帧）
*/
#define MAX_FRAME_DATA_SIZE (2048)

/*!
*	\brief DEFAULT_RETRY_COUNT 默认失败的时候，重新发送的次数
*/
#define DEFAULT_RETRY_COUNT (8)

/*!
*	\brief SINGLE_FRAME 表示单帧
*/
#define SINGLE_FRAME (0)

/*!
*	\brief MULTI_FRAME 表示多帧， 对应CCanSend::Multi_Frame
*/
#define MULTI_FRAME (1)

/*!
*	\brief MULTI_FRAME1 表示多帧1，对应CCanSend::Multi_Frame1
*/
#define MULTI_FRAME1 (2)

#define INVALID_FRAME (-1) //!< 表示无效帧

class ExceptFrameFixer
{
public:
	enum State {
		STAT_START_SET = 0,
		STAT_END_SET
	};

	struct Frame
	{
		UINT type;	//!< 标志帧类型
		UINT id;	//!< 帧id	
		UINT sid;	//!< 表示帧sid
		UINT did;   //!< 表示帧did
		UINT did_cmd; //!<
		BYTE data[MAX_FRAME_DATA_SIZE]; //! 数据内容
		UINT data_len;	//数据长度
	};

	ExceptFrameFixer(CCANSend *sender_ = NULL, UINT retry_count = DEFAULT_RETRY_COUNT);
	~ExceptFrameFixer(void);

	inline void StartSet()
	{
		state_ = STAT_START_SET;
	}

	inline void EndSet()
	{
		state_ = STAT_END_SET;
	}

	inline void Reset()
	{
		state_ = STAT_END_SET;
		retry_count_ = 0;
		already_set_set_.clear();
		already_read_set_.clear();
	}

	// 下面的三个函数指示捕获设置模式下发送的数据
	// 对于其他的模式不进行捕获和重发
	inline void SetSingleFrameRestoreData(UINT id, UINT sid, UINT did, UINT did_cmd, BYTE *data, UINT data_len)
	{
		ASSERT(data_len <= MAX_FRAME_DATA_SIZE && "MAX_FRAME_DATA_SIZE is too small");
		if (STAT_END_SET == state_) return;
		SetCommonRestoreData(id, sid, did, did_cmd, data, data_len);
		frame_.type = SINGLE_FRAME;
	}

	inline void SetMultiFrameRestoreData(UINT id, UINT sid, UINT did, UINT did_cmd, BYTE *data, UINT data_len)
	{
		ASSERT(data_len <= MAX_FRAME_DATA_SIZE && "MAX_FRAME_DATA_SIZE is too small");
		if (STAT_END_SET == state_) return;
		SetCommonRestoreData(id, sid, did, did_cmd, data, data_len);
		frame_.type = MULTI_FRAME;
	}

	inline void SetMultiFrame1RestoreData(UINT id, UINT sid, UINT did, UINT did_cmd, BYTE *data, UINT data_len)
	{
		ASSERT(data_len <= MAX_FRAME_DATA_SIZE && "MAX_FRAME_DATA_SIZE is too small");
		if (STAT_END_SET == state_) return;
		SetCommonRestoreData(id, sid, did, did_cmd, data, data_len);
		frame_.type = MULTI_FRAME1;
	}

	/*!
	*	\brief 恢复之前漏发的帧
	*/
	bool Restore();

	inline void Log()
	{
		CString log_str;
		log_str.Format(_T("state: %d, id: %03X, sid: %03X, did: %03X, did_cmd: %d, data_len: %d, retry_count: %d, max_retry_count: %d\n"), state_, frame_.id, frame_.sid, frame_.did,
			frame_.did_cmd, frame_.data_len, retry_count_, max_retry_count_);
		log_file_.Write(log_str.GetBuffer(), log_str.GetLength() * sizeof(TCHAR));
		log_file_.Flush();
	}

	void AlreadySet(UINT set_did)
	{
		already_set_set_.insert(set_did);
	}

	bool IsAlreadSet(UINT set_did)
	{
		return already_set_set_.end() != already_set_set_.find(set_did);
	}

	void AlreadyRead(UINT read_did)
	{
		already_read_set_.insert(read_did);
	}

	bool IsAlreadyRead(UINT read_did)
	{
		return already_read_set_.end() != already_read_set_.find(read_did);
	}

private:

	inline void SetCommonRestoreData(UINT id, UINT sid, UINT did, UINT did_cmd, BYTE *data, UINT data_len)
	{
		frame_.id = id;
		frame_.sid = sid;
		frame_.did = did;
		frame_.did_cmd = did_cmd;
		memcpy(frame_.data, data, min(data_len, MAX_FRAME_DATA_SIZE));
		frame_.data_len = data_len;
	}

private:
	Frame frame_;
	State state_;
	UINT retry_count_;
	UINT max_retry_count_;
	CCANSend *sender_;

	CFile log_file_;

	std::set<UINT> already_set_set_;
	std::set<UINT> already_read_set_;
};

extern ExceptFrameFixer fixer;