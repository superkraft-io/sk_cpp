#pragma once

#include <wil/com.h>
#include "WebView2.h"
#include <optional>
#include <filesystem>
#include <regex>
#include <iostream>

#include "../../sk_common.hxx"
#include "../../utils/sk_str_utils.hxx"
#include "../../utils/sk_web_utils.hxx"
#include "../../utils/sk_file.hxx"
#include "sk_webview_resourceHandler_response_windows.hxx"
#include "../../../module_system/sk_module_system.hxx"


using namespace SK;


// Enum to represent resource context with MIME types
enum class ResourceContext {
    Document,
    Stylesheet,
    Image,
    Media,
    Font,
    Script,
    XMLHttpRequest,
    Fetch,
    TextTrack,
    EventSource,
    WebSocket,
    Manifest,
    Other
};

// Map enum values to corresponding MIME types or descriptions
static const std::map<ResourceContext, SK_String> ResourceContextMimeMap = {
    {ResourceContext::Document      , "text/html"                   },
    {ResourceContext::Stylesheet    , "text/css"                    },
    {ResourceContext::Image         , "image/*"                     },
    {ResourceContext::Media         , "video/*, audio/*"            },
    {ResourceContext::Font          , "font/*"                      },
    {ResourceContext::Script        , "application/javascript"      },
    {ResourceContext::XMLHttpRequest, "application/json"            },
    {ResourceContext::Fetch         , "application/json"            },
    {ResourceContext::TextTrack     , "text/vtt"                    },
    {ResourceContext::EventSource   , "text/event-stream"           },
    {ResourceContext::WebSocket     , "application/websocket"       },
    {ResourceContext::Manifest      , "application/json"            },
    {ResourceContext::Other         , "application/octet-stream"    }
};

// Function to convert COREWEBVIEW2_WEB_RESOURCE_CONTEXT to ResourceContext
static ResourceContext ConvertToResourceContext(COREWEBVIEW2_WEB_RESOURCE_CONTEXT context)
{
    switch (context) {
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_DOCUMENT:
        return ResourceContext::Document;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_STYLESHEET:
        return ResourceContext::Stylesheet;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_IMAGE:
        return ResourceContext::Image;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_MEDIA:
        return ResourceContext::Media;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_FONT:
        return ResourceContext::Font;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_SCRIPT:
        return ResourceContext::Script;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_XML_HTTP_REQUEST:
        return ResourceContext::XMLHttpRequest;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_FETCH:
        return ResourceContext::Fetch;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_TEXT_TRACK:
        return ResourceContext::TextTrack;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_EVENT_SOURCE:
        return ResourceContext::EventSource;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_WEBSOCKET:
        return ResourceContext::WebSocket;
    case COREWEBVIEW2_WEB_RESOURCE_CONTEXT_MANIFEST:
        return ResourceContext::Manifest;
    default:
        return ResourceContext::Other;
    }
}



static SK_String ReadStream(IStream* stream)
{
    if (!stream)
        return "";
    char buffer[4096];
    ULONG bytesRead;
    SK_String result;
    while (SUCCEEDED(stream->Read(buffer, sizeof(buffer), &bytesRead)) && bytesRead > 0)
    {
        result.data.append(buffer, bytesRead);
    }
    return result;
}

// Function to get MIME type from ResourceContext
static SK_String GetMimeTypeFromResourceContext(ResourceContext context)
{
    auto it = ResourceContextMimeMap.find(context);
    return (it != ResourceContextMimeMap.end()) ? it->second : "unknown";
}

static SK_String resourceContextToString(COREWEBVIEW2_WEB_RESOURCE_CONTEXT webResourceContext)
{
    ResourceContext context = ConvertToResourceContext(webResourceContext);
    return GetMimeTypeFromResourceContext(context);
}






