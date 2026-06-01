#include "garage_capacity.h"

#include <stdexcept>

namespace pgof {

namespace {
constexpr int min_spaces = 10;
constexpr int max_spaces = 1000000;
}

garage_capacity get_garage_capacity(const unsigned int total_spaces) {
    if (total_spaces < min_spaces || total_spaces > max_spaces) {
        throw std::out_of_range("total_spaces must be in [10, 1000000]");
    }

    const int base = total_spaces / 3;
    const int remainder = total_spaces % 3;

    garage_capacity capacity{};
    capacity.n1 = base + (remainder > 0 ? 1 : 0);
    capacity.n2 = base + (remainder > 1 ? 1 : 0);
    capacity.n3 = base;

    return capacity;
}

}  // namespace pgof::garage