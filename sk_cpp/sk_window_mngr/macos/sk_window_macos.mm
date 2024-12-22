#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#import <WebKit/WebKit.h>

#include "../../sk_str_utils.hxx"

#include "sk_window_macos.h"


SK_Window::~SK_Window() {
    
}

 void SK_Window::initialize(unsigned int wndIdx) {
     
 }
 
 void SK_Window::create() {
     
     applyTo();
 }

int SK_Window::applyTo(){
    NSRect frame = NSMakeRect(100, 100, 800, 600);
    window = [[NSWindow alloc] initWithContentRect:frame
                                                   styleMask:(NSWindowStyleMaskTitled |
                                                              NSWindowStyleMaskClosable |
                                                              NSWindowStyleMaskResizable)
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
    
    // Create a block to call the static function when the window is resized
    resizeObserver = [[NSNotificationCenter defaultCenter] addObserverForName:NSWindowDidResizeNotification
                                                                       object:window
                                                                        queue:nil
                                                                   usingBlock:^(NSNotification *notification) {
        // Call the static function to handle the resize logic
        handleWindowResize(notification);
    }];
    
    
    [window setTitle: StringToNSString(title)];
    [window makeKeyAndOrderFront:nil];
    
    webview.parentWnd = window;
    
    createWebView();
    
    return 0;
}

void SK_Window::createWebView() {
    webview.create();
}


void SK_Window::handleWindowResize(NSNotification* notification) {
    webview.update();
}
