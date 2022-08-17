#if !defined(CORE_RENDERER_VERTEX_LAYOUT_HPP__)
#	define CORE_RENDERER_VERTEX_LAYOUT_HPP__

#	include <system_types.hpp>

namespace tt {
	enum vertex_attr {
		VA_POSITION,
		VA_NORMAL,
		VA_TANGENT,
		VA_BITANGENT,
		VA_COLOR_0,
		VA_COLOR_1,
		VA_COLOR_2,
		VA_COLOR_3,
		VA_INDICES,
		VA_WEIGHT,
		VA_TEX_COORD_0,
		VA_TEX_COORD_1,
		VA_TEX_COORD_2,
		VA_TEX_COORD_3,
		VA_TEX_COORD_4,
		VA_TEX_COORD_5,
		VA_TEX_COORD_6,
		VA_TEX_COORD_7,

		VA_COUNT,
		VA_UNSET,
	};

	enum vertex_attr_type {
		// uint8
		VAT_UINT8,
		// uint10  (not always available)
		VAT_UINT10,
		// int16
		VAT_INT16,
		// half (not always available)
		VAT_HALF,
		// float
		VAT_FLOAT,
	};

	struct vertex_layout {
		vertex_layout();

		struct detail_t {
			uint16 attr : 5;
			uint16 type : 3;
			uint16 quantity : 3;
			uint16 normalized : 1;
			uint16 as_int : 1;
		};

		uint8 attrs[VA_COUNT];
		detail_t details[VA_COUNT];
		int8 size;
	};

	extern void layout_begin(vertex_layout &layout);
	extern void layout_end(vertex_layout &layout);

	/**
	 * @brief Add attribute to vertex layout.
	 * 
	 * @param layout 
	 * @param attr
	 * @param type uint8, float, etc...
	 * @param quanity the number of elements in the attribute [1-4]
	 * @param normalized When using fixed point AttribType (f.e. Uint8)
	 * value will be normalized for vertex shader usage. When normalized
	 * is set to true
	 * @param as_int indicates if the shader will handle unpacking the int
	 * 
	 * @remarks Must be called between begin/end.
	 */
	extern void layout_add(vertex_layout &layout, vertex_attr attr, vertex_attr_type type, uint8 quantity, bool normalized = false, bool as_int = false);
}    // namespace tt

#endif    // CORE_RENDERER_VERTEX_LAYOUT_HPP__
