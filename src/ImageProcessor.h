#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <string>

class ImageProcessor {
public:
    static cv::Mat loadImage(const std::string& imagePath);
    static cv::Mat convertToGrayscale(const cv::Mat& image);
};

#endif // IMAGE_PROCESSOR_H
