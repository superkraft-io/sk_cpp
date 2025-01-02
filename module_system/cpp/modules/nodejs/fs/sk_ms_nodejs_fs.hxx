#pragma once

#include "../../../../../core/utils/sk_path_utils.hxx"
#include "../../../../../core/sk_webview_resourceHandler/os/sk_webview_resourceHandler_response_windows.hxx"
#include "../../../../../core/utils/sk_string.hxx"
#include "../../../../../core/utils/sk_file.hxx"
#include "../../../../../core/utils/sk_array.hxx"
#include "json.hpp"

class SK_Module_fs {
public:
    SK_Module_fs::SK_Module_fs() {
        nlohmann::json stat = SK_File::getFileInfo("c:/asus");
        SK_String statStr = stat.dump(4);
        int x = 0;
    }

    void handleOperation(const SK_String& operation, const nlohmann::json& payload, SK_WebViewResource_Response& respondWith) {
        SK_String path = payload["path"];

        if (path.substring(0, 7) == "sk_vfs/") {
            //vbe->sk_c_api->sk->vfs->handle_IPC_Msg(msgID, obj, responseData);
            //return;
        }

        //!!! IMPORTANT !!!! If in RELEASE mode, refer to BDFS and return

        #ifdef SK_MODE_RELEASE
            //if (vbe->mode != "debug") {
            //    vbe->sk_c_api->sk->bdfs->handle_IPC_Msg(msgID, obj, responseData);
            //    return;
           // }
        #endif

        SK_String fullPath = path;

        //check if file exists using path as-is. If it does exist, then most likely the path is n absolute path, se we do not prepend project path.
        /*if (!fs::exists(path.toStdString())) {
            if (operation != "mkdir") {
                fullPath = SK_FS::getProjectPath() + "/assets" + path;
            }
        }*/

        //If the path is not absolute, then make the SK_Project folder the root folder
        if (!SK_File::isPathAbsolute(path)) {
            if (operation != "mkdir") {
                fullPath = SK_Path_Utils::paths["sk_project"] + path;
            }
        }

        
             if (operation == "access"   ) access(fullPath, respondWith);
        else if (operation == "stat"     ) stat(fullPath, respondWith);
        else if (operation == "writeFile") writeFile(fullPath, payload["data"].get<std::string>(), respondWith);
        else if (operation == "readFile" ) readFile(fullPath, respondWith);
        else if (operation == "readdir"  ) readdir(fullPath, respondWith);
        else if (operation == "unlink"   ) unlink(fullPath, respondWith);
        else if (operation == "mkdir"    ) mkdir(fullPath, respondWith);
    };



    
    void access(const SK_String& path, SK_WebViewResource_Response& respondWith) {
        respondWith.JSON(nlohmann::json{ {"access", SK_String(SK_File::exists(path) ? "true" : "false")} });
    };

    void writeFile(const SK_String& path, const SK_String& data, SK_WebViewResource_Response& respondWith) {
        if (SK_File::exists(path)) SK_File::unlink(path);
        SK_File file;

        bool writeStatus = SK_File::append(path, data);

        if (writeStatus) {
            respondWith.JSON_OK();
        }
        else {
            respondWith.error(404, "ENOENT");
        }

    };

    void readFile(const SK_String& path, SK_WebViewResource_Response& respondWith) {
        SK_File file;
        file.loadFromDisk(path);

        respondWith.JSON(nlohmann::json{ {"data", file.toBase64()} });
    };

    void readdir(const SK_String& path, SK_WebViewResource_Response& respondWith) {
        nlohmann::json list = SK_File::list(path);

        nlohmann::json fileList;

        for (int i = 0; i < list.size(); i++) {
            nlohmann::json file = list[i];
            fileList.push_back(nlohmann::json{
                {"type", (file["type"] == "directory" ? "dir" : "file")},
                {"name", file["name"]}
            });
        }



        respondWith.JSON(fileList);
    };

    void unlink(const SK_String& path, SK_WebViewResource_Response& respondWith) {
        try {
            if (SK_File::unlink(path)) {
                respondWith.JSON_OK();
            }
            else {
                respondWith.error(404, "EBUSY");
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            respondWith.error(404, e.what());
        }
    };

    void mkdir(const SK_String& path, SK_WebViewResource_Response& respondWith) {
        try {
            // Use create_directories to create the directory and all its parent directories
            if (std::filesystem::create_directories(path)) {
                respondWith.JSON_OK();
            }
            else {
                respondWith.error(404, "EBUSY");
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            respondWith.error(404, e.what());
        }
    };
    
    void stat(const SK_String& path, SK_WebViewResource_Response& respondWith) {
        nlohmann::json statInfo = SK_File::getFileInfo(path);

        if (statInfo.contains("error")) {
            respondWith.error(404, "ENOENT");
            return;
        }

        respondWith.JSON(statInfo);;
    };
};
