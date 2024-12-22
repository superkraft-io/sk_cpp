#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "../../sk_webview/sk_webview.h"

class SK_Window {
public:
	~SK_Window();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::string windowClassName = "SK_Window";
	HWND hwnd;
	WNDCLASS wc;




	/* Functions below must be the same regardless of OS */
	std::string title = "SK Window";

	bool visible = false;

	int width = 800;
	int height = 600;
	int left = 0;
	int top = 0;


	SK_WebView webview;

	void create();
	int applyTo(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

	void initialize(unsigned int wndIdx);

	void createWebView();
private:

};