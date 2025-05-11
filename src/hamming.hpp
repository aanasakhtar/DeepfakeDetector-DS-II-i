#pragma once
#include <vector>
#include <string>
#include <cmath>

class HammingDistance
{
public:
    static int calculate(const std::vector<bool> &hash1, const std::vector<bool> &hash2)
    {
        if (hash1.size() != hash2.size()) return -1;
        int distance = 0;
        for (size_t i = 0; i < hash1.size(); i++)
            if (hash1[i] != hash2[i]) distance++;
        return distance;
    }

    static int calculate(const std::string &hashStr1, const std::string &hashStr2)
    {
        if (hashStr1.size() != hashStr2.size()) return -1;
        int distance = 0;
        for (size_t i = 0; i < hashStr1.size(); i++)
        {
            unsigned char xorResult = hashStr1[i] ^ hashStr2[i];
            while (xorResult) { distance++; xorResult &= xorResult - 1; }
        }
        return distance;
    }
};

class SimilarityMetrics
{
public:
    static double calculateCosine(const std::vector<float> &hash1, const std::vector<float> &hash2)
    {
        if (hash1.size() != hash2.size()) return 0.0;
        double dot = 0, mag1 = 0, mag2 = 0;
        for (size_t i = 0; i < hash1.size(); i++)
        {
            dot += hash1[i] * hash2[i];
            mag1 += hash1[i] * hash1[i];
            mag2 += hash2[i] * hash2[i];
        }
        double denominator = std::sqrt(mag1) * std::sqrt(mag2);
        return denominator == 0 ? 0 : dot / denominator;
    }
};