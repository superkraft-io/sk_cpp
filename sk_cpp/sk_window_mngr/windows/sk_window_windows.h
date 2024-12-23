#pragma once

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "../root/sk_window_root.h"

class SK_Window : public SK_Window_Root {
public:
	~SK_Window();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::string windowClassName = "SK_Window";
	HWND hwnd;
	WNDCLASS wc;

	void initialize(unsigned int _wndIdx) override;
	void create();
	int applyTo(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	void createWebView();
private:

};