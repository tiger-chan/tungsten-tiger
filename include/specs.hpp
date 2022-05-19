
#include <ostream>

namespace tt {
	enum class architecture {
		x86,
		x86_64,
		x64,
	};

	enum class compiler {
		clang,
		msvc,
		gcc,
		apple,
		intel,
	};

	enum class cpp_standard {
		cxx17,
		cxx20,
	};

	struct sem_ver {
		unsigned long major = 0;
		unsigned long minor = 0;
		unsigned long patch = 0;
	};

	struct compiler_spec {
		bool debug;
		cpp_standard cxx;
		architecture arch;
		compiler compiler;
		sem_ver ver;
	};

	namespace {
		static constexpr compiler_spec create_compiler_spec() {
			compiler_spec spec{};
#ifdef NDEBUG
			spec.debug = false;
#else
			spec.debug = true;
#endif

// ARCHITECTURES
#ifdef _M_X64
			spec.arch = architecture::x64;
#elif defined(_M_IX86) || __1386__
			spec.arch = architecture::x86;
#elif __x86_64__
			spec.arch = architecture::x86_64;
#endif

#if _MSVC_LANG
			constexpr long long ver = _MSVC_LANG;
#elif __cplusplus
			constexpr long long ver = __cplusplus;
#endif

			if constexpr (ver < 202002L) {
				spec.cxx = cpp_standard::cxx17;
			}
			else {
				spec.cxx = cpp_standard::cxx20;
			}

// COMPILER VERSIONS
#if _MSC_VER
			spec.compiler = compiler::msvc;
			spec.ver.major = _MSC_VER / 100;
			spec.ver.minor = _MSC_VER % 100;
			spec.ver.patch = 0;
#elif __GNUC__
			spec.compiler = compiler::gcc;
			spec.ver.major = __GNUC__;
			spec.ver.minor = __GNUC_MINOR__;
			spec.ver.patch = __GNUC_PATCHLEVEL__;
#elif __clang_major__
			spec.compiler = compiler::clang;
			spec.ver.major = __clang_major__;
			spec.ver.minor = __clang_minor__;
			spec.ver.patch = __clang_patchlevel__;
#endif

#if __INTEL_COMPILER
			spec.compiler = compiler::intel;
#elif __apple_build_version__
			spec.compiler = compiler::apple;
#endif

			return spec;
		}
	}    // namespace

	static constexpr compiler_spec spec = tt::create_compiler_spec();
}    // namespace tt

static std::ostream& operator<<(std::ostream& os, tt::architecture arch) {
	switch (arch) {
		case tt::architecture::x86: return os << "x86";
		case tt::architecture::x86_64: return os << "x86_64";
		case tt::architecture::x64: return os << "x64";
	}
	return os;
}

static std::ostream& operator<<(std::ostream& os, tt::compiler c) {
	switch (c) {
		case tt::compiler::apple: return os << "apple";
		case tt::compiler::clang: return os << "clang";
		case tt::compiler::msvc: return os << "msvc";
		case tt::compiler::gcc: return os << "gcc";
		case tt::compiler::intel: return os << "intel";
	}
	return os;
}

static std::ostream& operator<<(std::ostream& os, tt::cpp_standard c) {
	switch (c) {
		case tt::cpp_standard::cxx17: return os << "c++17";
		case tt::cpp_standard::cxx20: return os << "c++20";
	}
	return os;
}

static std::ostream& operator<<(std::ostream& os, tt::sem_ver ver) {
	return os << ver.major << "." << ver.minor << "." << ver.patch;
}
