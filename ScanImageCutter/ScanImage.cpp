#include "stdafx.h"
#include "ScanImage.h"

ScanImage::ScanImage()
{
	_fileList.clear();
}

ScanImage::~ScanImage()
{
	close();
}

bool ScanImage::init(const char* sourceDir, const char* targetDir)
{
	if (nullptr == sourceDir)
	{
		cerr << "sourceDir is nullptr" << endl;
		return false;
	}

	if (nullptr == targetDir)
	{
		cerr << "targetDir is nullptr" << endl;
		return false;
	}

	_sourceDir = sourceDir;
	_targetDir = targetDir;

	try
	{
		if (false == filesystem::is_directory(_sourceDir))
		{
			cerr << "sourceDir(" << _sourceDir << ") is not directory" << endl;
			return false;
		}

		if (false == filesystem::is_directory(_targetDir))
		{
			cerr << "targetDir(" << _targetDir << ") is not directory" << endl;
			return false;
		}
	}
	catch (const filesystem::filesystem_error& ex)
	{
		cerr << "filesystem exception : " << ex.what() << endl;
		return false;
	}

	if (false == getFilesWithExtension("jpg"))
	{
		cerr << "there are no files in sourcedir" << endl;
		return false;
	}

	return true;
}

void ScanImage::close()
{
	_sourceDir = "";
	_targetDir = "";

	_fileList.clear();
}

bool ScanImage::run(const char* command)
{
	switch (getCommandType(command))
	{
	case CT_DIVIDE_VERTICAL:
		divideVertical();
		break;
	case CT_DIVIDE_HORIZONTAL:
		divideHorizontal();
		break;
	case CT_TRIM:
		trim();
		break;
	default:
		cerr << "unknown command" << endl;
		break;
	}

	return true;
}

bool ScanImage::getFilesWithExtension(const std::string& extension)
{
	for (const auto& entry : filesystem::directory_iterator(_sourceDir))
	{
		if (entry.is_regular_file() && entry.path().extension() == extension)
		{
			_fileList.push_back(entry.path().string());
		}
	}

	return (false == _fileList.empty());
}

ScanImage::CommandType ScanImage::getCommandType(const std::string& command)
{
	std::map<std::string, CommandType> commands = {
		{"divV", CT_DIVIDE_VERTICAL},
		{"divH", CT_DIVIDE_HORIZONTAL},
		{"trim", CT_TRIM}
	};

	auto itor = commands.find(command);
	if (itor != commands.end())
		return itor->second;

	return CT_UNKNOWN;
}

bool ScanImage::divideVertical()
{
	auto path = std::filesystem::current_path() / "../../image/0023.jpeg";
	Mat img = imread(path.string(), IMREAD_COLOR);
	if (img.empty())
	{
		cerr << "cannot read image " << std::filesystem::current_path() << endl;
		return false;
	}

	int width = img.cols;
	int height = img.rows;

	cout << "width : " << width << endl;
	cout << "height : " << height << endl;

	int curXPos = 0;
	int maxBGRSum = 0;
	for (int x = 0; x < width; ++x)
	{
		int columnBGRSum = 0;

		for (int y = 0; y < height; ++y)
		{
			cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);

			columnBGRSum += static_cast<int>(pixel[0] + pixel[1] + pixel[2]);
		}

		if (maxBGRSum < columnBGRSum)
		{
			maxBGRSum = columnBGRSum;
			curXPos = x;
		}
	}

	cout << " minBGRSum  is " << maxBGRSum << endl;
	cout << " cur x posision is " << curXPos << endl;

	// 좌측 이미지 저장
	{
		cv::Rect regionOfInterest(0, 0, curXPos, height); // (x, y, width, height)

		cv::Mat roi = img(regionOfInterest);

		auto savePath = std::filesystem::current_path() / "../../image/0023_left.jpeg";

		if (!cv::imwrite(savePath.string(), roi))
		{
			cerr << "can not save img " << savePath.string() << std::endl;
			return false;
		}
	}

	// 우측 이미지 저장
	{
		cv::Rect regionOfInterest(curXPos, 0, width - curXPos, height); // (x, y, width, height)

		cv::Mat roi = img(regionOfInterest);

		auto savePath = std::filesystem::current_path() / "../../image/0023_right.jpeg";

		if (!cv::imwrite(savePath.string(), roi))
		{
			cerr << "can not save img " << savePath.string() << std::endl;
			return false;
		}
	}

	return true;
}

bool ScanImage::divideHorizontal()
{
	return true;
}

bool ScanImage::trim()
{
	return true;
}