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

	auto path = std::filesystem::current_path();

	_sourceDir = (path / sourceDir);
	_targetDir = (path / targetDir);

	try
	{
		if (false == filesystem::is_directory(_sourceDir.string()))
		{
			cerr << "sourceDir(" << _sourceDir << ") is not directory" << endl;
			return false;
		}

		if (false == filesystem::is_directory(_targetDir.string()))
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

	getFilesWithExtension(".jpeg");
	getFilesWithExtension(".jpg");
	
	if (_fileList.empty())
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
		if (entry.is_regular_file() && entry.path().extension().string() == extension)
		{
			_fileList.push_back(entry.path());
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
	for (auto filename : _fileList)
	{
		Mat img = imread(filename.string(), IMREAD_COLOR);
		if (img.empty())
		{
			cerr << "cannot read image : " << filename << endl;
			continue;
		}

		cout << filename << " read successfully. width : " << img.cols << ", height : " << img.rows << endl;

		int curXPos = 0;
		int maxBGRSum = 0;
		for (int x = 0; x < img.cols; ++x)
		{
			int columnBGRSum = 0;

			for (int y = 0; y < img.rows; ++y)
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

		if (curXPos <= 0)
		{
			cerr << "cannot cut image : " << filename << " curX : " << curXPos << ", minBGRSum : " << maxBGRSum << endl;
			continue;
		}

		std::string filenameLeftCut = getTargetFilePathName(filename, "_left");
		std::string filenameRightCut = getTargetFilePathName(filename, "_right");
		
		saveImage(filenameLeftCut, img, 0, 0, curXPos, img.rows);
		saveImage(filenameRightCut, img, curXPos, 0, img.cols - curXPos, img.rows);
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

string ScanImage::getTargetFilePathName(filesystem::path& source, const std::string& addPostfix)
{
	string targetFilePathName = source.string().substr(0, source.string().find_last_of('.')) + addPostfix + source.extension().string();

	size_t startPos = targetFilePathName.find(_sourceDir.string());
	if (startPos != std::string::npos) {
		targetFilePathName.replace(startPos, _sourceDir.string().length(), _targetDir.string());
	}

	return targetFilePathName;
}

bool ScanImage::saveImage(const string& filename, Mat& img, int x, int y, int width, int height)
{
	cv::Rect	regionOfInterest(x, y, width, height);
	cv::Mat		roi = img(regionOfInterest);

	if (!cv::imwrite(filename, roi))
	{
		cerr << "can not save img " << filename << std::endl;
		return false;
	}

	return true;
}