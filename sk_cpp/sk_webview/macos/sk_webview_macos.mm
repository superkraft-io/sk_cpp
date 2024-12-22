#include <iostream>

#import <WebKit/WebKit.h>

#include "../../sk_str_utils.hxx"
#include "sk_webview_macos.h"


void SK_WebView::create() {
    NSRect frame = [parentWnd frame];
    webview = [[WKWebView alloc] initWithFrame: frame];
    [webview setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
    [parentWnd setContentView: webview];	
    
    gotoURL(currentURL);
}


void SK_WebView::update() {
    NSRect frame = [parentWnd frame];
    [webview setFrame: frame];
}




void SK_WebView::gotoURL(std::string url) {
    currentURL = url;

    if (webview == NULL) return;

    NSURL *_url = [NSURL URLWithString: StringToNSString(currentURL)];
    NSURLRequest *request = [NSURLRequest requestWithURL: _url];
    [webview loadRequest:request];
}
