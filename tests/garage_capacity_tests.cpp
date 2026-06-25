#include "garage_capacity.h"

#include <gtest/gtest.h>
#include <stdexcept>

using pgof::garage_capacity;
using pgof::get_garage_capacity;

TEST(GarageCapacityTest, MinimumInput) {
    const garage_capacity capacity = get_garage_capacity(10);
    EXPECT_EQ(capacity.n1, 4) << "n1 for N=10 should be 4";
    EXPECT_EQ(capacity.n2, 3) << "n2 for N=10 should be 3";
    EXPECT_EQ(capacity.n3, 3) << "n3 for N=10 should be 3";
}

TEST(GarageCapacityTest, MaximumInput) {
    const garage_capacity capacity = get_garage_capacity(1000000);
    EXPECT_EQ(capacity.n1, 333334) << "n1 for N=1000000 should be 333334";
    EXPECT_EQ(capacity.n2, 333333) << "n2 for N=1000000 should be 333333";
    EXPECT_EQ(capacity.n3, 333333) << "n3 for N=1000000 should be 333333";
}

TEST(GarageCapacityTest, TotalAndBalance) {
    const int total_spaces = 101;
    const garage_capacity capacity = get_garage_capacity(total_spaces);
    const int sum = capacity.n1 + capacity.n2 + capacity.n3;

    EXPECT_EQ(sum, total_spaces) << "n1+n2+n3 should equal input N";
    EXPECT_LE(capacity.n1 - capacity.n3, 1) << "distribution should be balanced";
}

TEST(GarageCapacityTest, InvalidLowInput) {
    EXPECT_THROW(get_garage_capacity(9), std::out_of_range) << "N<10 should throw out_of_range";
}

TEST(GarageCapacityTest, InvalidHighInput) {
    EXPECT_THROW(get_garage_capacity(1000001), std::out_of_range) << "N>1000000 should throw out_of_range";
}
