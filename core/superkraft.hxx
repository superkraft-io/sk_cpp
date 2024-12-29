#pragma once

#include "sk_common.hxx"

#include "utils/sk_machine.h"


#include "sk_window_mngr/sk_window_mngr.hxx"
#include "sk_ipc/sk_ipc.hxx"

#include "sk_webview/sk_webview_resourceHandler/sk_webview_resourceHandler.hxx"

BEGIN_SK_NAMESPACE

class Superkraft {
public:
	SK_Machine machine;
	SK_WebViewResourceHandler* wvrh;
	SK_IPC ipc;
	SK_Window_Mngr wndMngr;

	Superkraft::Superkraft() {
		SK_Path_Utils::init();
		SK_Machine::init();
		wvrh = new SK_WebViewResourceHandler();
	}

	Superkraft::~Superkraft() {
		delete wvrh;
	}
};

END_SK_NAMESPACE
