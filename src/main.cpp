#include <iostream>
#include "ImageProcessor.h"
#include "Hasher.h"
#include "HashComparator.h"

int main()
{
    std::string originalPath = "original.jpg"; // We will change these
    std::string suspectPath = "suspect.jpg";

    cv::Mat original = ImageProcessor::convertToGrayscale(ImageProcessor::loadImage(originalPath));
    cv::Mat suspect = ImageProcessor::convertToGrayscale(ImageProcessor::loadImage(suspectPath));

    std::string hash1 = Hasher::computeDHash(original);
    std::string hash2 = Hasher::computeDHash(suspect);

    int distance = HashComparator::hammingDistance(hash1, hash2);

    std::cout << "Hamming Distance: " << distance << std::endl;
    if (HashComparator::isDeepfake(distance, 10))
    {
        std::cout << "Warning: Possible Deepfake Detected!" << std::endl;
    }
    else
    {
        std::cout << "Image Appears Legitimate." << std::endl;
    }

    return 0;
}
