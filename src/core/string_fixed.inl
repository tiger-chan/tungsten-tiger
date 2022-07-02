#include "string.hpp"
#include "string_util.hpp"
#include "string_view.hpp"

namespace tt {
	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE>::basic_fixed_string() {
		tt::strset(arr, '\0', BUFFER_SIZE);
		len = 0;
	}

	template<uint32 BUFFER_SIZE>
	template<uint32 BUF_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE>::basic_fixed_string(const char (&buf)[BUF_SIZE]) {
		if constexpr (BUF_SIZE <= BUFFER_SIZE) {
			tt::strcpy(arr, src);

			char *dest = arr + BUF_SIZE;
			tt::strset(dest, '\0', BUFFER_SIZE - BUF_SIZE);
			len = BUF_SIZE - 1;
		}
		else {
			tt::strncpy(arr, buf, BUFFER_SIZE);
			arr[BUFFER_SIZE - 1] = '\0';
			len = BUFFER_SIZE - 1;
		}
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE>::basic_fixed_string(const char *src) {
		len = tt::min(tt::strlen(src), BUFFER_SIZE - 1);
		tt::strncpy(arr, src, BUFFER_SIZE);
		arr[BUFFER_SIZE - 1] = '\0';
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE>::basic_fixed_string(const char *src, size_type count) {
		len = tt::min(count, BUFFER_SIZE - 1);
		tt::strncpy(arr, src, len);
		tt::strset(arr + len, '\0', BUFFER_SIZE - len);
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE>::basic_fixed_string(size_type count) {
		count = tt::min(BUFFER_SIZE - 1, count);
		tt::strset(arr, '\0', BUFFER_SIZE);
		len = count;
	}

	// Element access

	template<uint32 BUFFER_SIZE>
	constexpr char &basic_fixed_string<BUFFER_SIZE>::at(size_type pos) {
		return data()[pos];
	}

	template<uint32 BUFFER_SIZE>
	constexpr const char &basic_fixed_string<BUFFER_SIZE>::at(size_type pos) const {
		return data()[pos];
	}

	template<uint32 BUFFER_SIZE>
	constexpr char &basic_fixed_string<BUFFER_SIZE>::operator[](size_type pos) {
		return data()[pos];
	}

	template<uint32 BUFFER_SIZE>
	constexpr const char &basic_fixed_string<BUFFER_SIZE>::operator[](size_type pos) const {
		return data()[pos];
	}

	template<uint32 BUFFER_SIZE>
	constexpr const char *basic_fixed_string<BUFFER_SIZE>::c_str() const {
		return arr;
	}

	template<uint32 BUFFER_SIZE>
	constexpr const char *basic_fixed_string<BUFFER_SIZE>::data() const {
		return arr;
	}

	template<uint32 BUFFER_SIZE>
	constexpr char *basic_fixed_string<BUFFER_SIZE>::data() {
		return arr;
	}

	template<uint32 BUFFER_SIZE>
	constexpr const char &basic_fixed_string<BUFFER_SIZE>::back() const {
		return data()[size() - 1];
	}

	template<uint32 BUFFER_SIZE>
	constexpr char &basic_fixed_string<BUFFER_SIZE>::back() {
		return data()[size() - 1];
	}

	template<uint32 BUFFER_SIZE>
	constexpr const char &basic_fixed_string<BUFFER_SIZE>::front() const {
		return *data();
	}

	template<uint32 BUFFER_SIZE>
	constexpr char &basic_fixed_string<BUFFER_SIZE>::front() {
		return *data();
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE>::operator string_view() const noexcept {
		return string_view(arr, size());
	}

	// Capacity

	template<uint32 BUFFER_SIZE>
	constexpr uint32 basic_fixed_string<BUFFER_SIZE>::size() const {
		return len;
	}

	template<uint32 BUFFER_SIZE>
	constexpr uint32 basic_fixed_string<BUFFER_SIZE>::length() const {
		return len;
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool basic_fixed_string<BUFFER_SIZE>::empty() const {
		return len == 0;
	}

	template<uint32 BUFFER_SIZE>
	constexpr uint32 basic_fixed_string<BUFFER_SIZE>::capacity() const {
		constexpr static const uint32 cap = BUFFER - 1;
		return cap;
	}

	// Operations
	template<uint32 BUFFER_SIZE>
	constexpr void basic_fixed_string<BUFFER_SIZE>::resize(size_type count) {
		resize(count, '\0');
	}

	template<uint32 BUFFER_SIZE>
	constexpr void basic_fixed_string<BUFFER_SIZE>::resize(size_type count, char ch) {
		size_type c = std::min(count, BUFFER_SIZE - 1);
		if (count < len) {
			tt::strset(data() + count, '\0', len - count);
			len = count;
		}
		else {
			tt::strset(data() + len, ch, c - len);
			tt::strset(data() + c, '\0', BUFFER_SIZE - c);
			len = c;
		}
	}

	template<uint32 BUFFER_SIZE>
	constexpr void basic_fixed_string<BUFFER_SIZE>::clear() {
		tt::strset(arr, '\0', len);
		len = 0;
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE> &basic_fixed_string<BUFFER_SIZE>::erase(size_type index, size_type count) {
		// Starting at data() + index copy the characters from data() + index + count
		if (size() <= index) {
			return *this;
		}

		size_type max_c = size() - index;
		count = count > max_c ? max_c : count;
		size_type e = size() - count;

		if (e == 0) {
			tt::strset(data() + index, '\0', count);
		}
		else {
			char *d = data();
			uint32 i = index;
			uint32 j = i + count;
			uint32 k = j + e;
			for (; j < k; ++i, ++j) {
				d[i] = d[j];
			}
			tt::strset(d + i, '\0', e);
		}

		len = index;

		return *this;
	}
}    // namespace tt

namespace tt {
	template<uint32 LSize, uint32 RSize>
	constexpr basic_fixed_string<LSize + RSize> operator+(const basic_fixed_string<LSize> &lhs, const basic_fixed_string<RSize> &rhs) {
		basic_fixed_string<LSize + RSize> r{ lhs.size() + rhs.size() };

		tt::strcpy(r.data(), lhs.data());
		tt::strcpy(r.data() + lhs.size(), rhs.data());

		return r;
	}

	template<uint32 LSize, uint32 RSize>
	constexpr basic_fixed_string<LSize> &operator+=(basic_fixed_string<LSize> &lhs, const basic_fixed_string<RSize> &rhs) {
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + rhs.size());
		tt::strncpy(lhs.data() + cur, rhs.data(), lhs.size() - cur);

		return lhs;
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE> operator+(const basic_fixed_string<BUFFER_SIZE> &lhs, const char *rhs) {
		uint32 r_size = tt::strlen(rhs);
		basic_fixed_string<BUFFER_SIZE> r{ lhs.size() + r_size };

		tt::strncpy(r.data(), lhs.data(), lhs.size());
		tt::strncpy(r.data() + lhs.size(), rhs, r.size() - lhs.size());

		return r;
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE> operator+(const char *lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		uint32 l_size = tt::strlen(lhs);
		basic_fixed_string<BUFFER_SIZE> r{ l_size + rhs.size() };

		// Cap the size so it doesn't overflow the buffer
		l_size = tt::min(l_size, r.size());
		tt::strncpy(r.data(), lhs, l_size);
		tt::strncpy(r.data() + l_size, rhs.data(), r.size() - l_size);

		return r;
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE> &operator+=(basic_fixed_string<BUFFER_SIZE> &lhs, const char *rhs) {
		uint32 r_size = tt::strlen(rhs);
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + r_size);

		tt::strncpy(lhs.data() + cur, rhs, lhs.size() - cur);

		return lhs;
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE> operator+(const basic_fixed_string<BUFFER_SIZE> &lhs, char rhs) {
		basic_fixed_string<BUFFER_SIZE> r{ lhs.size() + 1 };

		tt::strncpy(r.data(), lhs.data(), lhs.size());
		tt::strset(r.data() + lhs.size(), rhs, 1);

		return r;
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE> operator+(char lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		basic_fixed_string<BUFFER_SIZE> r{ 1 + rhs.size() };

		tt::strset(r.data(), lhs, 1);
		tt::strncpy(r.data() + 1, rhs.data(), r.size() - 1);

		return r
	}

	template<uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE> &operator+=(const basic_fixed_string<BUFFER_SIZE> &lhs, char rhs) {
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + 1);

		tt::strncpy(r.data(), lhs.data(), lhs.size());
		tt::strset(r.data() + lhs.size(), rhs, r.size() - lhs.size());

		return lhs;
	}

	template<uint32 BUFFER_SIZE, typename StringViewLike>
	constexpr basic_fixed_string<BUFFER_SIZE> operator+(const basic_fixed_string<BUFFER_SIZE> &lhs, const StringViewLike &rhs) {
		basic_fixed_string<BUFFER_SIZE> r{ lhs.size() + rhs.size() };

		tt::strcpy(r.data(), lhs.data());
		tt::strncpy(r.data() + lhs.size(), rhs.data(), r.size() - lhs.size());

		return r;
	}

	template<typename StringViewLike, uint32 BUFFER_SIZE>
	constexpr basic_fixed_string<BUFFER_SIZE> operator+(const StringViewLike &lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		uint32 l_size = lhs.size();
		basic_fixed_string<BUFFER_SIZE> r{ l_size + rhs.size() };

		// Cap the size so it doesn't overflow the buffer
		l_size = tt::min(l_size, r.size());
		tt::strncpy(r.data(), lhs, l_size);
		tt::strncpy(r.data() + l_size, rhs.data(), r.size() - l_size);

		return r;
	}

	template<uint32 BUFFER_SIZE, typename StringViewLike>
	constexpr basic_fixed_string<BUFFER_SIZE> &operator+=(basic_fixed_string<BUFFER_SIZE> &lhs, const StringViewLike &rhs) {
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + rhs.size());
		tt::strncpy(lhs.data() + cur, rhs.data(), lhs.size() - cur);

		return lhs;
	}

	template<uint32 BUFFER_SIZE>
	basic_fixed_string<BUFFER_SIZE> &operator+=(basic_fixed_string<BUFFER_SIZE> &lhs, const basic_string &rhs) {
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + rhs.size());
		tt::strncpy(lhs.data() + cur, rhs.data(), lhs.size() - cur);

		return lhs;
	}
}    // namespace tt

// Equality Operators
namespace tt {
	template<uint32 LSize, uint32 RSize>
	constexpr bool operator==(const basic_fixed_string<LSize> &lhs, const basic_fixed_string<RSize> &rhs) {
		return lhs.size() == rhs.size() && tt::strcmp(lhs.data(), rhs.data()) == 0;
	}

	template<uint32 LSize, uint32 RSize>
	constexpr bool operator!=(const basic_fixed_string<LSize> &lhs, const basic_fixed_string<RSize> &rhs) {
		return !(lhs == rhs);
	}

	template<uint32 LSize, uint32 RSize>
	constexpr bool operator<(const basic_fixed_string<LSize> &lhs, const basic_fixed_string<RSize> &rhs) {
		return tt::strcmp(lhs.data(), rhs.data()) < 0;
	}

	template<uint32 LSize, uint32 RSize>
	constexpr bool operator>(const basic_fixed_string<LSize> &lhs, const basic_fixed_string<RSize> &rhs) {
		return tt::strcmp(lhs.data(), rhs.data()) > 0;
	}

	template<uint32 LSize, uint32 RSize>
	constexpr bool operator<=(const basic_fixed_string<LSize> &lhs, const basic_fixed_string<RSize> &rhs) {
		return !(lhs > rhs);
	}

	template<uint32 LSize, uint32 RSize>
	constexpr bool operator>=(const basic_fixed_string<LSize> &lhs, const basic_fixed_string<RSize> &rhs) {
		return !(lhs < rhs);
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator==(const basic_fixed_string<BUFFER_SIZE> &lhs, const char *rhs) {
		return tt::strcmp(lhs.data(), rhs) == 0;
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator!=(const basic_fixed_string<BUFFER_SIZE> &lhs, const char *rhs) {
		return !(lhs == rhs);
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator<(const basic_fixed_string<BUFFER_SIZE> &lhs, const char *rhs) {
		return tt::strcmp(lhs.data(), rhs) < 0;
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator>(const basic_fixed_string<BUFFER_SIZE> &lhs, const char *rhs) {
		return tt::strcmp(lhs.data(), rhs) > 0;
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator<=(const basic_fixed_string<BUFFER_SIZE> &lhs, const char *rhs) {
		return !(lhs > rhs);
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator>=(const basic_fixed_string<BUFFER_SIZE> &lhs, const char *rhs) {
		return !(lhs < rhs);
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator==(const char *lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		return tt::strcmp(lhs, rhs.data()) == 0;
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator!=(const char *lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		return !(lhs == rhs);
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator<(const char *lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		return tt::strcmp(lhs, rhs.data()) < 0;
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator>(const char *lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		return tt::strcmp(lhs, rhs.data()) > 0;
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator<=(const char *lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		return !(lhs > rhs);
	}

	template<uint32 BUFFER_SIZE>
	constexpr bool operator>=(const char *lhs, const basic_fixed_string<BUFFER_SIZE> &rhs) {
		return !(lhs < rhs);
	}
}    // namespace tt

template<uint32 BUFFER_SIZE>
std::ostream &operator<<(std::ostream &o, const tt::basic_fixed_string<BUFFER_SIZE> &str) {
	return o << str.c_str();
}

template<uint32 BUFFER_SIZE>
std::istream &operator>>(std::istream &i, tt::basic_fixed_string<BUFFER_SIZE> &str) {
	using istream = std::istream;
	using ctype = std::ctype<char>;
	using string = tt::basic_fixed_string<BUFFER_SIZE>;
	using size_type = typename string::size_type;
	using traits = tt::char_traits<char>;

	typename istream::iostate io_state = istream::goodbit;
	bool string_changed = false;
	const typename istream::sentry i_sentry(i);

	if (i_sentry) {    // state okay, extract characters
		const ctype &facet = std::use_facet<ctype>(i.getloc());
		str.erase();

		try {
			size_type sz;
			if (0 < i.width() && static_cast<size_type>(i.width()) < str.npos - 1) {
				sz = static_cast<size_type>(i.width());
			}
			else {
				sz = str.npos - 1;
			}

			typename traits::int_type next_val = i.rdbuf()->sgetc();

			for (; 0 < sz; --sz, next_val = i.rdbuf()->snextc()) {
				if (traits::eq_int_type(traits::eof(), next_val)) {    // end of file, quit
					io_state |= istream::eofbit;
					break;
				}
				else if (facet.is(ctype::space, traits::to_char_type(next_val))) {
					break;    // whitespace, quit
				}
				else {    // add character to string
					str.push_back(traits::to_char_type(next_val));
					string_changed = true;
				}
			}
		}
		catch (...) {
			i.setstate(istream::badbit, true);
		}
	}

	i.width(0);
	if (!string_changed) {
		io_state |= istream::failbit;
	}

	i.setstate(io_state);
	return i;
}
