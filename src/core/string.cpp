#include "string.hpp"

#include "macro.hpp"
#include "string_util.hpp"
#include "string_view.hpp"

namespace tt {
	basic_string::basic_string() {
		tt::strset(arr, '\0', BUFFER_SIZE);
		len = 0;
		cap = BUFFER_SIZE;
	}

	basic_string::basic_string(const char *src) {
		const char *s = src;
		if (!s) {
			len = 0;
			tt::strset(arr, '\0', BUFFER_SIZE);
		}
		else {
			const uint32 sz = tt::strlen(src);
			cap = BUFFER_SIZE;
			if (sz < BUFFER_SIZE) {
				tt::strcpy(arr, src);
				len = sz;
				char *d = arr;
				tt::strset(d + len, '\0', BUFFER_SIZE - sz);
				len = sz;
			}
			else {
				len = sz - 1;
				cap = ALIGN_UP(sz, 16);
				ptr = new value[cap];
				tt::strcpy(ptr, src);
				tt::strset(ptr + len, '\0', cap - len);
			}
		}
	}

	basic_string::basic_string(const char *src, size_type count) {
		reserve(count + 1);
		len = count;
		tt::strncpy(data(), src, len);
	}

	basic_string::basic_string(size_type count) {
		if (count < BUFFER_SIZE) {
			tt::strset(arr, '\0', BUFFER_SIZE);
			len = count;
			cap = BUFFER_SIZE;
		}
		else {
			cap = ALIGN_UP(count, 16);
			ptr = new char[cap];
			tt::strset(ptr, '\0', cap);
			len = count;
		}
	}

	basic_string::basic_string(const basic_string &o) {
		if (o.capacity() <= BUFFER_SIZE) {
			tt::strncpy(arr, o.data(), BUFFER_SIZE);
			len = o.size();
			cap = BUFFER_SIZE;
		}
		else {
			cap = ALIGN_UP(o.capacity(), 16);
			ptr = new char[cap];
			tt::strncpy(ptr, o.data(), cap);
			len = o.size();
		}
	}

	basic_string::basic_string(basic_string &&o) {
		len = o.len;
		cap = o.cap;

		if (len >= BUFFER_SIZE) {
			ptr = o.ptr;
			o.ptr = nullptr;
		}
		else {
			tt::strncpy(arr, o.arr, BUFFER_SIZE);
		}
	}

	basic_string::~basic_string() {
		if (cap > BUFFER_SIZE) {
			delete ptr;
			len = 0;
			cap = BUFFER_SIZE;
		}
	}

	// Element access
	char &basic_string::at(size_type pos) {
		// TODO: ASSERT(pos < size());
		return data()[pos];
	}

	const char &basic_string::at(size_type pos) const {
		// TODO: ASSERT(pos < size());
		return data()[pos];
	}

	const char &basic_string::operator[](size_type pos) const {
		return data()[pos];
	}

	char &basic_string::operator[](size_type pos) {
		return data()[pos];
	}

	const char *basic_string::c_str() const {
		return len > BUFFER_SIZE ? ptr : arr;
	}

	const char *basic_string::data() const {
		return len > BUFFER_SIZE ? ptr : arr;
	}

	char *basic_string::data() {
		return len > BUFFER_SIZE ? ptr : arr;
	}

	const char &basic_string::back() const {
		return data()[size() - 1];
	}

	char &basic_string::back() {
		return data()[size() - 1];
	}

	const char &basic_string::front() const {
		return *data();
	}

	char &basic_string::front() {
		return *data();
	}

	basic_string::operator string_view() const noexcept {
		return string_view{ *this };
	}

	bool basic_string::empty() const {
		return len == 0;
	}

	// Capacity

	uint32 basic_string::size() const {
		return len;
	}

	uint32 basic_string::length() const {
		return len;
	}

	uint32 basic_string::capacity() const {
		return cap;
	}

