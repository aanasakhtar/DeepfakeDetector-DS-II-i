#include "HashComparator.h"

int HashComparator::hammingDistance(const std::string &hash1, const std::string &hash2)
{
    int count = 0;
    int len = std::min(hash1.length(), hash2.length());
    for (int i = 0; i < len; ++i)
    {
        if (hash1[i] != hash2[i])
        {
            count++;
        }
    }
    return count;
}

bool HashComparator::isDeepfake(int distance, int threshold)
{
    return distance > threshold;
}
