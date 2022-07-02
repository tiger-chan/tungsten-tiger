#if !defined(CORE_STRING_HPP__)
#	define CORE_STRING_HPP__

#	include "system_types.hpp"
#	include "string_fixed.hpp"

namespace tt {
	struct string_view;
}

namespace tt {
	struct basic_string {
		using size_type = uint32;
		using value = char;

		static constexpr size_type npos = size_type(-1);

		basic_string();

		template<uint32 BUF_SIZE>
		basic_string(const char (&buf)[BUF_SIZE]);

		basic_string(const char *src);

		basic_string(const char *src, size_type count);

		basic_string(std::nullptr_t) = delete;

		basic_string(size_type count);

		basic_string(const basic_string &o);

		basic_string(basic_string &&o);

		~basic_string();

		// Element access
		char &at(size_type pos);

		const char &at(size_type pos) const;

		const char &operator[](size_type pos) const;

		char &operator[](size_type pos);

		const char *c_str() const;

		const char *data() const;

		char *data();

		const char &back() const;

		char &back();

		const char &front() const;

		char &front();

		operator string_view() const noexcept;

		// Capacity

		bool empty() const;

		size_type size() const;

		size_type length() const;

		size_type capacity() const;

		// Operations
		void reserve(size_type count);
		void resize(size_type count);
		void resize(size_type count, char ch);

		void shrink_to_fit();

		void clear();
		basic_string &erase(size_type index = 0, size_type count = npos);

		void push_back(char ch);
		void pop_back();

	private:
		static constexpr const size_type BUFFER_SIZE = 16;
		size_type len = 0;
		size_type cap = BUFFER_SIZE;
		union {
			value arr[BUFFER_SIZE] = {};
			value *ptr;
		};
	};

	using string = basic_string;

	template<uint32 BUF_SIZE>
	basic_string::basic_string(const char (&buf)[BUF_SIZE]) {
		if constexpr (BUF_SIZE <= BUFFER_SIZE) {
			tt::strncpy(arr, buf, BUFFER_SIZE);
			len = BUF_SIZE;
			cap = BUFFER_SIZE;
			ptr[BUFFER_SIZE - 1] = '\0';
		}
		else {
			len = BUF_SIZE;
			cap = ALIGN_UP(BUF_SIZE, 16);
			ptr = new value[cap];

			tt::strncpy(ptr, buf, cap);
			ptr[len - 1] = '\0';
		}
	}
}    // namespace tt

#	include <iterator>

// Extensions
namespace tt {
	extern basic_string operator+(const basic_string &lhs, const basic_string &rhs);
	extern basic_string &operator+=(basic_string &lhs, const basic_string &rhs);

	extern basic_string operator+(const basic_string &lhs, const char *rhs);
	extern basic_string operator+(const char *lhs, const basic_string &rhs);
	extern basic_string &operator+=(basic_string &lhs, const char *rhs);

	extern basic_string operator+(const basic_string &lhs, char rhs);
	extern basic_string operator+(char lhs, const basic_string &rhs);
	extern basic_string &operator+=(const basic_string &lhs, char rhs);

	template<typename StringViewLike>
	basic_string operator+(const basic_string &lhs, const StringViewLike &rhs) {
		basic_string r(lhs.size() + rhs.size());

		tt::strcpy(r.data(), lhs.data());
		tt::strcpy(r.data() + lhs.size(), rhs.data());

		return r;
	}

	template<typename StringViewLike>
	basic_string operator+(const StringViewLike &lhs, const basic_string &rhs) {
		basic_string r(lhs.size() + rhs.size());

		tt::strcpy(r.data(), lhs.data());
		tt::strcpy(r.data() + lhs.size(), rhs.data());

		return r;
	}

	template<typename StringViewLike>
	basic_string &operator+=(basic_string &lhs, const StringViewLike &rhs) {
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + rhs.size());

		tt::strcpy(lhs.data() + cur, rhs.data());

