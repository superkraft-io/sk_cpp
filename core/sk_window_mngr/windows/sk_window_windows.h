#pragma once

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "../../sk_common.hxx"

#ifdef SK_FRAMEWORK = iPlug2
	#include "../../sk_window_mngr/root/sk_window_root.hxx"
#else
	#include "../root/sk_window_root.h"
#endif


BEGIN_SK_NAMESPACE

class SK_Window : public SK_Window_Root {
public:
	~SK_Window();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	SK_String windowClassName = "SK_Window";
	HWND hwnd;
	WNDCLASS wc;

	void initialize(unsigned int _wndIdx) override;
	void create();
	int applyTo(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	void createWebView();
private:

};

END_SK_NAMESPACE