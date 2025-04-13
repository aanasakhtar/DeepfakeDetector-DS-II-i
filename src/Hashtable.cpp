#include "Hashtable.h"

void Hashtable::insert(int hammingDistance, const std::string &originalPath, const std::string &suspectPath)
{
    data[hammingDistance].push_back(std::make_pair(originalPath, suspectPath));
}

std::vector<std::pair<std::string, std::string>> Hashtable::getImagePairs(int hammingDistance)
{
    if (data.find(hammingDistance) != data.end())
    {
        return data[hammingDistance];
    }
    return std::vector<std::pair<std::string, std::string>>();
}

std::vector<std::pair<std::string, std::string>> Hashtable::getPairsAboveThreshold(int threshold)
{
    std::vector<std::pair<std::string, std::string>> result;

    for (const auto &entry : data)
    {
        if (entry.first >= threshold)
        {
            result.insert(result.end(), entry.second.begin(), entry.second.end());
        }
    }

    return result;
}

std::vector<std::pair<std::string, std::string>> Hashtable::getPairsBelowThreshold(int threshold)
{
    std::vector<std::pair<std::string, std::string>> result;

    for (const auto &entry : data)
    {
        if (entry.first < threshold)
        {
            result.insert(result.end(), entry.second.begin(), entry.second.end());
        }
    }

    return result;
}