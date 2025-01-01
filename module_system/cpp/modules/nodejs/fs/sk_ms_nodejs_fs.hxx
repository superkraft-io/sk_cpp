#pragma once


class SK_Module_fs {
public:
    void handleOperation(const SK_String& operation, const nlohmann::json& payload);
    /*
    void access(String msgID, String path, String& responseData);
    void stat(String msgID, String path, String& responseData);
    void writeFile(String msgID, String path, String data, String& responseData);
    void readFile(String msgID, String path, String& responseData);
    void readdir(String msgID, String path, String& responseData);
    void unlink(String msgID, String path, String& responseData);
    void mkdir(String msgID, String path, String& responseData);
    */
};
