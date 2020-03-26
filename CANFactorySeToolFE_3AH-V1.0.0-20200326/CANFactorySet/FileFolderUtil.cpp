#include "stdafx.h"
#include "FileFolderUtil.h"

static struct FileFolderUtilInitializer {
	FileFolderUtilInitializer() {
		FileFolderUtil::GetInstance();
	}
} __FileFolderUtilInitializer;

FileFolderUtil::FileFolderUtil()
{
	// ��ȡexe�ļ���ǰ·��
	TCHAR buf[BUFSIZ] = {_T('\0')};
	auto cnt = GetModuleFileName(GetModuleHandle(NULL), buf, BUFSIZ);
	buf[cnt] = _T('\0');

	execute_file_dir_ = buf;

	auto index = execute_file_dir_.ReverseFind(_T('\\'));
	if (index != -1) {
		execute_file_dir_ = execute_file_dir_.Left(index);
	}

	// ����D:\\ReleaseĿ¼����Ȼ����ᱼ����
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
