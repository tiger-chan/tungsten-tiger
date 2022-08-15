#include "vertex_layout.hpp"

namespace {
	template<uint32 Size>
	void set_all_details(tt::vertex_layout::detail_t (&details)[Size], tt::vertex_layout::detail_t val) {
		for (uint32 i = 0; i < Size; ++i) {
			details[i] = val;
		}
	}

	template<uint32 Size>
	void set_all_attrs(uint8 (&attrs)[Size], tt::vertex_attr val) {
		for (uint32 i = 0; i < Size; ++i) {
			attrs[i] = static_cast<uint8>(val);
		}
	}
}    // namespace

namespace tt {
	vertex_layout::vertex_layout()
		: details{}
		, attrs{}
		, size{ 0 } {
	}

	void layout_begin(vertex_layout &layout) {
		::set_all_details(layout.details, tt::vertex_layout::detail_t{});
		::set_all_attrs(layout.attrs, VA_UNSET);
		layout.size = 0;
	}

	void layout_end(vertex_layout &layout) {
	}

	void layout_add(vertex_layout &layout, vertex_attr attr, vertex_attr_type type, uint8 quantity, bool normalized, bool as_int) {
		uint8 i = static_cast<uint8>(attr);

		if (layout.attrs[i] != static_cast<uint8>(VA_UNSET)) {
			// Maybe this should be an error?
			return;
		}

		layout.attrs[i] = layout.size;
		auto &detail = layout.details[layout.size++];
		detail.attr = i;
		detail.type = type;
		detail.quantity = quantity;
		detail.normalized = normalized;
		detail.as_int = as_int;
	}
}    // namespace tt