#import <Cocoa/Cocoa.h>


#include "superkraft.hxx"

NSString * const ViewDidLoadNotification = @"ViewDidLoadNotification";

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        NSApplication *app = [NSApplication sharedApplication];

        // Add observer for ViewDidLoadNotification
        [[NSNotificationCenter defaultCenter] addObserverForName:ViewDidLoadNotification
                                                         object:nil
                                                          queue:[NSOperationQueue mainQueue]
                                                     usingBlock:^(NSNotification * _Nonnull note)
         {
            NSWindow *mainWindow = app.windows.firstObject;
            
            sk.wndMngr.newWindow([mainWindow](SK_Window* _c) {
                _c->webview.navigate("https://github.com/superkraft-io");
                _c->visible = true;
                _c->applyTo(mainWindow);
            });
            
            //create the second window
            /*sk.wndMngr.newWindow([](SK_Window* _c) {
                _c->visible = true;
                _c->title = "Second window";
                _c->transparent = true;
                //_c->webview.navigate("https://github.com/superkraft-io/sk_cpp");
                _c->create();
            });*/
        }];
        
        
        return NSApplicationMain(argc, argv);
    }
}
