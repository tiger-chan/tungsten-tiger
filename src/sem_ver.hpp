#if !defined(SEM_VER_HPP__)
#define SEM_VER_HPP__

#include <ostream>

#include "system_types.hpp"

namespace tt {
	struct sem_ver {
		uint32 major = 0;
		uint32 minor = 0;
		uint32 patch = 0;
	};
}

std::ostream& operator<<(std::ostream& os, tt::sem_ver ver);

#endif // SEM_VER_HPP__

