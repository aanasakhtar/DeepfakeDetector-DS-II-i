#include "detector.hpp"
#include <cmath>
#include <random>

Detector::Detector(int hashSize, int hammingThreshold)
    : hashSize(hashSize), hammingThreshold(hammingThreshold)
{
    hashGenerator = std::make_shared<HashGenerator>(16);
    imageProcessor = std::make_shared<ImageProcessor>();
    hashDatabase = std::make_shared<HashDatabase>(hashGenerator, hammingThreshold);
}

bool Detector::initialize(const std::string &databaseFile)
{
    if (!databaseFile.empty())
    {
        return hashDatabase->loadFromFile(databaseFile);
    }
    return true;
}

bool Detector::addOriginalImage(const std::string &filepath, const std::string &imageId)
{
    cv::Mat image = imageProcessor->loadImage(filepath);
    if (!imageProcessor->isValidImage(image)) return false;

    cv::Mat processed = imageProcessor->preprocess(image);
    std::vector<float> features = hashGenerator->generateDCTFeatures(processed);

    std::string id = imageId.empty() ? generateImageId() : imageId;
    return hashDatabase->addHash(id, features, filepath, true);
}

DetectionDetails Detector::checkImage(const std::string &filepath)
{
    cv::Mat image = imageProcessor->loadImage(filepath);
    return checkImage(image, "");
}

DetectionDetails Detector::checkImage(const cv::Mat &image, const std::string &imageId)
{
    DetectionDetails details;
    details.result = DetectionResult::INVALID_IMAGE;

    if (!imageProcessor->isValidImage(image)) return details;

    cv::Mat processed = imageProcessor->preprocess(image);
    std::vector<float> features = hashGenerator->generateDCTFeatures(processed);

    if (hashDatabase->containsHash(features))
    {
        details.result = DetectionResult::EXACT_DUPLICATE;
    }
    else
    {
        std::vector<ImageRecord> similarImages = hashDatabase->findSimilarHashes(features);

        if (!similarImages.empty())
        {
            details.result = DetectionResult::SIMILAR;
            details.matches = similarImages;

            double minDistance = 2.0; // Max cosine distance
            for (const auto &match : similarImages)
            {
                double similarity = SimilarityMetrics::calculateCosine(features, match.features);
                double distance = 1.0 - similarity;

                if (distance < minDistance)
                {
                    minDistance = distance;
                    details.bestMatchId = match.imageId;
                    details.bestMatchDistance = minDistance;
                }
            }
            // Adjust threshold for cosine distance
            double dynamicThreshold = hammingThreshold / 100.0;
            if (image.size().area() > 50000) dynamicThreshold *= 1.2; // Slightly relax for larger images
            if (minDistance > dynamicThreshold) details.result = DetectionResult::ORIGINAL;
        }
        else
        {
            details.result = DetectionResult::ORIGINAL;
        }
    }

    return details;
}

bool Detector::saveDatabase(const std::string &filepath)
{
    return hashDatabase->saveToFile(filepath);
}

size_t Detector::getDatabaseSize() const
{
    return hashDatabase->size();
}

std::string Detector::generateImageId() const
{
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9999);
    std::stringstream ss;
    ss << "img_" << millis << "_" << std::setw(4) << std::setfill('0') << dis(gen);
    return ss.str();
}