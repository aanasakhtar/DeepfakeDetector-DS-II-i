#ifndef HASHER_H
#define HASHER_H

#include <opencv2/opencv.hpp>
#include <string>

class Hasher
{
public:
    static std::string computeDHash(const cv::Mat &image);
    static std::string computePHash(const cv::Mat &image);
};

#endif // HASHER_H
