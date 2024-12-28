#pragma once

#include <iostream>
#include <wil/com.h>
#include <wrl.h>
#include <WebView2.h>
#include "WebView2EnvironmentOptions.h"


#include "../../sk_common.hxx"
BEGIN_SK_NAMESPACE

class SK_WebView {
public:
	HWND* parentHwnd;

	wil::com_ptr<ICoreWebView2Settings> settings;
	wil::com_ptr<ICoreWebView2Controller> controller = nullptr;
	wil::com_ptr<ICoreWebView2> webview = nullptr;       

	void create();
	void update();

	SK_String currentURL = "";
	void navigate(const SK_String& url);
private:

};

END_SK_NAMESPACE