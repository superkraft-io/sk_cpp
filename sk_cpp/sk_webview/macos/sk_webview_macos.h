
#include <iostream>

class SK_WebView {
public:
    NSWindow *parentWnd;
    
    WKWebView *webview;
    
    void create();
    void update();

    std::string currentURL = "";
    void gotoURL(std::string url);
private:

};
