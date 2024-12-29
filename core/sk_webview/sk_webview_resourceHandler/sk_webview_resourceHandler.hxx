#if SK_OS == windows
#include "sk_webview_resourceHandler_windows.hxx"
#elif SK_OS == macos
#include "sk_webview_resourceHandler_macos.hxx"
#elif SK_OS == ios
#include "sk_webview_resourceHandler_ios.hxx"
#elif SK_OS == linux
#include "sk_webview_resourceHandler_linux.hxx"
#elif SK_OS == android
#include "sk_webview_resourceHandler_android.hxx"
#endif