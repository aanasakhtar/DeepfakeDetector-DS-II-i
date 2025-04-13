    
    
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "ImageProcessor.h"
#include "Hasher.h"
#include "Hashtable.h"

int main()
{
    // Create hashtable
    Hashtable imagePairsTable;

    // Example paths (you would replace these with actual image paths)
    std::vector<std::pair<std::string, std::string>> imagePairs;

    // In a real application, you would load these pairs from a directory or list
    for (int i = 1; i <= 1000; i++)
    {
        std::string originalPath = "dataset/original/img" + std::to_string(i) + ".jpg";
        std::string suspectPath = "dataset/suspect/img" + std::to_string(i) + ".jpg";
        imagePairs.push_back(std::make_pair(originalPath, suspectPath));
    }

    // Process batch of images and insert into hashtable
    ImageProcessor::processBatch(imagePairs, imagePairsTable);

    // Set threshold for determining real vs fake
    int threshold = 10; // This we can change

    // Create processor that will categorize images
    ImageProcessor processor(imagePairsTable, threshold);

    // Get results
    const auto &realImages = processor.getRealImages();
    const auto &fakeImages = processor.getFakeImages();

    // Print results
    std::cout << "Found " << realImages.size() << " real images" << std::endl;
    std::cout << "Found " << fakeImages.size() << " fake images" << std::endl;

    return 0;
}