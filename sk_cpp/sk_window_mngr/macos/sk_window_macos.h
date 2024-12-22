#ifndef SK_WINDOW_MACOS_H
#define SK_WINDOW_MACOS_H

#include "../../sk_webview/sk_webview.h"

class SK_Window {
public:
    ~SK_Window();

    NSWindow *window;
    id resizeObserver;

    /* Functions below must be the same regardless of OS */
    std::string title = "SK Window";

    bool visible = false;

    int width = 800;
    int height = 600;
    int left = 0;
    int top = 0;


    SK_WebView webview;

    void create();
    int applyTo();

    void initialize(unsigned int wndIdx);

    void createWebView();

private:
    void handleWindowResize(NSNotification* notification);
};


#endif
