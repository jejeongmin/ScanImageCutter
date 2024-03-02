// ScanImageCutter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "ScanImage.h"

int main(int argc, char* argv[])
{
    const int validArgNum = 3;
    if (argc != validArgNum)
    {
        cerr << "invalid argument number, argument num is " << validArgNum << endl;
        cerr << "ex) [sourceDir] [targetDir] [divV|divH|trim]" << endl;
        return -1;
    }

    ScanImage   image;

    if (false == image.init(argv[0], argv[1]))
        return -1;

    if (false == image.run(argv[2]))
        return -1;

    image.close();
}