#if !defined(PLATFORM_PLATFORM_HPP__)
#	define PLATFORM_PLATFORM_HPP__

namespace tt {
	struct platform {
		virtual void init() = 0;
		virtual void shutdown() = 0;
	};

	extern platform &get_platform();
}    // namespace tt

#endif    // PLATFORM_PLATFORM_HPP__
