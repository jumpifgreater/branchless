#include "branchless.h"
#include <chrono>
#include <iostream>

namespace branchless {
	bool branchlessStrCmp_branchless(const char* a, const char* b, uint len) {
		ull diff = 0;
		for (int i = 0; i < len; i++) {
			diff |= a[i] ^ b[i];
		}

		return !diff;
	}

	void memcpyConditional_branchless(void* dest, void* src, int size, bool condition) {
		auto* s = static_cast<const char*>(src);
		auto* d = static_cast<char*>(dest);
		auto mask = static_cast<uint>(-static_cast<int>(condition));

		for (int i = 0; i < size; i++) {
			d[i] = (d[i] & ~mask) | (s[i] & mask);
		}
	}

	template<typename T>
	T setBitConditional_branchless(T value, int bit, bool condition) {
		auto mask = T(1) << bit;
		return value ^ ((-T(condition) ^ value) & mask);
	}

	template<typename T, uint N>
	uint find_branchless(std::array<T, N> arr, T target) {
		auto result = N;
		for (int i = 0; i < N; i++) {
			auto found = (arr[i] == target);
			result = select_branchless<uint>(found, i, result);
		}
		return result;
	}


	template<typename T>
	uint find_branchless(std::vector<T> vec, T target) {
		auto result = vec.size();
		for (uint i = 0; i < vec.size(); ++i) {
			auto found = (vec[i] == target);
			result = select(found, i, result);
		}
		return result;
	}

	template<typename T>
	uint binarySearch_branchless(T* arr, uint size, T target) {
		uint left = 0;
		uint right = size;

		while (left < right) {
			auto mid = left + ((right - left) >> 1);
			auto goRight = arr[mid] < target;
			left = select(goRight, mid + 1, left);
			right = select(goRight, right, mid);
		}

		return left;
	}

	namespace crypto {

		bool secure_compare(byte* a, byte* b, uint len) {
			byte diff = 0;
			for (uint i = 0; i < len; i++) {
				diff |= a[i] ^ b[i];
			}
			return !diff;
		}

		void conditional_move(byte* dest, byte* src, uint len, bool condition) {
			auto mask = -(byte)condition;
			for (uint i = 0; i < len; i++) {
				dest[i] ^= mask & (dest[i] ^ src[i]);
			}
		}
	};

	/*

		BENCHMARK SKIDDED FROM SOME RANDOM GUY BUT CREDITS TO HIM
   
	*/

	void benchmark_comparison() {
		uint iterations = 1000000;

		auto start = std::chrono::high_resolution_clock::now();
		volatile int result1 = 0;
		for (size_t i = 0; i < iterations; ++i) {
			int x = rand();
			if (x > 0) result1 += x;
			else if (x < 0) result1 -= x;
			else result1 += 1;
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto branched_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		start = std::chrono::high_resolution_clock::now();
		volatile int result2 = 0;
		for (size_t i = 0; i < iterations; ++i) {
			int x = rand();
			int pos_mask = -(x > 0);
			int neg_mask = -(x < 0);
			int zero_mask = -(x == 0);
			result2 += (x & pos_mask) + (-x & neg_mask) + (1 & zero_mask);
		}
		end = std::chrono::high_resolution_clock::now();
		auto branchless_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		std::cout << "Branched: " << branched_time.count() << " micro s\n";
		std::cout << "Branchless: " << branchless_time.count() << " micro s\n";
		std::cout << "Speedup: " << (double)branched_time.count() / branchless_time.count() << "x\n";
	}
}
