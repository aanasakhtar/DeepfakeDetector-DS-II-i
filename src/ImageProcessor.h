#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "Hashtable.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <utility>

class ImageProcessor
{
public:
    ImageProcessor(Hashtable &ht, int threshold);

    // Getters for the real and fake vectors
    const std::vector<std::pair<std::string, std::string>> &getRealImages() const;
    const std::vector<std::pair<std::string, std::string>> &getFakeImages() const;

    // Image loading and processing operations
    static cv::Mat loadImage(const std::string &imagePath);
    static cv::Mat convertToGrayscale(const cv::Mat &image);

    // Process a batch of image pairs and insert into hashtable
    static void processBatch(const std::vector<std::pair<std::string, std::string>> &imagePairs, Hashtable &ht);

private:
    std::vector<std::pair<std::string, std::string>> fakeImages;
    std::vector<std::pair<std::string, std::string>> realImages;

    // Private function to split hashtable entries into real and fake vectors
    void fakeRealDistribution(Hashtable &ht, int threshold);
};

#endif // IMAGE_PROCESSOR_H