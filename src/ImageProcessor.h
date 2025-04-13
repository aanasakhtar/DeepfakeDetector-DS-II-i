#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "Hashtable.h"

#include <opencv2/opencv.hpp>

#include <string>

class ImageProcessor {
public:
    std::vector<std::string> fake;
    std::vector<std::string> real;
    

    static cv::Mat loadImage(const std::string& imagePath);
    static cv::Mat convertToGrayscale(const cv::Mat& image);
    void fake_real_distribution(Hashtable ht,int threshold);
};

#endif // IMAGE_PROCESSOR_H
