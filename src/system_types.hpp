#if !defined(SYSTEM_TYPES_HPP__)
#	define SYSTEM_TYPES_HPP__

namespace {
	// From https://stackoverflow.com/a/58846073/3261668

	template<typename T>
	constexpr bool is_signed() {
		return T(-1) < 0;
	}

	template<typename T>
	constexpr T max_size() {
		constexpr T size = sizeof(T);
		constexpr T bits = 8;
		constexpr T one = T(1);

		if constexpr (is_signed<T>()) {
			// 2^(sizeof(unsigned int) * 8 - 1) - 1
			return (one << (size * bits - 1)) - 1;
		}
		else {
			return ~T(0);
		}
	}

	template<typename T>
	constexpr T min_size() {
		if constexpr (is_signed<T>()) {
			return -max_size<T>() - T(1);
		}
		else {
			return T(0);
		}
	}
}    // namespace

using byte = unsigned char;    // 8 bits
using word = unsigned short;    // 16 bits
using dword = unsigned int;    // 32 bits
using uint = unsigned int;
using ulong = unsigned long;

using int8 = signed char;
using uint8 = unsigned char;
using int16 = short int;
using uint16 = unsigned short int;
using int32 = int;
using uint32 = unsigned int;
using int64 = long long;
using uint64 = unsigned long long;

template<typename T>
static constexpr T numeric_max = max_size<T>();

template<typename T>
static constexpr T numeric_min = min_size<T>();

static constexpr auto uint8_max = max_size<uint8>();
static constexpr auto uint8_min = min_size<uint8>();

static constexpr auto uint16_max = max_size<uint16>();
static constexpr auto uint16_min = min_size<uint16>();

static constexpr auto uint32_max = max_size<uint32>();
static constexpr auto uint32_min = min_size<uint32>();

static constexpr auto uint64_max = max_size<uint64>();
static constexpr auto uint64_min = min_size<uint64>();

static constexpr auto int8_max = max_size<int8>();
static constexpr auto int8_min = min_size<int8>();

static constexpr auto int16_max = max_size<int16>();
static constexpr auto int16_min = min_size<int16>();

static constexpr auto int32_max = max_size<int32>();
static constexpr auto int32_min = min_size<int32>();

static constexpr auto int64_max = max_size<int64>();
static constexpr auto int64_min = min_size<int64>();

namespace tt {
	template<typename T>
	static constexpr T min(const T &x, const T &y) {
		return x < y ? x : y;
	}

	template<typename T>
	static constexpr T max(const T &x, const T &y) {
		return x > y ? x : y;
	}

	template<typename T>
	static constexpr T abs(const T &v) {
		return v < 0 ? -v : v;
	}
}    // namespace tt

#	ifndef ALIGN_UP
// https://stackoverflow.com/a/4840428/3261668
#		define ALIGN_UP(x, a) ((x + (a - 1)) & ~(a - 1))
#		define ALIGN_DOWN(x, a) (x & ~(a - 1))

#	endif

#endif    // SYSTEM_TYPES_HPP__
