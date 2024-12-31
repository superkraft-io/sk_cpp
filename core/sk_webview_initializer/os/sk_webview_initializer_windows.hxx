
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include "WebView2EnvironmentOptions.h"

#include <optional>
#include <filesystem>
#include <regex>
#include <iostream>

#include "json.hpp"

#include "../../sk_common.hxx"
#include "../../utils/sk_str_utils.hxx"
#include "../../utils/sk_web_utils.hxx"

#if SK_FRAMEWORK == SK_FRAMEWORK_SUPERKRAFT
    //Include nothing
#elif SK_FRAMEWORK == SK_FRAMEWORK_iPlug2
    #include "config.h"
#elif SK_FRAMEWORK == SK_FRAMEWORK_JUCE
    //Include JUCE header
#endif

using namespace Microsoft::WRL;
using namespace SK;

BEGIN_SK_NAMESPACE

class SK_WebView_Initializer {
public:
    wil::com_ptr<ICoreWebView2> webview;

    void init(void* _webview){
        webview = static_cast<ICoreWebView2*>(_webview);
        inject_core();

        webview = nullptr;
    }

    void inject_core(){
        SK_String modulesRoot = "https://sk.sb.gc";

        injectData("window.sk_api = {}");

        SK_String payload = generateFromFiles(std::vector<SK_String>{
            SK_Path_Utils::paths["global_js_core"] + "/sk_ipc.js",
            SK_Path_Utils::paths["global_js_core"] + "/module.js",
            SK_Path_Utils::paths["global_js_core"] + "/sk_global_js_core.js"
        })
            .replace("'<sk_static_info>'", getStaticInfo());

        injectData(payload);
    }

    SK_String generateFromFiles(std::vector<SK_String>& paths){
        SK_String data;

        for (int i = 0; i < paths.size(); i++) {
            SK_File file;

            #ifdef SK_MODE_DEBUG
                file.loadFromDisk(paths[i]);
            #else
            #endif

            data += "\n\r" + file;
        }

        return data;
    }

    void injectData(const SK_String& data){
        webview->AddScriptToExecuteOnDocumentCreated(
            data,
            Callback<ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler>(
                [this](HRESULT error, PCWSTR id) -> HRESULT {
                    return S_OK;
                }
            ).Get()
       );
    }

    SK_String getStaticInfo() {
        SK_String appName = sk_config["product_info"]["name"];
        SK_String appVersion = sk_config["product_info"]["version"];

        nlohmann::json res {
            {"argv"   ,  "<argv>"},
            {"argv0"  , ""},
            {"mode"   , "debug"},
            {"name"   , SK_MODE},
            {"version", "0.0"}
        };

        SK_String output = res.dump();

        SK_String args = "[\"\"]";
        if (args == "[\"\"]") {
            args = "[]";
        }
        output = output.replace("\"<argv>\"", args);


        return output;
    }
};


END_SK_NAMESPACE
