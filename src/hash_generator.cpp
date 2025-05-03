#include "hash_generator.hpp"
#include <bitset>

HashGenerator::HashGenerator(int hashSize) : hashSize(hashSize) {}

cv::Mat HashGenerator::preprocessForHash(const cv::Mat &image, int size)
{
    cv::Mat resized, grayscale;

    // Convert to grayscale if needed
    if (image.channels() == 3 || image.channels() == 4)
    {
        cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
    }
    else
    {
        grayscale = image.clone();
    }

    // Resize to the required dimensions
    cv::resize(grayscale, resized, cv::Size(size, size), 0, 0, cv::INTER_AREA);

    return resized;
}

std::vector<bool> HashGenerator::generatePHash(const cv::Mat &image)
{
    // Step 1: Preprocess - resize and convert to grayscale
    int size = 32; // For DCT, we need a larger initial size
    cv::Mat preprocessed = preprocessForHash(image, size);

    // Step 2: Apply DCT (Discrete Cosine Transform)
    cv::Mat floatImg;
    preprocessed.convertTo(floatImg, CV_32F);
    cv::Mat dct;
    cv::dct(floatImg, dct);

    // Step 3: Extract low frequency components (top-left 8x8 corner)
    cv::Mat dctSubset = dct(cv::Rect(0, 0, 8, 8));

    // Step 4: Compute average DCT value (excluding the DC component)
    double sum = 0.0;
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (i != 0 || j != 0)
            { // Skip the DC component (0,0)
                sum += dctSubset.at<float>(i, j);
                count++;
            }
        }
    }
    double avg = sum / count;

    // Step 5: Generate hash based on whether DCT value > average
    std::vector<bool> hash;
    hash.reserve(64);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            hash.push_back(dctSubset.at<float>(i, j) > avg);
        }
    }

    return hash;
}

std::string HashGenerator::hashToString(const std::vector<bool> &hash)
{
    std::string result;
    result.reserve((hash.size() + 7) / 8); // Ceiling division by 8

    for (size_t i = 0; i < hash.size(); i += 8)
    {
        std::bitset<8> byte;
        for (size_t j = 0; j < 8 && (i + j) < hash.size(); j++)
        {
            byte[j] = hash[i + j];
        }
        result.push_back(static_cast<char>(byte.to_ulong()));
    }

    return result;
}

std::vector<bool> HashGenerator::stringToHash(const std::string &hashStr)
{
    std::vector<bool> hash;
    hash.reserve(hashStr.length() * 8);

    for (char c : hashStr)
    {
        std::bitset<8> byte(c);
        for (int i = 0; i < 8; i++)
        {
            hash.push_back(byte[i]);
        }
    }

    return hash;
}