static nlohmann::json ExtractHeadersToJson(ICoreWebView2HttpRequestHeaders* headers)
{
    nlohmann::json headersJson;

    // Start by getting the iterator for the headers
    wil::com_ptr<ICoreWebView2HttpHeadersCollectionIterator> iterator;
    if (SUCCEEDED(headers->GetIterator(&iterator)))
    {
        while (true)
        {
            // Get the current header's name and value
            wil::unique_cotaskmem_string headerName, headerValue;
            HRESULT hr = iterator->GetCurrentHeader(&headerName, &headerValue);

            if (SUCCEEDED(hr))
            {
                // Add the header name and value to the JSON object
                headersJson[wstringToString(headerName.get())] = wstringToString(headerValue.get());
            }

            else
            {
                break; // Stop if GetCurrentHeader fails
            }


            // Move to the next header; stop if no more headers
            BOOL hasNext = FALSE;
            if (FAILED(iterator->MoveNext(&hasNext)) || !hasNext)
            {
                break;
            }
        }
    }

    return headersJson;
}







class SK_WebViewResource_Request
{
public:
    SK_String url;
    SK_String protocol;
    SK_String host;
    SK_String path;
    SK_String method;
    SK_String resourceContext;
    nlohmann::json parameters;
    nlohmann::json headers;
    nlohmann::json body;



    SK_WebViewResource_Request::SK_WebViewResource_Request(ICoreWebView2WebResourceRequestedEventArgs* args) { parse(args); }

    void parse(ICoreWebView2WebResourceRequestedEventArgs* args) {
        wil::com_ptr<ICoreWebView2WebResourceRequest> request;
        args->get_Request(&request);


        // Extract URI
        wil::unique_cotaskmem_string _url;
        if (SUCCEEDED(request->get_Uri(&_url))) {
            url = wstringToString(_url.get());
            parseParameters(url);
        }

        // Extract Method
        wil::unique_cotaskmem_string _method;
        if (SUCCEEDED(request->get_Method(&_method))) {
            method = wstringToString(_method.get());
        }

        // Extract Headers
        wil::com_ptr<ICoreWebView2HttpRequestHeaders> _headers;
        if (SUCCEEDED(request->get_Headers(&_headers))) {
            headers = ExtractHeadersToJson(_headers.get());
        }

        COREWEBVIEW2_WEB_RESOURCE_CONTEXT _resourceContext;
        if (SUCCEEDED(args->get_ResourceContext(&_resourceContext))) {
            resourceContext = resourceContextToString(_resourceContext);
        }
    };

    void parseParameters(const SK_String& url)
    {
        std::regex urlRegex(R"(^(https?)://([^/?#]+)([^?#]*)(\?([^#]*))?)");
        std::smatch urlMatch;

        if (std::regex_match(url.data, urlMatch, urlRegex))
        {
            protocol = urlMatch[1].str(); // Protocol
            host = urlMatch[2].str();     // Host
            path = urlMatch[3].str();     // Path

            // Parse query parameters if they exist
            SK_String query = urlMatch[5].str();
            if (query.length() != 0) {
                std::regex queryRegex(R"(([^&=]+)=([^&]*)?)");
                auto queryBegin = std::sregex_iterator(query.data.begin(), query.data.end(), queryRegex);
                auto queryEnd = std::sregex_iterator();

                for (auto it = queryBegin; it != queryEnd; ++it)
                {
                    parameters[(*it)[1].str()] = (*it)[2].str();
                }
            }
        }
        else
        {
            throw std::invalid_argument("Invalid URL format");
        }
    }
};


using SK_WebViewResourceRequest_Callback = std::function<void(SK_WebViewResource_Request& request, SK_WebViewResource_Response& respondWith)>;

class SK_WebViewResourceHandler_Route {
public:
    SK_String id = "*";

    SK_WebViewResourceRequest_Callback onRequest;
};

class SK_WebViewResourceHandler_RouteMngr {
public:
    std::vector<SK_WebViewResourceHandler_Route*> routes;

