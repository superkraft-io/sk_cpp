
#include <iostream>
#include <wil/com.h>
#include <wrl.h>
#include <WebView2.h>
#include "WebView2EnvironmentOptions.h"

class SK_WebView {
public:
	HWND* parentHwnd;

	wil::com_ptr<ICoreWebView2Settings> settings;
	wil::com_ptr<ICoreWebView2Controller> controller = nullptr;
	wil::com_ptr<ICoreWebView2> webview = nullptr;

	void create();
	void update();

	std::string currentURL = "";
	void gotoURL(std::string url);
private:

};