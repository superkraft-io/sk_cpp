#pragma once

#include <wil/com.h>
#include "WebView2.h"
#include <optional>
#include <filesystem>
#include <regex>
#include <iostream>

#include "../../sk_var.hxx"
#include "../../utils/sk_str_utils.hxx"
#include "../../utils/sk_web_utils.hxx"
#include "../../utils/sk_file.hxx"

#include "json.hpp"

using namespace SK;


class SK_WebViewResource_Response {
public:
    wil::com_ptr<ICoreWebView2Environment> webviewEnvironment;

    
    int statusCode = 404;
    SK_String statusMessage = "Not found";
    nlohmann::json headers{ {"Content-Type", "application/json"} };
    std::vector<BYTE> data;

    wil::com_ptr<ICoreWebView2WebResourceResponse> response;

    
    SK_WebViewResource_Response() {
        SK_String defaultData = "{\"error\":\"404\",\"message\":\"Not found\"}";
        data = std::vector<BYTE>(defaultData.data.begin(), defaultData.data.end());
    }

    ~SK_WebViewResource_Response() {
        response.reset();
    }

    
    void setAsOK() {
        statusCode = 200;
        statusMessage = "OK";
    }

    bool JSON(nlohmann::json json) {
        SK_String resAsString = std::any_cast<nlohmann::json>(json).dump(0);
        if (resAsString == "null") resAsString = "{}";

        statusMessage = "";

        data = std::vector<BYTE>(resAsString.data.begin(), resAsString.data.end());

        headers["Content-Type"] = "application/json";

        setAsOK();

        return true;
    }

    bool JSON_OK() {
        return string("{}", "application/json");
    }

    bool string(SK_String str, SK_String mimeType = "plain/text") {
        //DBGMSG(str.c_str());

        statusMessage = "";

        data = std::vector<BYTE>(str.data.begin(), str.data.end());
        headers["Content-Type"] = mimeType;

        setAsOK();

        return true;
    }

    bool file(SK_String path, SK_String mimeType = "auto") {
        //DBGMSG(path.c_str());


        SK_File file;
        if (file.loadFromDisk(path.replaceAll("\\", "/").c_str())) {
            headers["Content-Type"] = (mimeType == "auto" ? file.mimeType : mimeType);
            data = std::vector<BYTE>(file.data.begin(), file.data.end());
            setAsOK();
            return true;
        }

        error(); //something went wrong reading the file so we return a 404

        return false;
    }

    bool error(int code = 404, SK_String msg = "Not Found") {
        statusCode = code;
        statusMessage = msg;

        JSON({
            {"error", code},
            { "message", msg }
        });
        
        return this;
    }

    wil::com_ptr<ICoreWebView2WebResourceResponse> SK_WebViewResource_Response::get() {
        // Create a memory stream from the byte array using CreateStreamOnHGlobal
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, data.size());
        if (hGlobal == nullptr) {
            // Handle error if memory allocation fails
            return nullptr;
        }

        // Lock the global memory and copy data to it
        void* pData = GlobalLock(hGlobal);
        if (pData != nullptr) {
            memcpy(pData, data.data(), data.size());
            GlobalUnlock(hGlobal);
        }


        // Create an IStream from the global memory handle
        wil::com_ptr<IStream> contentStream;
        HRESULT hr = ::CreateStreamOnHGlobal(hGlobal, TRUE, &contentStream);
        if (FAILED(hr)) {
            // Handle the error properly if needed
            GlobalFree(hGlobal);
            return nullptr;
        }




        wil::unique_cotaskmem_string _statusMessage = stringToUniqueCoTaskMemString(statusMessage);

        SK_String _headers = jsonToDelimitedString(headers) + "\r\nAccess-Control-Allow-Origin: *\r\n";
        wil::unique_cotaskmem_string _responseHeaders = stringToUniqueCoTaskMemString(_headers);

        // Create the response with status code, headers, and content stream
        hr = webviewEnvironment->CreateWebResourceResponse(contentStream.get(),   // The stream containing the custom content
            statusCode,             // HTTP status code
            _statusMessage.get(),   // Status message
            _responseHeaders.get(), // Headers
            &response               // Output response
        );

        if (SUCCEEDED(hr)) {
            return response.get();
        }
    }
};
