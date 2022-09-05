#if !defined(CORE_RENDERER_MEMORY_ALLOCATION_HPP__)
#	define CORE_RENDERER_MEMORY_ALLOCATION_HPP__

#	include <system_types.hpp>

namespace tt {
	namespace gl {
		namespace memory {
			struct memory_pool;

			struct allocation {
				int32 id;
				memory_pool *pool;
				byte *data;
				int32 size;
				int32 offset;
			};
		}    // namespace memory
	}    // namespace gl
}    // namespace tt

#endif    // CORE_RENDERER_MEMORY_ALLOCATION_HPP__
