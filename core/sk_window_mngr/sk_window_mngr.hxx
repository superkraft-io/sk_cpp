#pragma once

#include <string>
#include <functional>
#include <optional>

#include <unordered_map>

#include "../sk_common.hxx"


#if SK_OS == windows
    #include "windows/sk_window_windows.h"
#elif SK_OS == macos
	#include "macos/sk_window_macos.h"
#elif SK_OS == ios
    #include "ios/sk_window_ios.h"
#elif SK_OS == linux
    #include "linux/sk_window_linux.h"
#elif SK_OS == android
    #include "android/sk_window_android.h"
#endif


BEGIN_SK_NAMESPACE

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


END_SK_NAMESPACE