    SK_WebViewResourceHandler_Route* on(const SK_String& id, SK_WebViewResourceRequest_Callback _onRequest) {
        SK_WebViewResourceHandler_Route* route = findByID_simple(id);
        if (route != nullptr) return route;

        route = new SK_WebViewResourceHandler_Route();
        route->id = id;
        route->onRequest = _onRequest;

        routes.push_back(route);
        
        return route;
    }

    bool off(const SK_String& id){
        std::pair<int, SK_WebViewResourceHandler_Route*> res = findByID(id);
        if (res.second == nullptr) return false;

        auto it = std::find(routes.begin(), routes.end(), res.second);
        if (it != routes.end()) {
            routes.erase(it); // Remove the pointer from the vector
        }

        delete res.second;
    }

    std::pair<int, SK_WebViewResourceHandler_Route*> findByID(const SK_String& id) {
        for (int i = 0; i < routes.size(); i++) {
            SK_WebViewResourceHandler_Route* route = routes[i];
            if (route->id.substring(0, id.length()) == id || id.substring(0, route->id.length()) == route->id) return std::pair<int, SK_WebViewResourceHandler_Route*>(i, route);
        }

        return std::pair<int, SK_WebViewResourceHandler_Route*>(-1, nullptr);
    }

    SK_WebViewResourceHandler_Route* findByID_simple(const SK_String& id) {
        std::pair<int, SK_WebViewResourceHandler_Route*> res = findByID(id);
        if (res.second == nullptr) return nullptr;
        return res.second;
    }

    SK_WebViewResourceHandler_Route* operator[](const SK_String& key) {
        return findByID_simple(key);
    }
};



class SK_WebViewResourceHandler {
public:
    SK_Module_System* modsys;

    SK_WebViewResourceHandler_RouteMngr routes;

    SK_WebViewResourceHandler::SK_WebViewResourceHandler(){
        //default route points to the "bundle" folder
        routes.on("*", [&](SK_WebViewResource_Request& request, SK_WebViewResource_Response& respondWith){
            int x = 0;
            #if defined SK_MODE_DEBUG
                std::string filePath = SK::SK_Path_Utils::paths["sk_project"] + request.path;
                respondWith.file(filePath);
            #else
                
            #endif
        });

        routes.on("https://sk.sb", [&](SK_WebViewResource_Request& request, SK_WebViewResource_Response& respondWith) {
            int x = 0;
            #if defined SK_MODE_DEBUG
                SK_String filePath = SK::SK_Path_Utils::paths["soft_backend"] + request.path;
                respondWith.file(filePath);
            #else

            #endif
        });

        routes.on("https://sk.modsys", [&](SK_WebViewResource_Request& request, SK_WebViewResource_Response& respondWith) {
            int x = 0;
            #if defined SK_MODE_DEBUG
                SK_String filePath = SK::SK_Path_Utils::paths["module_system"] + request.path;
                respondWith.file(filePath);
            #else

            #endif
        });

        routes.on("https://sk.modop", [&](SK_WebViewResource_Request& request, SK_WebViewResource_Response& respondWith) {
            SK_String data = request.path.substring(2, request.path.length()).fromBase64();
            nlohmann::json payload = nlohmann::json::parse(data.data);

            modsys->performOperation(
                payload["module"].get<std::string>(),
                payload["operation"].get<std::string>(),
                payload["payload"],
                respondWith
            );

            int x = 0;
        });
    }

    void handleRequest(SK_WebViewResource_Request& request, SK_WebViewResource_Response& respondWith){
        SK_String url = request.url;

        SK_String routeID = request.protocol + "://" + request.host;
        SK_WebViewResourceHandler_Route* route = routes.findByID_simple(routeID);

        if (route == nullptr) {
            routes["*"]->onRequest(request, respondWith);
            return;
        }
        
        route->onRequest(request, respondWith);
        
       

        /*response.fromJSON({
          {"key", "value"}
        });*/

        //respondWith.string("Hello :)", "text/html");

        
    };

};