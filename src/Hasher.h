#ifndef HASHER_H
#define HASHER_H

#include <opencv2/opencv.hpp>
#include <string>

class Hasher
{
public:
    static std::string computeDHash(const cv::Mat &image);

    // Compute hamming distance between two hash strings
    static int hammingDistance(const std::string &hash1, const std::string &hash2);
};

#endif // HASHER_H