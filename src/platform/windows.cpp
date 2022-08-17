#include "windows.hpp"

#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>

namespace tt {
	void windows::init() {
	}

	void windows::shutdown() {
	}

	platform &get_platform() {
		static windows plat{};
		return plat;
	}

	int get_platform_data(SDL_Window *window, bgfx::PlatformData &pd) {
		// Collect information about the window from SDL
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
		if (!SDL_GetWindowWMInfo(window, &wmi)) {
			return 1;
		}

		// and give the pointer to the window to pd
		//pd.ndt = wmi.info.x11.display;
		pd.nwh = wmi.info.win.window;
		return 0;
	}
}    // namespace tt