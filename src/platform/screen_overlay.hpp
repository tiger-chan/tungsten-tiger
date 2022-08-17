#if !defined(CORE_PLATFORM_SCREEN_OVERLAY_HPP__)
#	define CORE_PLATFORM_SCREEN_OVERLAY_HPP__

namespace tt {
	struct screen_overlay {
		virtual void init() = 0;
		virtual void shutdown() = 0;
		virtual void update() = 0;
		virtual void frame() = 0;

		virtual void load_url(const char* url) = 0;
		virtual void load_html(const char* html) = 0;
	};

	extern screen_overlay &overlay();
}

#endif    // CORE_PLATFORM_SCREEN_OVERLAY_HPP__