	void basic_string::reserve(size_type count) {
		size_type new_cap = ALIGN_UP(count < cap ? cap : count + 1, 16);

		if (cap > BUFFER_SIZE) {
			// We are using a pointer already we will need to clean it up before we finish if we are growing.
			if (new_cap > cap) {
				// growing
				char *p = new char[new_cap];
				tt::strncpy(p, ptr, cap);
				tt::strset(p + cap, '\0', new_cap - cap);
				cap = new_cap;
				delete ptr;
				ptr = p;
			}
		}
		else {
			// We are currently using the buffer array, nothing to cleanup
			if (new_cap > cap) {
				// growing
				char *p = new char[new_cap];
				tt::strncpy(p, arr, cap);
				tt::strset(p + cap, '\0', new_cap - cap);
				cap = new_cap;
				ptr = p;
			}
		}
	}

	void basic_string::resize(size_type count) {
		resize(count, '\0');
	}

	void basic_string::resize(size_type count, char ch) {
		// Always round up to the nearest multiple of 16 to ensure constant block sizes
		size_type new_cap = ALIGN_UP(count < cap ? cap : count + 1, 16);

		if (cap > BUFFER_SIZE) {
			// We are using a pointer already we will need to clean it up before we finish if we are growing.
			if (new_cap > cap) {
				// growing
				char *p = new char[new_cap];
				tt::strncpy(p, ptr, len);
				tt::strset(p + len, ch, count - len);
				tt::strset(p + count, '\0', new_cap - count);
				p[count] = '\0';
				cap = new_cap;
				len = count;
				delete ptr;
				ptr = p;
			}
			else if (count < len) {
				tt::strset(ptr + count, '\0', len - count);
				len = count;
			}
			else {
				tt::strset(ptr + len, ch, count - len);
				tt::strset(ptr + count, '\0', cap - count);
				len = count;
				ptr[count] = '\0';
			}
		}
		else {
			// We are currently using the buffer array, nothing to cleanup
			if (new_cap > cap) {
				// growing
				char *p = new char[new_cap];
				tt::strncpy(p, arr, len);
				tt::strset(p + len, ch, count - len);
				tt::strset(p + count, '\0', new_cap - count);
				p[count] = '\0';
				cap = new_cap;
				len = count;
				ptr = p;
			}
			else if (count < len) {
				tt::strset(arr + count, '\0', len - count);
				len = count;
				arr[count] = '\0';
			}
			else {
				tt::strset(arr + len, ch, count - len);
				tt::strset(arr + count, '\0', cap - count);
				len = count;
				arr[count] = '\0';
			}
		}
	}

	void basic_string::shrink_to_fit() {
		// Always round up to the nearest multiple of 16 to ensure constant block sizes
		if (cap > BUFFER_SIZE) {
			// We are shrinking a pointer.
			size_type new_cap = ALIGN_UP(len + 1, 16);
			if (new_cap == cap) {
				// the cap is already the correct size
				return;
			}
			if (new_cap <= BUFFER_SIZE) {
				char *p = ptr;
				tt::strncpy(arr, p, BUFFER_SIZE);
				delete p;
			}
			else {
				char *p = new char[new_cap];
				tt::strncpy(p, ptr, new_cap);
				delete ptr;
				ptr = p;
			}

			cap = new_cap;
		}
	}

	void basic_string::clear() {
		tt::strset(data(), '\0', len);
	}

	basic_string &basic_string::erase(size_type index, size_type count) {
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

	void basic_string::push_back(char ch) {
		resize(size() + 1);
		data()[len - 1] = ch;
	}

	void basic_string::pop_back() {
		data()[--len] = '\0';
	}
}    // namespace tt

namespace tt {
	basic_string operator+(const basic_string &lhs, const basic_string &rhs) {
		basic_string r(lhs.size() + rhs.size());

		tt::strcpy(r.data(), lhs.data());
		tt::strcpy(r.data() + lhs.size(), rhs.data());

		return r;
	}

	basic_string &operator+=(basic_string &lhs, const basic_string &rhs) {
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + rhs.size());

		tt::strcpy(lhs.data() + cur, rhs.data());

		return lhs;
	}

