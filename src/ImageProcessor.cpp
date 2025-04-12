#include "ImageProcessor.h"
#include <iostream>

cv::Mat ImageProcessor::loadImage(const std::string &imagePath)
{
    cv::Mat img = cv::imread(imagePath);
    if (img.empty())
    {
        std::cerr << "Error: Unable to load image: " << imagePath << std::endl;
    }
    return img;
}

cv::Mat ImageProcessor::convertToGrayscale(const cv::Mat &image)
{
    cv::Mat gray;
    if (!image.empty())
    {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }
    return gray;
}
