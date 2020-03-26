#include "stdafx.h"
#include "FileFolderUtil.h"

static struct FileFolderUtilInitializer {
	FileFolderUtilInitializer() {
		FileFolderUtil::GetInstance();
	}
} __FileFolderUtilInitializer;

FileFolderUtil::FileFolderUtil()
{
	// 获取exe文件当前路径
	TCHAR buf[BUFSIZ] = {_T('\0')};
	auto cnt = GetModuleFileName(GetModuleHandle(NULL), buf, BUFSIZ);
	buf[cnt] = _T('\0');

	execute_file_dir_ = buf;

	auto index = execute_file_dir_.ReverseFind(_T('\\'));
	if (index != -1) {
		execute_file_dir_ = execute_file_dir_.Left(index);
	}

	// 创建D:\\Release目录，不然程序会奔溃的
	CreateFolderIfNotExist(_T("D:\\Release"));
}

const CString& FileFolderUtil::GetExecuteFileDirecotroy() const
{
	return execute_file_dir_;
}

void FileFolderUtil::CreateFolderIfNotExist(const CString &dir)
{
	if (!::PathFileExists(dir)) {
		::CreateDirectory(dir, NULL);
	}
}
