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

	bool	divideHorizontal();
	bool	divideVertical();
	bool	trim();

	bool	getFilesWithExtension(const std::string& extension);
	CommandType	getCommandType(const std::string& command);
	std::pair<int, int>	getScanRangeOffset(int range);
	bool	getTrimOffset(const Mat& img, Rect rect, Point& offset, int depth = 2);
	bool	getTrimRect(const Mat& img, Rect& rect);
	
	string	getTargetFilePathName(filesystem::path& source, const std::string& addPostfix);
	bool	saveImage(const string& filename, const Mat& img, int x, int y, int width, int height);

	filesystem::path	_sourceDir = "",
						_targetDir = "";

	vector<filesystem::path>	_fileList;

	const int _whiteBGRSum = 254 + 254 + 254;
};

