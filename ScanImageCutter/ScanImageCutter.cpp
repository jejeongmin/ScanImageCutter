// ScanImageCutter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include <filesystem>

using namespace cv;

int main()
{
    auto path = std::filesystem::current_path() / "../../image/0023.jpeg";
    Mat img = imread(path.string(), IMREAD_COLOR);

    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window

    return 0;
}