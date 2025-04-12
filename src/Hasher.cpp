#include "Hasher.h"
#include <bitset>
#include <iostream>

std::string Hasher::computeDHash(const cv::Mat &image)
{
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(9, 8)); // 9x8 for dHash

    std::string hash = "";
    for (int row = 0; row < resized.rows; ++row)
    {
        for (int col = 0; col < resized.cols - 1; ++col)
        {
            hash += (resized.at<uchar>(row, col) > resized.at<uchar>(row, col + 1)) ? '1' : '0';
        }
    }
    return hash;
}

std::string Hasher::computePHash(const cv::Mat &image)
{
    cv::Mat resized, floatImg, dctImg;

    cv::resize(image, resized, cv::Size(32, 32));
    resized.convertTo(floatImg, CV_32F);
    cv::dct(floatImg, dctImg);

    // Use top-left 8x8 block of DCT
    cv::Mat dctBlock = dctImg(cv::Rect(0, 0, 8, 8));
    float avg = static_cast<float>(cv::mean(dctBlock)[0]);

    std::string hash = "";
    for (int i = 0; i < dctBlock.rows; ++i)
    {
        for (int j = 0; j < dctBlock.cols; ++j)
        {
            hash += (dctBlock.at<float>(i, j) > avg) ? '1' : '0';
        }
    }
    return hash;
}
