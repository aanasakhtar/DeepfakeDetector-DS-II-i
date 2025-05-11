#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class HashGenerator
{
public:
    HashGenerator(int featureSize = 16);
    std::vector<float> generateDCTFeatures(const cv::Mat &image); // Correct method
    std::string hashToString(const std::vector<float> &features);
    std::vector<float> stringToHash(const std::string &hashStr);
private:
    int featureSize;
    cv::Mat preprocessForHash(const cv::Mat &image, int size);
};