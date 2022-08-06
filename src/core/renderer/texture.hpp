#if !defined(CORE_RENDERER_TEXTURE_HPP__)
#	define CORE_RENDERER_TEXTURE_HPP__

#	include <system_types.hpp>
#	include "components.hpp"

namespace tt {
	enum texture_format {
		a8 /* Alpha 8 bits */,
		r8 /* Red 8 bits */,
		rgba /* Red Green Blue 8 bits each */,
	};

	struct texture {
		texture_format format{ texture_format::rgba };
		uint16 height{ 0 };
		uint16 width{ 0 };
		byte *data{ nullptr };
		uint32 data_len{ 0 };
	};
}    // namespace tt

#endif    // CORE_RENDERER_TEXTURE_HPP__
