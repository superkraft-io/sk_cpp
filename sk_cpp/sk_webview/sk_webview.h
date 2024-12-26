#pragma once

#if SK_OS == windows
#include "windows/sk_webview_windows.h"
#elif SK_OS == macos
#include "macos/sk_webview_macos.h"
#elif SK_OS == ios
#include "ios/sk_webview_ios.h"
#elif SK_OS == linux
#include "linux/sk_webview_linux.h"
#elif SK_OS == android
#include "android/sk_webview_android.h"
#endif
