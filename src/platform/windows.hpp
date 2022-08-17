#if !defined(PLATFORM_WINDOWS_HPP__)
#	define PLATFORM_WINDOWS_HPP__

#	include "platform.hpp"

namespace tt {
	struct windows : public platform {
		void init() override;
		void shutdown() override;
	};
}    // namespace tt

#endif    // PLATFORM_WINDOWS_HPP__
