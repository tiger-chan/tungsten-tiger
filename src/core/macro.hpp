#if !defined(CORE_MACRO_HPP__)
#	define CORE_MACRO_HPP__

#	ifndef UNUSED
#		define UNUSED(x) (void)(x)
#	endif    // UNUSED

#	ifndef NOT_YET_IMPLEMENTED
#		define NOT_YET_IMPLEMENTED(X) (void)(X)
#	endif    // NOT_YET_IMPLEMENTED

#	ifndef NUM_ARGS
#		define NUM_ARGS(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value
#	endif

// https://stackoverflow.com/a/4840428/3261668
#	define ALIGN_UP(x, a) (((x) + (a - 1)) & ~(a - 1))
#	define ALIGN_DOWN(x, a) ((x) & ~(a - 1))

#endif    // CORE_MACRO_HPP__
