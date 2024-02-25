// ScanImageCutter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include <filesystem>

using namespace cv;
using namespace std;

int main()
{
    auto path = std::filesystem::current_path() / "../../image/0023.jpeg";
    Mat img = imread(path.string(), IMREAD_COLOR);
    if (img.empty())
    {
        cerr << "cannot read image " << std::filesystem::current_path() << endl;
        return -1;
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
            return -1;
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
            return -1;
        }
    }    

    return 0;
}