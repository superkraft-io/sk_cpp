#include <windows.h>
#include <wil/com.h>
#include <wrl.h>
#include <WebView2.h>
#include "WebView2EnvironmentOptions.h"


#pragma comment(lib, "ole32.lib")

#include "sk_webview_windows.h"

using namespace Microsoft::WRL;

void SK_WebView::create() {
    // Initialize WebView2
    // 
// Step 3 - Create a single WebView within the parent window
// Locate the browser and set up the environment for WebView
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result)) {
                    // Debug: Log WebView2 environment creation failure
                    return result;
                }

                // Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
                env->CreateCoreWebView2Controller(*parentHwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [this](HRESULT result, ICoreWebView2Controller* _controller) -> HRESULT {
                        if (FAILED(result)) {
                            // Debug: Log controller creation failure
                            return result;
                        }
                        
                        
                        controller = _controller;
                        controller->get_CoreWebView2(&webview);

                        // Add a few settings for the webview
                        // The demo step is redundant since the values are the default settings
                        webview->get_Settings(&settings);
                        settings->put_IsScriptEnabled(TRUE);
                        settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                        settings->put_IsWebMessageEnabled(TRUE);
                        settings->put_AreDefaultContextMenusEnabled(true);
                        settings->put_AreDevToolsEnabled(true);

                        // Resize WebView to fit the bounds of the parent window
                        RECT bounds;
                        GetClientRect(*parentHwnd, &bounds);
                        controller->put_Bounds(bounds);

                        // Schedule an async task to navigate to Bing
                        webview->Navigate(L"https://github.com/superkraft-io");

                        // Step 4 - Navigation events

                        // Step 5 - Scripting

                        // Step 6 - Communication between host and web content

                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());

}


void SK_WebView::update() {
    if (controller != nullptr) {
        RECT bounds;
        GetClientRect(*parentHwnd, &bounds);
        controller->put_Bounds(bounds);
    };
}