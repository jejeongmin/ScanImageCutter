// ScanImageCutter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "ScanImage.h"

int main(int argc, char* argv[])
{
    const int validArgNum = 4;
    if (argc != validArgNum)
    {
        cout << "invalid argument number, argument num is " << validArgNum << endl;
        cout << "ex) [sourceDir] [targetDir] [divV|divH|trim]" << endl;
        return -1;
    }

    ScanImage   image;

    if (false == image.init(argv[1], argv[2]))
        return -1;

    if (false == image.run(argv[3]))
        return -1;

    image.close();
}