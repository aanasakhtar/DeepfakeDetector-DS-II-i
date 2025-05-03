#pragma once

#include <vector>
#include <string>

class HammingDistance
{
public:
    // Calculate Hamming distance between two binary vectors
    static int calculate(const std::vector<bool> &hash1, const std::vector<bool> &hash2)
    {
        if (hash1.size() != hash2.size())
        {
            // Handle error: hashes should be the same length
            return -1;
        }

        int distance = 0;
        for (size_t i = 0; i < hash1.size(); i++)
        {
            if (hash1[i] != hash2[i])
            {
                distance++;
            }
        }

        return distance;
    }

    // Calculate Hamming distance between two hash strings
    static int calculate(const std::string &hashStr1, const std::string &hashStr2)
    {
        if (hashStr1.size() != hashStr2.size())
        {
            // Handle error: hash strings should be the same length
            return -1;
        }

        int distance = 0;
        for (size_t i = 0; i < hashStr1.size(); i++)
        {
            unsigned char xorResult = hashStr1[i] ^ hashStr2[i];

            // Count set bits in xorResult (popcount)
            while (xorResult)
            {
                distance++;
                xorResult &= xorResult - 1; // Clear the least significant bit set
            }
        }

        return distance;
    }
};