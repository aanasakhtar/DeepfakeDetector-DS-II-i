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

int Hasher::hammingDistance(const std::string &hash1, const std::string &hash2)
{
    int count = 0;
    int len = std::min(hash1.length(), hash2.length());
    for (int i = 0; i < len; ++i)
    {
        if (hash1[i] != hash2[i])
        {
            count++;
        }
    }
    return count;
}