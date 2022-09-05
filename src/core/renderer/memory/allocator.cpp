#include "allocator.hpp"

namespace tt {
	namespace gl {
		memory::allocator& allocator() {
			static memory::allocator alloc{};
			return alloc;
		}

		namespace {
			int cmp(const vertex_layout *lhs, const vertex_layout *rhs) {
				if (lhs->size != rhs->size) {
					return lhs->size < rhs->size;
				}
				if (lhs->stride != rhs->stride) {
					return lhs->stride < rhs->stride;
				}
				return memcmp(lhs->details, rhs->details, sizeof(lhs->details));
			}

			int32 block_size(const memory::allocator_data &data, memory::memory_usage usage) {
				switch (usage) {
					case memory::MU_IDX_BUFFER: return data.index_memory_bytes;
					case memory::MU_VERT_BUFFER: return data.vertex_memory_bytes;
					default: return 0;
				}
			}

			bool alloc_from_pool(memory::allocator_data &data, int32 count, index_buffer_flags flags, memory::allocation *alloc) {
				auto &&pools = data.pools[memory::MU_IDX_BUFFER];
				for (auto &&pool : pools) {
					if (pool->data.layout_flags != flags) {
						continue;
					}

					if (pool->allocate(count, alloc)) {
						return true;
					}
				}
				return false;
			}

			bool alloc_from_pool(memory::allocator_data &data, int32 count, const vertex_layout *layout, vertex_buffer_flags flags, memory::allocation *alloc) {
				auto &&pools = data.pools[memory::MU_VERT_BUFFER];
				for (auto &&pool : pools) {
					if (!(cmp(&pool->data.layout, layout) == 0 && pool->data.layout_flags == flags)) {
						continue;
					}

					if (pool->allocate(count, alloc)) {
						return true;
					}
				}
				return false;
			}

			void alloc_from_new_pool(memory::allocator_data &data, int32 count, index_buffer_flags flags, memory::allocation *alloc) {
				auto stride = (flags & IB_INDEX_32) == IB_INDEX_32 ? 4 : 2;
				auto pool_size = block_size(data, memory::MU_IDX_BUFFER) / stride;

				auto pool = new memory::memory_pool{
					data.next_pool++,
					pool_size,
					flags
				};

				if (pool->init() == 0) {
					data.pools[memory::MU_IDX_BUFFER].emplace_back(pool);
				}
				else {
					// Could not allocate more memory throw error?
				}

				pool->allocate(count, alloc);
			}

			void alloc_from_new_pool(memory::allocator_data &data, int32 count, const vertex_layout *layout, vertex_buffer_flags flags, memory::allocation *alloc) {
				auto pool_size = block_size(data, memory::MU_VERT_BUFFER) / layout->stride;

				auto pool = new memory::memory_pool{
					data.next_pool++,
					pool_size,
					layout,
					flags,
				};

				if (pool->init() == 0) {
					data.pools[memory::MU_VERT_BUFFER].emplace_back(pool);
				}
				else {
					// Could not allocate more memory throw error?
				}

				pool->allocate(count, alloc);
			}
		}    // namespace

		namespace memory {
			allocator::allocator() = default;
			allocator::~allocator() = default;

			int32 allocator::init(int32 ibuffer_mb, int32 vbuffer_mb) {
				data.vertex_memory_bytes = vbuffer_mb * 1024 * 1024;
				data.index_memory_bytes = ibuffer_mb * 1024 * 1024;
				return 0;
			}

			void allocator::shutdown() {
				cleanup();
				for (auto &&pools : data.pools) {
					for (size_t i = 0; i < pools.size(); ++i) {
						delete pools[i];
					}

					pools.clear();
				}
			}

			void allocator::cleanup() {
				auto last_id = data.garbage_id;
				data.garbage_id = (data.garbage_id + 1) % frame_buffer_count;

				auto &&garbage = data.garbage[last_id];

				for (size_t i = 0; i < garbage.size(); ++i) {
					auto &&alloc = garbage[i];
					alloc.pool->deallocate(&alloc);

					if (alloc.pool->empty()) {
						auto pool = alloc.pool;
						auto &pools = data.pools[pool->data.usage];
						pools.erase(std::find_if(std::begin(pools), std::end(pools), [pool](const auto &item) { return item == pool; }));
						delete pool;
					}
				}
				garbage.clear();
			}
		}    // namespace memory
	}    // namespace gl

	gl::memory::allocation ialloc(int32 count, index_buffer_flags flags) {
		gl::memory::allocation allocation;
		if (gl::alloc_from_pool(gl::allocator().data, count, flags, &allocation)) {
			return allocation;
		}

		gl::alloc_from_new_pool(gl::allocator().data, count, flags, &allocation);

		return allocation;
	}

	gl::memory::allocation valloc(int32 count, const vertex_layout *layout, vertex_buffer_flags flags) {
		gl::memory::allocation allocation;
		if (gl::alloc_from_pool(gl::allocator().data, count, layout, flags, &allocation)) {
			return allocation;
		}

		gl::alloc_from_new_pool(gl::allocator().data, count, layout, flags, &allocation);

		return allocation;
	}

	void dealloc(const gl::memory::allocation *alloc) {
		if (!alloc->pool) {
			return;
		}

		auto &&data = gl::allocator().data;

		data.garbage[data.garbage_id].emplace_back(*alloc);
	}

	byte *map(gl::memory::allocation alloc) {
		return alloc.pool ? alloc.data : nullptr;
	}

	void unmap(gl::memory::allocation alloc) {
		if (!alloc.pool) {
			return;
		}

		switch(alloc.pool->data.usage) {
			case gl::memory::MU_IDX_BUFFER: {
				auto id = alloc.pool->data.idx_id;
				get_renderer().update(id, alloc.data, alloc.size, alloc.offset);
				break;
			}
			case gl::memory::MU_VERT_BUFFER: {
				auto id = alloc.pool->data.vert_id;
				get_renderer().update(id, alloc.data, alloc.size, alloc.offset);
				break;
			}
		}
	}
}    // namespace tt