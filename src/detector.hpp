#pragma once
#include "hamming.hpp"
#include "hash_generator.hpp"
#include "hash_database.hpp"
#include "image_processor.hpp"
#include <memory>
#include <string>

enum class DetectionResult
{
    ORIGINAL,
    EXACT_DUPLICATE,
    SIMILAR,
    INVALID_IMAGE
};

struct DetectionDetails
{
    DetectionResult result;
    std::vector<ImageRecord> matches;
    double bestMatchDistance = -1;
    std::string bestMatchId = "";
};

class Detector
{
public:
    Detector(int hashSize = 16, int hammingThreshold = 5);

    bool initialize(const std::string &databaseFile = "");
    bool addOriginalImage(const std::string &filepath, const std::string &imageId = "");
    DetectionDetails checkImage(const std::string &filepath);
    DetectionDetails checkImage(const cv::Mat &image, const std::string &imageId = "");
    bool saveDatabase(const std::string &filepath);
    size_t getDatabaseSize() const;

private:
    std::shared_ptr<HashGenerator> hashGenerator;
    std::shared_ptr<HashDatabase> hashDatabase;
    std::shared_ptr<ImageProcessor> imageProcessor;
    int hashSize;
    int hammingThreshold;

    std::string generateImageId() const;
};