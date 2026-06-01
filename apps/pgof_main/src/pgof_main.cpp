#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include  "garage_capacity.h"

using namespace pgof;

// PGOF main application entry point
int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
    
    const unsigned int total_spaces = 100;
	const garage_capacity initial_capacity = get_garage_capacity(total_spaces);
	(void)initial_capacity;

    std::cout << "Initial garage capacity: "
              << "n1=" << initial_capacity.n1 << ", "
              << "n2=" << initial_capacity.n2 << ", "
              << "n3=" << initial_capacity.n3 << '\n';

	return 0;
}