	basic_string operator+(const basic_string &lhs, const char *rhs) {
		uint32 r_size = tt::strlen(rhs);
		basic_string r(lhs.size() + r_size);

		tt::strcpy(r.data(), lhs.data());
		tt::strcpy(r.data() + lhs.size(), rhs);

		return r;
	}

	basic_string operator+(const char *lhs, const basic_string &rhs) {
		uint32 l_size = tt::strlen(lhs);
		basic_string r(l_size + rhs.size());

		tt::strcpy(r.data(), lhs);
		tt::strcpy(r.data() + l_size, rhs.data());

		return r;
	}

	basic_string &operator+=(basic_string &lhs, const char *rhs) {
		uint32 r_size = tt::strlen(rhs);
		uint32 cur = lhs.size();
		lhs.resize(lhs.size() + r_size);

		tt::strcpy(lhs.data() + cur, rhs);

		return lhs;
	}

	basic_string operator+(const basic_string &lhs, char rhs) {
		basic_string r(lhs.size() + 1);
		tt::strcpy(r.data(), lhs.data());
		r.back() = rhs;

		return r;
	}

	basic_string operator+(char lhs, const basic_string &rhs) {
		basic_string r(rhs.size() + 1);
		r[0] = lhs;
		tt::strcpy(r.data() + 1, rhs.data());

		return r;
	}

	basic_string &operator+=(basic_string &lhs, char rhs) {
		lhs.resize(lhs.size() + 1);
		lhs.back() = rhs;

		return lhs;
	}

	bool operator==(const basic_string &lhs, const basic_string &rhs) {
		return lhs.size() == rhs.size() && tt::strcmp(lhs.data(), rhs.data()) == 0;
	}

	bool operator!=(const basic_string &lhs, const basic_string &rhs) {
		return !(lhs == rhs);
	}

	bool operator<(const basic_string &lhs, const basic_string &rhs) {
		return tt::strcmp(lhs.data(), rhs.data()) < 0;
	}

	bool operator>(const basic_string &lhs, const basic_string &rhs) {
		return tt::strcmp(lhs.data(), rhs.data()) > 0;
	}

	bool operator<=(const basic_string &lhs, const basic_string &rhs) {
		return !(lhs > rhs);
	}

	bool operator>=(const basic_string &lhs, const basic_string &rhs) {
		return !(lhs < rhs);
	}

	bool operator==(const basic_string &lhs, const char *rhs) {
		return tt::strcmp(lhs.data(), rhs) == 0;
	}

	bool operator!=(const basic_string &lhs, const char *rhs) {
		return !(lhs == rhs);
	}

	bool operator<(const basic_string &lhs, const char *rhs) {
		return tt::strcmp(lhs.data(), rhs) < 0;
	}

	bool operator>(const basic_string &lhs, const char *rhs) {
		return tt::strcmp(lhs.data(), rhs) > 0;
	}

	bool operator<=(const basic_string &lhs, const char *rhs) {
		return !(lhs > rhs);
	}

	bool operator>=(const basic_string &lhs, const char *rhs) {
		return !(lhs < rhs);
	}

	bool operator==(const char *lhs, const basic_string &rhs) {
		return tt::strcmp(lhs, rhs.data()) == 0;
	}

	bool operator!=(const char *lhs, const basic_string &rhs) {
		return !(lhs == rhs);
	}

	bool operator<(const char *lhs, const basic_string &rhs) {
		return tt::strcmp(lhs, rhs.data()) < 0;
	}

	bool operator>(const char *lhs, const basic_string &rhs) {
		return tt::strcmp(lhs, rhs.data()) > 0;
	}

	bool operator<=(const char *lhs, const basic_string &rhs) {
		return !(lhs > rhs);
	}

	bool operator>=(const char *lhs, const basic_string &rhs) {
		return !(lhs < rhs);
	}
}    // namespace tt

std::ostream &operator<<(std::ostream &o, const tt::basic_string &str) {
	return o << str.c_str();
}

std::istream &operator>>(std::istream &i, tt::basic_string &str) {
	using istream = std::istream;
	using ctype = std::ctype<char>;
	using string = tt::basic_string;
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
