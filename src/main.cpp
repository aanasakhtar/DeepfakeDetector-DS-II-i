#include <iostream>
#include "ImageProcessor.h"
#include "Hasher.h"



int hammingDistance(const std::string &hash1, const std::string &hash2)
{
    int count = 0;
    int len = std::min(hash1.length(), hash2.length());
    for (int i = 0; i < len; ++i)
    {
        if (hash1[i] != hash2[i])
        {
            count++;
        }
    }
    return count;
}
int main()
{
    // hashtable intialization
    std::string originalPath = "original.jpg"; // We will change these
    std::string suspectPath = "suspect.jpg";

    ImageProcessor IM(originPath,suspectPath);
    
    for(int i = 0; i < 1000; i++) {
        // loading our inputs 
        cv::Mat suspect = ImageProcessor::convertToGrayscale(ImageProcessor::loadImage(suspectPath));
        cv::Mat original = ImageProcessor::convertToGrayscale(ImageProcessor::loadImage(originalPath));

        std::string hash1 = Hasher::computeDHash(original);
        std::string hash2 = Hasher::computeDHash(suspect);

        int distance = hammingDistance(hash1, hash2);
        
        // put hamming value and images pair in hashtable 
    }



    
   

    return 0;
}