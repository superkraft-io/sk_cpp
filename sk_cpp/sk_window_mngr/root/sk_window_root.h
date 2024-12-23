#include <windows.h>
#include <iostream>
#include <stdio.h>
#pragma once

#include <string>

#include "../../sk_webview/sk_webview.h"

class SK_Window_Root {
public:
	unsigned int wndIdx;

	std::string title = "SK Window";

	bool visible = false;

	int width = 800;
	int height = 600;
	int left = 0;
	int top = 0;


	SK_WebView webview;

	virtual void initialize(unsigned int _wndIdx);
private:

};