#include "ImageProcessor.h"
#include "Hasher.h"
#include <iostream>

ImageProcessor::ImageProcessor(Hashtable &ht, int threshold)
{
    fakeRealDistribution(ht, threshold);
}

const std::vector<std::pair<std::string, std::string>> &ImageProcessor::getRealImages() const
{
    return realImages;
}

const std::vector<std::pair<std::string, std::string>> &ImageProcessor::getFakeImages() const
{
    return fakeImages;
}

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

void ImageProcessor::fakeRealDistribution(Hashtable &ht, int threshold)
{
    // Implementation of separating real and fake images based on threshold
    fakeImages = ht.getPairsBelowThreshold(threshold);
    realImages = ht.getPairsAboveThreshold(threshold);
}

void ImageProcessor::processBatch(const std::vector<std::pair<std::string, std::string>> &imagePairs, Hashtable &ht)
{
    for (const auto &pair : imagePairs)
    {
        // Load and process images
        cv::Mat originalImg = convertToGrayscale(loadImage(pair.first));
        cv::Mat suspectImg = convertToGrayscale(loadImage(pair.second));

        // Compute hashes
        std::string originalHash = Hasher::computeDHash(originalImg);
        std::string suspectHash = Hasher::computeDHash(suspectImg);

        // Calculate hamming distance
        int distance = Hasher::hammingDistance(originalHash, suspectHash);

        // Insert into hashtable
        ht.insert(distance, pair.first, pair.second);
    }
}
