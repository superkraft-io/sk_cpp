#pragma once

#include "json.hpp"

#include "sk_common.hxx"

#include "utils/sk_machine.h"


#include "sk_window_mngr/sk_window_mngr.hxx"
#include "sk_ipc/sk_ipc.hxx"

#include "sk_webview_initializer/sk_webview_initializer.hxx"
#include "sk_webview_resourceHandler/sk_webview_resourceHandler.hxx"
//#include "../module_system/sk_module_system.hxx"

BEGIN_SK_NAMESPACE


class Superkraft {
public:

	SK_Machine machine;
	SK_WebView_Initializer wvinit;
	SK_WebViewResourceHandler* wvrh;
	SK_IPC ipc;
	SK_Window_Mngr wndMngr;
	//SK_Module_System modsys;

	Superkraft::Superkraft() {
		SK_Path_Utils::init();

		SK_File configFile;
		configFile.loadFromDisk(SK_Path_Utils::paths["config"]);
		sk_config = nlohmann::json::parse(std::string(configFile));

		SK_Machine::init();
		wvrh = new SK_WebViewResourceHandler();
		//wvrh->modsys = &modsys;
	}

	Superkraft::~Superkraft() {
		delete wvrh;
	}
};

END_SK_NAMESPACE
