#ifndef HASH_COMPARATOR_H
#define HASH_COMPARATOR_H

#include <string>

class HashComparator
{
public:
    static int hammingDistance(const std::string &hash1, const std::string &hash2);
    static bool isDeepfake(int distance, int threshold);
};

#endif // HASH_COMPARATOR_H
