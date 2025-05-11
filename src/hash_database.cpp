#include "hash_database.hpp"
#include "hash_generator.hpp"
#include "hamming.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

HashDatabase::HashDatabase(std::shared_ptr<HashGenerator> hashGen, int hammingThreshold)
    : hashGenerator(hashGen), hammingThreshold(hammingThreshold) {}

bool HashDatabase::addHash(const std::string &imageId, const std::vector<float> &features,
                           const std::string &sourcePath, bool isOriginal)
{
    std::string featureStr = hashGenerator->hashToString(features);

    if (hashMap.find(featureStr) != hashMap.end())
    {
        std::cout << "Features already exist for imageId=" << imageId << std::endl;
        return false;
    }

    ImageRecord record;
    record.imageId = imageId;
    record.features = features;
    record.sourcePath = sourcePath;
    record.isOriginal = isOriginal;

    hashMap[featureStr] = record;
    std::cout << "Added features for imageId=" << imageId << ", featureStr length=" << featureStr.length() << std::endl;
    return true;
}

bool HashDatabase::containsHash(const std::vector<float> &features) const
{
    std::string featureStr = hashGenerator->hashToString(features);
    return hashMap.find(featureStr) != hashMap.end();
}

std::vector<ImageRecord> HashDatabase::findSimilarHashes(const std::vector<float> &features) const
{
    std::vector<ImageRecord> results;
    std::string queryFeatureStr = hashGenerator->hashToString(features);

    for (const auto &entry : hashMap)
    {
        const ImageRecord &record = entry.second;
        double similarity = SimilarityMetrics::calculateCosine(features, record.features);
        double distance = 1.0 - similarity; // Cosine distance (0 = identical, 2 = opposite)

        std::cout << "Comparing with " << record.imageId << ": similarity=" << similarity
                  << ", distance=" << distance << " (threshold=" << hammingThreshold / 100.0 << ")" << std::endl;

        if (distance <= hammingThreshold / 100.0) // Scale threshold for cosine distance
        {
            results.push_back(record);
        }
    }

    if (results.empty())
    {
        std::cout << "No similar matches found within threshold " << hammingThreshold / 100.0 << std::endl;
    }
    return results;
}

bool HashDatabase::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to open database file: " << filename << std::endl;
        return false;
    }

    hashMap.clear();
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line))
    {
        lineNumber++;
        std::istringstream iss(line);
        ImageRecord record;
        std::string isOriginalStr, featureStr;

        if (!(std::getline(iss, record.imageId, '\t') &&
              std::getline(iss, record.sourcePath, '\t') &&
              std::getline(iss, isOriginalStr, '\t') &&
              std::getline(iss, featureStr)))
        {
            std::cout << "Malformed line " << lineNumber << ": " << line << std::endl;
            continue;
        }

        if (record.imageId.empty() || record.sourcePath.empty())
        {
            std::cout << "Empty imageId or sourcePath at line " << lineNumber << ": " << line << std::endl;
            continue;
        }

        record.isOriginal = (isOriginalStr == "1" || isOriginalStr == "true");

        try
        {
            record.features = hashGenerator->stringToHash(featureStr);
            if (record.features.empty())
            {
                std::cout << "No valid features at line " << lineNumber << ": " << line << std::endl;
                continue;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Error parsing features at line " << lineNumber << ": " << e.what() << std::endl;
            continue;
        }

        hashMap[record.imageId] = record;
        std::cout << "Loaded line " << lineNumber << ": imageId=" << record.imageId << ", sourcePath=" << record.sourcePath << std::endl;
    }

    file.close();
    std::cout << "Loaded " << hashMap.size() << " records from " << filename << std::endl;
    return true;
}

bool HashDatabase::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to open file for saving: " << filename << std::endl;
        return false;
    }

    for (const auto &entry : hashMap)
    {
        const ImageRecord &record = entry.second;
        file << record.imageId << "\t"
             << record.sourcePath << "\t"
             << (record.isOriginal ? "1" : "0") << "\t"
             << hashGenerator->hashToString(record.features) << std::endl;
        std::cout << "Saved: imageId=" << record.imageId << ", sourcePath=" << record.sourcePath << std::endl;
    }

    file.close();
    std::cout << "Saved " << hashMap.size() << " records to " << filename << std::endl;
    return true;
}