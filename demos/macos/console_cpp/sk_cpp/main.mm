#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

#include "superkraft.hxx"


int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // Create the application instance
        NSApplication *app = [NSApplication sharedApplication];
        
        //create the main window
        SK_Window* wnd = sk.wndMngr.newWindow([](SK_Window* _c) {
            _c->webview.navigate("https://github.com/superkraft-io");
            _c->visible = true;
            _c->applyTo();
        });
        
        //create the second window
        SK_Window* secondWnd = sk.wndMngr.newWindow([](SK_Window* _c) {
            _c->visible = true;
            _c->title = "Second window";
            _c->transparent = true;
            //_c->webview.navigate("https://github.com/superkraft-io/sk_cpp");
            _c->create();
        });



        // Run the application event loop
        [app run];
    }
    return 0;
}
