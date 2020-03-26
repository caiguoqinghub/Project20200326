#include "StdAfx.h"
#include "ExceptFrameFixer.h"

#define EXCEPT_FRAME_LOG_FILE _T("except_frame_log.txt")

ExceptFrameFixer fixer; //因为CCanSend类内部直接引用的是全局变量，如果把ExceptFrameFixer聚合到类内部，那么捕获的Single_Frame就不全了

ExceptFrameFixer::ExceptFrameFixer(CCANSend *sender, UINT retry_count) 
	: state_(STAT_END_SET), retry_count_(0), max_retry_count_(retry_count),
	 sender_(new CCANSend())
{
	memset(&frame_, 0, sizeof(frame_));
	log_file_.Open(EXCEPT_FRAME_LOG_FILE, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);
}

ExceptFrameFixer::~ExceptFrameFixer(void)
{
	delete sender_;
	log_file_.Close();
}

bool ExceptFrameFixer::Restore()
{
	//if (sender_ != NULL && STATE_END != state_ && ++retry_count_ <= max_retry_count_) {
	if (E_OK == CanDev_opended && E_OK == CanDev_connect && state_ != STAT_END_SET && sender_ != NULL && ++retry_count_ <= max_retry_count_) {
		TRACE(_T("ExceptFrameFixer::Restore\n"));
		sender_->ResetSendBuffer();
		switch(frame_.type) {
		case SINGLE_FRAME:
			sender_->Single_Frame(frame_.id, frame_.sid, frame_.did, frame_.did_cmd, frame_.data, frame_.data_len);
			break;
		case MULTI_FRAME:
			sender_->Multi_Frame(frame_.id, frame_.sid, frame_.did, frame_.did_cmd, frame_.data, frame_.data_len);
			break;
		case MULTI_FRAME1:
	//		sender_->Multi_Frame1(frame_.id, frame_.sid, frame_.did, frame_.did_cmd, frame_.data, frame_.data_len);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}