#include "sem_ver.hpp"

std::ostream &operator<<(std::ostream &os, tt::sem_ver ver) {
	return os << ver.major << "." << ver.minor << "." << ver.patch;
}
