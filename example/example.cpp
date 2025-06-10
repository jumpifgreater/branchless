#include "include/branchless.h"
#include <iostream>

int main() {
	std::cout << "---- Branchless Library Example ----\n\n";

	// branchless operations
	int a = 42, b = 18;

	std::cout << "min(" << a << ", " << b << ") = " << branchless::min_branchless(a, b) << "\n";
	std::cout << "max(" << a << ", " << b << ") = " << branchless::max_branchless(a, b) << "\n";
	std::cout << "abs(-25) = " << branchless::absVal_branchless(-25) << "\n";

	std::cout << "\n---- Performance Benchmark ----\n";
	branchless::benchmark_comparison();
}