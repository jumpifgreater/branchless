#pragma once
#include <type_traits>
#include <cstdint>
#include <array>
#include <vector>

/*
   ----------------------------
   |					      |
   |  MADE BY JUMPIFGREATER   |
   |						  |
   ----------------------------
*/

typedef unsigned int uint;
typedef unsigned long long ull;
typedef unsigned char byte;

namespace branchless {
	template <typename T>
	T select_branchless(bool condition, T ifTrue, T ifFalse) {
		return ifFalse ^ ((ifTrue ^ ifFalse) & -(T)condition);
	}

	template <typename T>
	bool isEqual_branchless(T a, T b) {
		return !((a - b) | (b - a));
	}

	template <typename T>
	bool isGreater_branchless(T a, T b) {
		return ((long long)(a) - (long long)(b)) > 0;
	}

	template <typename T>
	bool isLess_branchless(T a, T b) {
		return ((long long)(b) - (long long)(a)) > 0;
	}

	bool branchlessStrCmp_branchless(const char* a, const char* b, uint len);

	void memcpyConditional_branchless(void* dest, void* src, int size, bool condition);

	template<typename T>
	T setBitConditional_branchless(T value, int bit, bool condition);

	template<typename T, uint N>
	uint find_branchless(std::array<T, N> arr, T target);


	template<typename T>
	uint find_branchless(std::vector<T> vec, T target);

	template<typename T>
	uint binary_search_branchless(T* arr, uint size, T target);

	namespace crypto {

		bool secure_compare(byte* a, byte* b, uint len);

		void conditional_move(byte* dest, byte* src, uint len, bool condition);
	};

	template <typename T>
	T min_branchless(T a, T b) {
		return b ^ ((a ^ b) & -(a < b));
	}

	template <typename T>
	T max_branchless(T a, T b) {
		return a ^ ((a ^ b) & -(a < b));
	}

	template <typename T>
	T absVal_branchless(T x) {
		auto mask = x >> (sizeof(T) * 8 - 1);
		return (x + mask) ^ mask;
	}

	template <typename T>
	T sign_branchless(T x) {
		return (T(0) < x) - (x < T(0));
	}

	template <typename T>
	T clamp_branchless(T value, T minVal, T maxVal) {
		return max(minVal, min(value, maxVal));
	}

	template <uint N, typename T>
	class BranchlessSwitch {
	private:
		std::array<T, N> lookupTable;
	public:
		BranchlessSwitch(std::array<T, N> table) : lookupTable(table) {}

		T operator()(uint idx) {
			auto safeIdx = idx & (N - 1);
			return lookupTable[safeIdx];
		}
	};

	template <typename T>
	class BranchlessVectorSwitch {
	private:
		std::vector<T> lookupTable;
		uint mask;
	public:
		BranchlessVectorSwitch(std::vector<T> table) : lookupTable(std::move(table)) {
			mask = (lookupTable.size() & (lookupTable.size() - 1)) == 0 ? lookupTable.size() - 1 : (1 << (32 - __builtin_clz(lookupTable.size()))) - 1;
		}

		T operator()(uint idx) {
			return lookupTable[idx & mask];
		}
	};

	void benchmark_comparison();
}
