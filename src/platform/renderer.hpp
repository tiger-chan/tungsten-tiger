#if !defined(PLATFORM_RENDERER_HPP__)
#	define PLATFORM_RENDERER_HPP__

namespace tt {
	struct renderer_manager {
		virtual int init() = 0;
		virtual void shutdown() = 0;

		virtual struct material *get_materials() const {
			return nullptr;
		}
	};

	extern renderer_manager &get_renderer();
}    // namespace tt

#endif    // PLATFORM_RENDERER_HPP__
