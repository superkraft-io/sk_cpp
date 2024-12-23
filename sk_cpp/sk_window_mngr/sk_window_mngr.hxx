#pragma once

#include <string>
#include <functional>
#include <optional>

#include <unordered_map>

#if defined(_WIN32) || defined(_WIN64)
    #include "windows/sk_window_windows.h"
#elif defined(__APPLE__)
    #if TARGET_OS_IPHONE
        #include "ios/sk_window_ios.h"
    #else
        #include "macos/sk_window_macos.h"
    #endif
#elif defined(__linux__)
    #include "linux/sk_window_linux.h"
#elif defined(__ANDROID__)
    #include "android/sk_window_android.h"
#else
    #error "Unsupported platform"
#endif

class SK_Window_Mngr {
public:
	unsigned int wndIdx = 0;
	std::unordered_map<std::string, SK_Window*> list;

	
	using SK_Window_Create_Callback = std::optional<std::function<void(SK_Window*)>>;

	SK_Window* newWindow(SK_Window_Create_Callback cb = nullptr) {
		wndIdx++;

		SK_Window* wnd = new SK_Window();

		wnd->initialize(wndIdx);

		list["window"] = wnd;

		if (cb != nullptr) (*cb)(wnd);

		return wnd;
	};

private:

};