		return lhs;
	}

	template<uint32 Size>
	basic_string operator+(const basic_string &lhs, const basic_fixed_string<Size> &rhs) {
		basic_string r(lhs.size() + rhs.size());

		tt::strcpy(r.data(), lhs.data());
		tt::strcpy(r.data() + lhs.size(), rhs.data());

		return r;
	}

	template<uint32 Size>
	basic_string operator+(const basic_fixed_string<Size> &lhs, const basic_string &rhs) {
		basic_string r(lhs.size() + rhs.size());

		tt::strcpy(r.data(), lhs.data());
		tt::strcpy(r.data() + lhs.size(), rhs.data());

		return r;
	}

	template<uint32 Size>
	basic_string &operator+=(basic_string &lhs, const basic_fixed_string<Size> &rhs) {
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + rhs.size());

		tt::strcpy(lhs.data() + cur, rhs.data());

		return lhs;
	}

	template<uint32 Size>
	basic_string operator+(const basic_string &lhs, const char (&rhs)[Size]) {
		basic_string r(lhs.size() + Size);

		tt::strcpy(r.data(), lhs.data());
		tt::strncpy(r.data() + lhs.size(), rhs, Size);
		r.back() = '\0';

		return r;
	}

	template<uint32 Size>
	basic_string operator+(const char (&lhs)[Size], const basic_string &rhs) {
		basic_string r(Size + rhs.size());

		tt::strncpy(r.data(), lhs, Size);
		tt::strcpy(r.data() + Size, rhs.data());

		return r;
	}

	template<uint32 Size>
	basic_string &operator+=(basic_string &lhs, const char (&rhs)[Size]) {
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + Size);

		tt::strncpy(r.data() + cur, rhs, Size);
		r.back() = '\0';

		return lhs;
	}

	extern bool operator==(const basic_string &lhs, const basic_string &rhs);
	extern bool operator!=(const basic_string &lhs, const basic_string &rhs);
	extern bool operator<(const basic_string &lhs, const basic_string &rhs);
	extern bool operator>(const basic_string &lhs, const basic_string &rhs);
	extern bool operator<=(const basic_string &lhs, const basic_string &rhs);
	extern bool operator>=(const basic_string &lhs, const basic_string &rhs);

	extern bool operator==(const basic_string &lhs, const char *rhs);
	extern bool operator!=(const basic_string &lhs, const char *rhs);
	extern bool operator<(const basic_string &lhs, const char *rhs);
	extern bool operator>(const basic_string &lhs, const char *rhs);
	extern bool operator<=(const basic_string &lhs, const char *rhs);
	extern bool operator>=(const basic_string &lhs, const char *rhs);

	extern bool operator==(const char *lhs, const basic_string &rhs);
	extern bool operator!=(const char *lhs, const basic_string &rhs);
	extern bool operator<(const char *lhs, const basic_string &rhs);
	extern bool operator>(const char *lhs, const basic_string &rhs);
	extern bool operator<=(const char *lhs, const basic_string &rhs);
	extern bool operator>=(const char *lhs, const basic_string &rhs);

	template<typename StringViewLike>
	static bool operator==(const basic_string &lhs, const StringViewLike &rhs) {
		return lhs.size() == rhs.size() && tt::strcmp(lhs.data(), rhs.data()) == 0;
	}

	template<typename StringViewLike>
	static bool operator!=(const basic_string &lhs, const StringViewLike &rhs) {
		return !(lhs == rhs);
	}

	template<typename StringViewLike>
	static bool operator<(const basic_string &lhs, const StringViewLike &rhs) {
		return tt::strcmp(lhs.data(), rhs.data()) < 0;
	}

	template<typename StringViewLike>
	static bool operator>(const basic_string &lhs, const StringViewLike &rhs) {
		return tt::strcmp(lhs.data(), rhs.data()) > 0;
	}

	template<typename StringViewLike>
	static bool operator<=(const basic_string &lhs, const StringViewLike &rhs) {
		return !(lhs > rhs);
	}

	template<typename StringViewLike>
	static bool operator>=(const basic_string &lhs, const StringViewLike &rhs) {
		return !(lhs < rhs);
	}

	template<typename StringViewLike>
	static bool operator==(const StringViewLike &lhs, const basic_string &rhs) {
		return lhs.size() == rhs.size() && tt::strcmp(lhs.data(), rhs.data()) == 0;
	}

	template<typename StringViewLike>
	static bool operator!=(const StringViewLike &lhs, const basic_string &rhs) {
		return !(lhs == rhs);
	}

	template<typename StringViewLike>
	static bool operator<(const StringViewLike &lhs, const basic_string &rhs) {
		return tt::strcmp(lhs.data(), rhs.data()) < 0;
	}

	template<typename StringViewLike>
	static bool operator>(const StringViewLike &lhs, const basic_string &rhs) {
		return tt::strcmp(lhs.data(), rhs.data()) > 0;
	}

	template<typename StringViewLike>
	static bool operator<=(const StringViewLike &lhs, const basic_string &rhs) {
		return !(lhs > rhs);
	}

	template<typename StringViewLike>
	static bool operator>=(const StringViewLike &lhs, const basic_string &rhs) {
		return !(lhs < rhs);
	}
}    // namespace tt

namespace tt {
	namespace literals {
		static basic_string operator""_s(const char *str, size_t size) {
			return basic_string(str, size);
		}
	}    // namespace literals
}    // namespace tt

#	include <iostream>

extern std::ostream &operator<<(std::ostream &o, const tt::basic_string &str);
extern std::istream &operator>>(std::istream &i, tt::basic_string &str);

#endif    // CORE_STRING_HPP__