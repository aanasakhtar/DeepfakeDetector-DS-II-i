#pragma once
#include "hamming.hpp"
#include "hash_generator.hpp"
#include "hash_database.hpp"
#include "image_processor.hpp"
#include <memory>
#include <string>

enum class DetectionResult
{
    ORIGINAL,        // No matches found, likely an original image
    EXACT_DUPLICATE, // Exact hash match found
    SIMILAR,         // Similar hash found (within threshold)
    INVALID_IMAGE    // Image couldn't be processed
};

struct DetectionDetails
{
    DetectionResult result;
    std::vector<ImageRecord> matches;
    int bestMatchDistance = -1;
    std::string bestMatchId = "";
};

class Detector
{
public:
    Detector(int hashSize = 64, int hammingThreshold = 5);

    // Initialize detector with database file
    bool initialize(const std::string &databaseFile = "");

    // Add an original image to the database
    bool addOriginalImage(const std::string &filepath, const std::string &imageId = "");

    // Check if an image is copied/fake
    DetectionDetails checkImage(const std::string &filepath);
    DetectionDetails checkImage(const cv::Mat &image, const std::string &imageId = "");

    // Save the current database state
    bool saveDatabase(const std::string &filepath);

    // Get the number of images in the database
    size_t getDatabaseSize() const;

private:
    std::shared_ptr<HashGenerator> hashGenerator;
    std::shared_ptr<HashDatabase> hashDatabase;
    std::shared_ptr<ImageProcessor> imageProcessor;

    int hashSize;
    int hammingThreshold;

    // Generate a unique ID if none provided
    std::string generateImageId() const;
};