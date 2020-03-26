#pragma once

class FileFolderUtil
{
public:
	FileFolderUtil();

	const CString& GetExecuteFileDirecotroy() const;
	void CreateFolderIfNotExist(const CString &dir);

	static FileFolderUtil& GetInstance()
	{
		static FileFolderUtil execute_file_dir;
		return execute_file_dir;
	}

protected:
	CString execute_file_dir_;
};