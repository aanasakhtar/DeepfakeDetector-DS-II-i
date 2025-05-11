#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

class HashGenerator;

struct ImageRecord
{
    std::string imageId;
    std::vector<float> features; // Changed from hashString
    std::string sourcePath;
    bool isOriginal;
};

class HashDatabase
{
public:
    HashDatabase(std::shared_ptr<HashGenerator> hashGen, int hammingThreshold = 5);
    bool addHash(const std::string &imageId, const std::vector<float> &features,
                 const std::string &sourcePath, bool isOriginal = true);
    bool containsHash(const std::vector<float> &features) const;
    std::vector<ImageRecord> findSimilarHashes(const std::vector<float> &features) const;
    bool loadFromFile(const std::string &filename);
    bool saveToFile(const std::string &filename) const;
    size_t size() const { return hashMap.size(); }

private:
    std::unordered_map<std::string, ImageRecord> hashMap; // Key can be a unique ID or feature string
    std::shared_ptr<HashGenerator> hashGenerator;
    int hammingThreshold;
};