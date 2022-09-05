#if !defined(CORE_RENDERER_MEMORY_ALLOCATOR_HPP__)
#	define CORE_RENDERER_MEMORY_ALLOCATOR_HPP__

#	include <system_types.hpp>
#	include "allocation.hpp"
#	include "memory_pool.hpp"
#	include <array>
#	include <vector>

#	ifndef FRAME_BUFFER_COUNT
#		define FRAME_BUFFER_COUNT 2
#	endif

#	ifndef INDEX_MEMORY_MB
#		define INDEX_MEMORY_MB 32
#	endif

#	ifndef VERTEX_MEMORY_MB
#		define VERTEX_MEMORY_MB 64
#	endif

namespace tt {
	// Index Buffer allocation
	gl::memory::allocation ialloc(int32 count, index_buffer_flags flags = IB_NONE);
	// Vertex Buffer allocation
	gl::memory::allocation valloc(int32 count, const vertex_layout *layout, vertex_buffer_flags flags = VB_NONE);
	void dealloc(const gl::memory::allocation *alloc);

	byte *map(gl::memory::allocation alloc);
	void unmap(gl::memory::allocation alloc);

	namespace gl {

		namespace memory {
			static constexpr const uint32 frame_buffer_count = FRAME_BUFFER_COUNT;

			struct allocator_data {
				uint32 next_pool{ 0 };
				uint32 garbage_id{ 0 };

				int32 index_memory_bytes;
				int32 vertex_memory_bytes;

				std::array<std::vector<memory_pool *>, MU_COUNT> pools;
				std::array<std::vector<allocation>, frame_buffer_count> garbage{};
			};

			struct allocator {
				allocator();
				~allocator();

				int32 init(int32 ibuffer_mb = INDEX_MEMORY_MB, int32 vbuffer_mb = VERTEX_MEMORY_MB);
				void shutdown();
				void cleanup();

				memory::allocator_data data;
			};
		}    // namespace memory

		memory::allocator& allocator();
	}    // namespace gl
}    // namespace tt

#endif    // CORE_RENDERER_MEMORY_ALLOCATOR_HPP__
