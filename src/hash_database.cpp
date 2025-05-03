#include "hash_database.hpp"
#include "hash_generator.hpp"
#include "hamming.hpp"
#include <fstream>
#include <sstream>

HashDatabase::HashDatabase(std::shared_ptr<HashGenerator> hashGen, int hammingThreshold)
    : hashGenerator(hashGen), hammingThreshold(hammingThreshold) {}

bool HashDatabase::addHash(const std::string &imageId, const std::vector<bool> &hash,
                           const std::string &sourcePath, bool isOriginal)
{
    // Convert hash to string for storage
    std::string hashStr = hashGenerator->hashToString(hash);

    // Check if this hash already exists
    if (hashMap.find(hashStr) != hashMap.end())
    {
        return false; // Hash already exists
    }

    // Add to database
    ImageRecord record;
    record.imageId = imageId;
    record.hashString = hashStr;
    record.sourcePath = sourcePath;
    record.isOriginal = isOriginal;

    hashMap[hashStr] = record;
    return true;
}

bool HashDatabase::containsHash(const std::vector<bool> &hash) const
{
    std::string hashStr = hashGenerator->hashToString(hash);
    return hashMap.find(hashStr) != hashMap.end();
}

std::vector<ImageRecord> HashDatabase::findSimilarHashes(const std::vector<bool> &queryHash) const
{
    std::vector<ImageRecord> results;
    std::string queryHashStr = hashGenerator->hashToString(queryHash);

    for (const auto &entry : hashMap)
    {
        const std::string &storedHashStr = entry.first;
        const ImageRecord &record = entry.second;

        // Convert stored hash string back to bool vector for Hamming comparison
        std::vector<bool> storedHash = hashGenerator->stringToHash(storedHashStr);

        // Calculate Hamming distance
        int distance = HammingDistance::calculate(queryHash, storedHash);

        // If distance is within threshold, add to results
        if (distance <= hammingThreshold)
        {
            results.push_back(record);
        }
    }

    return results;
}

bool HashDatabase::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    hashMap.clear();
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        ImageRecord record;
        std::string isOriginalStr;

        if (!(iss >> record.imageId >> record.hashString >> record.sourcePath >> isOriginalStr))
        {
            continue; // Skip malformed lines
        }

        record.isOriginal = (isOriginalStr == "1" || isOriginalStr == "true");
        hashMap[record.hashString] = record;
    }

    file.close();
    return true;
}

bool HashDatabase::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    for (const auto &entry : hashMap)
    {
        const ImageRecord &record = entry.second;
        file << record.imageId << " "
             << record.hashString << " "
             << record.sourcePath << " "
             << (record.isOriginal ? "1" : "0") << std::endl;
    }

    file.close();
    return true;
}