#if !defined(CORE_RENDERER_VERTEX_BUFFER_FLAGS_HPP__)
#	define CORE_RENDERER_VERTEX_BUFFER_FLAGS_HPP__

#	include <system_types.hpp>

namespace tt {
	enum vertex_buffer_flags {
		VB_NONE = 0,
		// 1 8-bit value
		VB_BUFFER_COMPUTE_FORMAT_8X1 = 0x0001,
		// 2 8-bit values
		VB_BUFFER_COMPUTE_FORMAT_8X2 = 0x0002,
		// 4 8-bit values
		VB_BUFFER_COMPUTE_FORMAT_8X4 = 0x0003,
		// 1 16-bit value
		VB_BUFFER_COMPUTE_FORMAT_16X1 = 0x0004,
		// 2 16-bit values
		VB_BUFFER_COMPUTE_FORMAT_16X2 = 0x0005,
		// 4 16-bit values
		VB_BUFFER_COMPUTE_FORMAT_16X4 = 0x0006,
		// 1 32-bit value
		VB_BUFFER_COMPUTE_FORMAT_32X1 = 0x0007,
		// 2 32-bit values
		VB_BUFFER_COMPUTE_FORMAT_32X2 = 0x0008,
		// 4 32-bit values
		VB_BUFFER_COMPUTE_FORMAT_32X4 = 0x0009,

		// Type `int`.
		VB_BUFFER_COMPUTE_TYPE_INT = 0x0010,
		// Type `uint`.
		VB_BUFFER_COMPUTE_TYPE_UINT = 0x0020,
		// Type `float`.
		VB_BUFFER_COMPUTE_TYPE_FLOAT = 0x0030,

		VB_BUFFER_NONE = 0x0000,
		// Buffer will be read by shader.
		VB_BUFFER_COMPUTE_READ = 0x0100,
		// Buffer will be used for writing.
		VB_BUFFER_COMPUTE_WRITE = 0x0200,
		// Buffer will be used for storing draw indirect commands.
		VB_BUFFER_DRAW_INDIRECT = 0x0400,
		// Allow dynamic index/vertex buffer resize during update.
		VB_BUFFER_ALLOW_RESIZE = 0x0800,
		// Index buffer contains 32-bit indices.
		VB_BUFFER_INDEX_32 = 0x1000,

		VB_BUFFER_COMPUTE_READ_WRITE = VB_BUFFER_COMPUTE_READ | VB_BUFFER_COMPUTE_WRITE
	};
}

#endif    // CORE_RENDERER_VERTEX_BUFFER_FLAGS_HPP__