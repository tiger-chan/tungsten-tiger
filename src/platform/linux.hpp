#if !defined(PLATFORM_LINUX_HPP__)
#	define PLATFORM_LINUX_HPP__

#	include "platform.hpp"

namespace tt {
	struct linux : public platform {
		void init() override;
		void shutdown() override;
	};
}    // namespace tt

#endif    // PLATFORM_LINUX_HPP__
