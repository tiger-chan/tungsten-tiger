#if !defined(CORE_RENDERER_MEMORY_MEMORY_POOL_HPP__)
#	define CORE_RENDERER_MEMORY_MEMORY_POOL_HPP__

#	include <system_types.hpp>
#	include "allocation.hpp"
#	include <platform/renderer.hpp>
#	include <core/renderer/vertex_layout.hpp>

namespace tt {
	namespace gl {
		namespace memory {
			enum memory_usage {
				MU_VERT_BUFFER,
				MU_IDX_BUFFER,

				MU_COUNT
			};

			struct pool_block {
				uint32 id;

				int32 size;
				int32 offset;

				pool_block *prev{ nullptr };
				pool_block *next{ nullptr };
				bool free;
			};

			struct pool_data {
				uint32 id{ 0 };
				uint32 next_block_id{ 0 };
				memory_usage usage;
				union {
					dyn_vertex_buffer_handle vert_id{};
					dyn_index_buffer_handle idx_id;
				};

				union {
					vertex_buffer_flags layout_flags{ VB_NONE };
					index_buffer_flags idx_flags;
				};

				// Only used in vertex types
				vertex_layout layout;

				int32 size{ 0 };
				int32 allocated{ 0 };

				byte *bytes{ nullptr };

				pool_block *head{ nullptr };
			};

			struct memory_pool {
				memory_pool(uint32 id, int32 size, index_buffer_flags flags);
				memory_pool(uint32 id, int32 size, const vertex_layout *layout, vertex_buffer_flags flags);
				~memory_pool();

				int32 init();
				void shutdown();

				bool allocate(int32 count, allocation *alloc);
				void deallocate(const allocation *alloc);

				bool empty() const;

				memory::pool_data data;
			};

		}    // namespace memory
	}    // namespace gl
}    // namespace tt

#endif    // CORE_RENDERER_MEMORY_MEMORY_POOL_HPP__
