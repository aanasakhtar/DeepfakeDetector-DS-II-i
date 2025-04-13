#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

class Hashtable
{
public:
    // Key: hamming distance, Value: vector of pairs (original_path, suspect_path)
    std::unordered_map<int, std::vector<std::pair<std::string, std::string>>> data;

    // Insert a new pair with associated hamming distance
    void insert(int hammingDistance, const std::string &originalPath, const std::string &suspectPath);

    // Get all image pairs with a specific hamming distance
    std::vector<std::pair<std::string, std::string>> getImagePairs(int hammingDistance);

    // Get all pairs with hamming distance above threshold
    std::vector<std::pair<std::string, std::string>> getPairsAboveThreshold(int threshold);

    // Get all pairs with hamming distance below threshold
    std::vector<std::pair<std::string, std::string>> getPairsBelowThreshold(int threshold);
};

#endif // HASHTABLE_H