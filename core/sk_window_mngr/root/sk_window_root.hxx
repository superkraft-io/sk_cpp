#pragma once

#include <optional>
#include <string>

#include "../../sk_webview/sk_webview.h"

#include "../../sk_common.hxx"

BEGIN_SK_NAMESPACE

class SK_Window_Root {
public:
	unsigned int wndIdx;

	SK_String title = "SK Window";

	bool visible = false;

	int width = 800;
	int height = 600;
	int left = 0;
	int top = 0;
    
    std::optional<int> zIndex = NULL;
    bool transparent = false;
    


	SK_WebView webview;

	virtual void initialize(unsigned int _wndIdx) {
        wndIdx = _wndIdx;
    }
private:

};

END_SK_NAMESPACE