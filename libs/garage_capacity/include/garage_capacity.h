#pragma once

namespace pgof {

// Represents the number of spaces in the garage of each size 
struct garage_capacity {
    int n1; // Number of spaces in the first bucket (size 1)
    int n2; // Number of spaces in the second bucket (size 2)
    int n3; // Number of spaces in the third bucket (size 3)
};

// Splits N spaces into size 1/2/3 buckets as evenly as possible.
garage_capacity get_garage_capacity(const unsigned int total_spaces);

}  // namespace pgof::garage