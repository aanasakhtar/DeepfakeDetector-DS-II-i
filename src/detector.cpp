#include "detector.hpp"
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

Detector::Detector(int hashSize, int hammingThreshold)
    : hashSize(hashSize), hammingThreshold(hammingThreshold)
{

    // Initialize components
    hashGenerator = std::make_shared<HashGenerator>(hashSize);
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
    // Load and validate image
    cv::Mat image = imageProcessor->loadImage(filepath);
    if (!imageProcessor->isValidImage(image))
    {
        return false;
    }

    // Process image for better hash generation
    cv::Mat processed = imageProcessor->preprocess(image);

    // Generate hash
    std::vector<bool> hash = hashGenerator->generatePHash(processed);

    // Create ID if not provided
    std::string id = imageId.empty() ? generateImageId() : imageId;

    // Add to database
    return hashDatabase->addHash(id, hash, filepath, true);
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

    // Validate image
    if (!imageProcessor->isValidImage(image))
    {
        return details;
    }

    // Process image
    cv::Mat processed = imageProcessor->preprocess(image);

    // Generate hash
    std::vector<bool> hash = hashGenerator->generatePHash(processed);

    // Check for exact match
    if (hashDatabase->containsHash(hash))
    {
        details.result = DetectionResult::EXACT_DUPLICATE;
    }
    else
    {
        // Look for similar hashes
        std::vector<ImageRecord> similarImages = hashDatabase->findSimilarHashes(hash);

        if (!similarImages.empty())
        {
            details.result = DetectionResult::SIMILAR;
            details.matches = similarImages;

            // Find best match (closest hash)
            int minDistance = hammingThreshold + 1;
            for (const auto &match : similarImages)
            {
                std::vector<bool> matchHash = hashGenerator->stringToHash(match.hashString);
                int distance = HammingDistance::calculate(hash, matchHash);

                if (distance < minDistance)
                {
                    minDistance = distance;
                    details.bestMatchId = match.imageId;
                }
            }

            details.bestMatchDistance = minDistance;
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
    // Generate a unique ID based on timestamp and random number
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    // Add some randomness
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9999);
    int random = dis(gen);

    // Format ID
    std::stringstream ss;
    ss << "img_" << millis << "_" << std::setw(4) << std::setfill('0') << random;
    return ss.str();
}