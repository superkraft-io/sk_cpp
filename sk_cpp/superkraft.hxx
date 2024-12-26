#pragma once

#include "sk_common.hxx"

#include "sk_window_mngr/sk_window_mngr.hxx"
#include "sk_ipc/sk_ipc.hxx"

#include "sk_webview/sk_webview_resourceHandler/sk_webview_resourceHandler.hxx"

BEGIN_SK_NAMESPACE

class Superkraft {
public:
	SK_WebViewResourceHandler wvrh;
	SK_IPC ipc;
	SK_Window_Mngr wndMngr;
};

END_SK_NAMESPACE