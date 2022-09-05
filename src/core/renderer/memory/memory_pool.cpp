#include "memory_pool.hpp"

namespace tt {
	namespace gl {
		namespace memory {
			namespace {
				static pool_data create(uint32 id, int32 size, memory_usage usage) {
					pool_data data{};

					data.id = id;
					data.size = size;
					data.usage = usage;

					return data;
				}

				static pool_data create(uint32 id, int32 size, index_buffer_flags flags) {
					pool_data data{};

					data.id = id;
					data.size = size;
					data.usage = MU_IDX_BUFFER;
					data.idx_flags = flags;

					return data;
				}

				static pool_data create(uint32 id, int32 size, const vertex_layout *layout, vertex_buffer_flags flags) {
					pool_data data{};

					data.id = id;
					data.size = size;
					data.usage = MU_VERT_BUFFER;
					data.layout = *layout;
					data.layout_flags = flags;

					return data;
				}
			}    // namespace

			memory_pool::memory_pool(uint32 id, int32 size, index_buffer_flags flags)
				: data{ create(id, size, flags) } {
			}

			memory_pool::memory_pool(uint32 id, int32 size, const vertex_layout *layout, vertex_buffer_flags flags)
				: data{ create(id, size, layout, flags) } {
			}

			memory_pool::~memory_pool() {
				shutdown();
			}

			int32 memory_pool::init() {
				// Should a type be associated?
				// I.e. should it require matching usage for pools
				// so that types are aligned rather than allocated for random things?

				// TODO: allocate the memory needed for this pool which will then be used for everything.
				switch (data.usage) {
					case MU_VERT_BUFFER: {
						data.bytes = new byte[data.size * data.layout.stride];
						data.vert_id = get_renderer().create_dyn_vertex_buffer(data.bytes, data.size, data.layout, data.layout_flags);

						break;
					}

					case MU_IDX_BUFFER: {
						auto stride = (data.idx_flags & IB_INDEX_32) == IB_INDEX_32 ? 4 : 2;
						data.bytes = new byte[data.size * stride];
						data.idx_id = get_renderer().create_dyn_index_buffer(data.bytes, data.size, data.idx_flags);

						break;
					}
				}

				data.head = new pool_block{
					data.next_block_id++,    // id
					data.size,    // size
					0,    // offset
					nullptr,    // previous block
					nullptr,    // next block
					true    // free for use
				};

				return 0;
			}

			void memory_pool::shutdown() {
				delete[] data.bytes;
				data.bytes = nullptr;

				switch(data.usage) {
					case MU_IDX_BUFFER: {
						get_renderer().destroy(data.idx_id);
						break;
					}
					case MU_VERT_BUFFER: {
						get_renderer().destroy(data.vert_id);
						break;
					}
				}

				pool_block *prev = nullptr;
				pool_block *cur = data.head;
				while (cur) {
					if (!cur->next) {
						delete cur;
						cur = nullptr;
					}
					else {
						prev = cur;
						cur = cur->next;
						delete prev;
					}
				}

				data.head = nullptr;
			}

			bool memory_pool::allocate(int32 count, allocation *alloc) {
				int32 free_size{ data.size - data.allocated };

				int32 size = 0;
				int32 alignment = 16;
				int32 stride = 16;
				switch (data.usage) {
					case MU_IDX_BUFFER: {
						alignment = ((data.idx_flags & IB_INDEX_32) == IB_INDEX_32) ? 32 : 16;
						stride = alignment / sizeof(byte);
						size = count * stride;
						break;
					}
					case MU_VERT_BUFFER: {
						stride = data.layout.stride;
						size = count * stride;
						break;
					}
				}

				if (free_size < size) {
					return false;
				}

				pool_block *cur{ nullptr };
				pool_block *best{ nullptr };

				int32 padding{ 0 };
				int32 offset{ 0 };
				int32 aligned{ 0 };
				for (cur = data.head; cur; cur = cur->next) {
					if (!cur->free) {
						// current block isn't free, so we skip it.
						continue;
					}

					if (cur->size < size) {
						// Current block doesn't have enough space
						continue;
					}

					offset = cur->offset;

					padding = offset - cur->offset;
					aligned = padding + size;

					if (cur->size < aligned) {
						// current block isn't large enough after adjusting for alignment, so we skip it.
						continue;
					}

					if (data.size < aligned + data.allocated) {
						// current request is too large for the allocated space after alignment, drop out of this pool.
						return false;
					}

					best = cur;
					break;
				}

				if (!best) {
					// There was no good fit with this pool
					return false;
				}

				if (best->size > size) {
					// We have too much space and need to split this block in to a new block with the excess
					pool_block *new_block = new pool_block;
					pool_block *next = best->next;
					best->next = new_block;
					new_block->next = next;
					if (next) {
						next->prev = new_block;
					}

					new_block->id = data.next_block_id++;
					new_block->size = best->size - aligned;
					new_block->offset = offset + size;
					new_block->free = true;
				}

				best->free = false;
				best->size = size;
				data.allocated += aligned;

				alloc->id = best->id;
				alloc->data = data.bytes + offset;
				alloc->offset = offset;
				alloc->pool = this;
				alloc->size = size;

				return true;
			}

			void memory_pool::deallocate(const allocation *alloc) {
				pool_block *cur = data.head;
				for (; cur; cur = cur->next) {
					if (cur->id == alloc->id) {
						break;
					}
				}

				if (!cur) {
					// Attempted to deallocate an unknown allocation.
					return;
				}

				cur->free = true;
				auto allocated_size = cur->size;

				auto block = cur->prev;
				if (block && block->free) {
					// collapse previous block and this one.
					block->next = cur->next;
					if (block->next) {
						block->next->prev = block;
					}

					block->size += cur->size;
					delete cur;
					cur = block;
				}

				block = cur->next;
				if (block && block->free) {
					if (block->next) {
						block->next->prev = cur;
					}

					cur->next = block->next;
					cur->size += block->size;

					delete block;
				}

				data.allocated -= allocated_size;
			}

			bool memory_pool::empty() const {
				return data.allocated == 0;
			}
		}    // namespace memory
	}    // namespace gl
}    // namespace tt