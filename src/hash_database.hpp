#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

// Forward declaration
class HashGenerator;

struct ImageRecord
{
    std::string imageId;    // Unique identifier for the image
    std::string hashString; // String representation of the hash // we can delete this
    std::string sourcePath; // Original path or identifier
    bool isOriginal;        // Whether this is an original or already flagged
};

class HashDatabase
{
public:
    HashDatabase(std::shared_ptr<HashGenerator> hashGen, int hammingThreshold = 5);

    // Add a hash to the database
    bool addHash(const std::string &imageId, const std::vector<bool> &hash,
                 const std::string &sourcePath, bool isOriginal = true);

    // Check if a hash exists in the database (exact match)
    bool containsHash(const std::vector<bool> &hash) const;

    // Find similar hashes within Hamming distance threshold
    std::vector<ImageRecord> findSimilarHashes(const std::vector<bool> &hash) const;

    // Load database from file
    bool loadFromFile(const std::string &filename);

    // Save database to file
    bool saveToFile(const std::string &filename) const;

    // Get count of hashes in database
    size_t size() const { return hashMap.size(); }

private:
    std::unordered_map<std::string, ImageRecord> hashMap;
    std::shared_ptr<HashGenerator> hashGenerator;
    int hammingThreshold;
};