#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class HashGenerator
{
public:
    // Default constructor
    HashGenerator(int hashSize = 64);

    // Generate a perceptual hash (pHash) from an image
    std::vector<bool> generatePHash(const cv::Mat &image);

    // Convert a hash to string representation for easy storage/comparison
    std::string hashToString(const std::vector<bool> &hash);

    // Convert a string representation back to a hash
    std::vector<bool> stringToHash(const std::string &hashStr);

private:
    int hashSize; // Size of the hash (e.g., 64 bits = 8x8)

    // Helper methods
    cv::Mat preprocessForHash(const cv::Mat &image, int size);
};