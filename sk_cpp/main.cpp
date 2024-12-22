#include <windows.h>
#include <iostream>
#include <stdio.h>

#include "superkraft.hxx"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    //create the main window
    SK_Window* wnd = sk.wndMngr.newWindow();
    wnd->visible = true;
    wnd->applyTo(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

    //create the second window
    /*SK_Window* secondWnd = sk.wndMngr.newWindow([](SK_Window* _c) {
        _c->visible = true;
        _c->title = "Second window";
        _c->create();
    });
    */



    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}