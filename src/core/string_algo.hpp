#if !defined(CORE_STRING_ALGO_HPP__)
#	define CORE_STRING_ALGO_HPP__

#	include "system_types.hpp"

namespace tt {
	template<typename String>
	constexpr String &append(String &lhs, uint32 count, char ch) {
		lhs.resize(lhs.size() + count, ch);
		return lhs;
	}

	template<typename String, typename StringLike>
	constexpr String &append(String &lhs, const StringLike &rhs) {
		return lhs += rhs;
	}

	template<typename String, typename InputIter>
	constexpr String &append(String &lhs, InputIter first, InputIter last) {
		auto len = std::distance(first, last);
		uint32 sz = lhs.size();
		lhs.resize(sz + len);

		char *data = lhs.data() + sz;
		const char *end = lhs.data() + lhs.size();
		for (; first != last && data != end; ++first, ++data) {
			*data = *first;
		}

		return lhs;
	}

	template<typename String>
	String substr(const String &str, uint32 pos, uint32 count) {
		if (pos >= str.size()) {
			return String();
		}

		uint32 c = tt::min(count, str.size() - pos);

		return String(str.data() + pos, c);
	}
}    // namespace tt

#endif CORE_STRING_ALGO_HPP__