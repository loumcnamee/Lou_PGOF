#include "garage_capacity.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

using pgof::garage_capacity;
using pgof::get_garage_capacity;

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "Test failed: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

void test_minimum_input() {
    const garage_capacity capacity = get_garage_capacity(10);
    expect(capacity.n1 == 4, "n1 for N=10 should be 4");
    expect(capacity.n2 == 3, "n2 for N=10 should be 3");
    expect(capacity.n3 == 3, "n3 for N=10 should be 3");
}

void test_maximum_input() {
    const garage_capacity capacity = get_garage_capacity(1000000);
    expect(capacity.n1 == 333334, "n1 for N=1000000 should be 333334");
    expect(capacity.n2 == 333333, "n2 for N=1000000 should be 333333");
    expect(capacity.n3 == 333333, "n3 for N=1000000 should be 333333");
}

void test_total_and_balance() {
    const int total_spaces = 101;
    const garage_capacity capacity = get_garage_capacity(total_spaces);
    const int sum = capacity.n1 + capacity.n2 + capacity.n3;

    expect(sum == total_spaces, "n1+n2+n3 should equal input N");
    expect(capacity.n1 - capacity.n3 <= 1, "distribution should be balanced");
}

void test_invalid_low_input() {
    bool threw = false;
    try {
        (void)get_garage_capacity(9);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    expect(threw, "N<10 should throw out_of_range");
}

void test_invalid_high_input() {
    bool threw = false;
    try {
        (void)get_garage_capacity(1000001);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    expect(threw, "N>1000000 should throw out_of_range");
}

}  // namespace

int main() {
    test_minimum_input();
    test_maximum_input();
    test_total_and_balance();
    test_invalid_low_input();
    test_invalid_high_input();

    std::cout << "All garage capacity tests passed.\n";
    return 0;
}