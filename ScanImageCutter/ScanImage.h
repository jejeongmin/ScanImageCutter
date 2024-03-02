#pragma once

class ScanImage
{
private:
	enum CommandType
	{
		CT_DIVIDE_VERTICAL,
		CT_DIVIDE_HORIZONTAL,
		CT_TRIM,
		CT_UNKNOWN,
		MAX_COMMANDTYPE,
	};

public:
			ScanImage();
	virtual ~ScanImage();

	bool	init(const char* sourceDir, const char* targetDir);
	void	close();

	bool	run(const char* command);

private:

	bool	getFilesWithExtension(const std::string& extension);
	CommandType	getCommandType(const std::string& command);

	bool	divideHorizontal();
	bool	divideVertical();
	bool	trim();

	string	getTargetFilePathName(filesystem::path& source, const std::string& addPostfix);
	bool	saveImage(const string& filename, Mat& img, int x, int y, int width, int height);

	filesystem::path	_sourceDir = "",
						_targetDir = "";

	vector<filesystem::path>	_fileList;
};

