#pragma once

#include "wdlstring.h"
#include <functional>
#include <memory>

#include "json.hpp"
#include <string>


#include "../sk_common.hxx"

BEGIN_SK_NAMESPACE

/** IWebView is a base interface for hosting a platform web view inside an IPlug plug-in's UI */
class SK_IPC {
public:
    std::string sender_id = "SK_Backend";
    long long msg_id = 0;


    using SK_IPC_FrontendCallback = std::function<void(nlohmann::json, std::string& responseData)>;
    using SK_IPC_BackendCallback = std::function<void(nlohmann::json)>;

    SK_IPC_BackendCallback onSendToFrontend;
    SK_IPC_BackendCallback onMessage;

    std::unordered_map<std::string, SK_IPC_BackendCallback> awaitList;
    std::unordered_map<std::string, SK_IPC_FrontendCallback> listeners;
    std::unordered_map<std::string, SK_IPC_FrontendCallback> listeners_once;


    /** Returns a standard OK IPC message*/
    static inline const std::string OK = "{}";

    /** Returns an ERROR IPC object
    * @param error Error code
    * @param message A human readable message
    * @return Returns a stringified JSON object, e.g {error: "failed", message: "This request failed"}*/
    static inline const std::string Error(std::string error, std::string message = ""){
        nlohmann::json json;

        json["error"] = error;
        json["message"] = message;

        return json.dump();
    }

  
    nlohmann::json handle_IPC_Msg(const nlohmann::json& payload) {
        std::string msg_id = std::string(payload["msg_id"]);
        std::string type = std::string(payload["type"]);
        std::string sender = std::string(payload["sender"]);
        std::string target = std::string(payload["target"]);
        std::string event_id = std::string(payload["event_id"]);
        nlohmann::json data = payload["data"];


        if (type == "request") {
            nlohmann::json SK_iPlug2_IPC_packet;
            SK_iPlug2_IPC_packet["msg_id"] = payload["msg_id"];
            SK_iPlug2_IPC_packet["type"] = "response";
            SK_iPlug2_IPC_packet["sender"] = "iPlug2_backend";
            SK_iPlug2_IPC_packet["target"] = sender;
            SK_iPlug2_IPC_packet["event_id"] = payload["event_id"];

            std::string responseData = "{\"error\":\"invalid_ipc_request\"}";

            handleRequest(msg_id, type, sender, event_id, data, responseData);

            SK_iPlug2_IPC_packet["data"] = responseData;

            return SK_iPlug2_IPC_packet;
        }
        else if (type == "response") {
            handleResponse(msg_id, type, sender, event_id, data);
            return NULL;
        }
        else if (type == "message") {
            handleMessage(data);
            return NULL;
        }
    }

    /** Returns an event type if it exists
    * @param event_id Name of the event
    * @return "always" for a standard event, "once" for a one-time event, "" (empty string) if the event does not exist*/
    std::string SK_IPC::eventExists(const std::string& event_id){
        auto listener = listeners[event_id];
        auto listener_once = listeners_once[event_id];

        if (listener != NULL) return "always";
        if (listener_once != NULL) return "once";

        return "";
    }

    /** Adds an event that is fired when the frontend requests a response with the specified event ID
    * @param event_id Name of the event
    * @param callback Callback with the event data
    * @return A string representing the event message ID*/
    void on(const std::string& event_id, SK_IPC_FrontendCallback callback) {
        if (eventExists(event_id) != "") return;

        listeners[event_id] = callback;
    }


  
  
    /** Removes an event
    * @param event_id Name of the event*/
    void off(const std::string& event_id) {
        listeners.erase(event_id);
        listeners_once.erase(event_id);
    }

  
    /** Adds a one-time event that is fired when the frontend requests a response with the specified event ID. A one-time event is automatically removed once it has been fired.
    * @param event_id Name of the event
    * @param callback Callback with the event data*/
    void once(const std::string& event_id, SK_IPC_FrontendCallback callback) {
        if (eventExists(event_id) != "") return;

        listeners_once[event_id] = callback;
    }


    void handleRequest(std::string msg_id, std::string type, std::string sender, std::string event_id, const nlohmann::json& data, std::string& responseData) {
        auto listener = listeners[event_id];
        auto listener_once = listeners_once[event_id];

        bool exists = false;

        if (listener != NULL) {
            exists = true;
            listener(data, responseData);
        }
        else {
            if (listener_once != NULL) {
                exists = true;
                listener_once(data, responseData);
                off(event_id);
            }
        }

        if (!exists) {
            responseData = SK_IPC::Error("unknown_listener", "A listener with the event ID \"" + event_id + "\" does not exist.");
        }
    }


    void handleResponse(std::string msg_id, std::string type, std::string sender, std::string event_id, const nlohmann::json& data) {
        SK_IPC_BackendCallback awaiter = awaitList[msg_id];
        if (awaiter != NULL) awaiter(data);
    }

    void handleMessage(const nlohmann::json& json) {
        if (onMessage != NULL) onMessage(json);
    }

    std::string sendToFE(std::string event_id, nlohmann::json data, std::string type, SK_IPC_BackendCallback cb) {
        std::string _type = "request";
        if (type != "") _type = type;


        if (event_id == "") throw "[SK_iPlug2 IPC.sendToFE] Event ID cannot be empty";

        msg_id++;

        nlohmann::json req;

        req["msg_id"] = std::to_string(msg_id);
        req["type"] = _type;
        req["sender"] = sender_id;
        req["target"] = "SK_iPlug2_frontend";
        req["event_id"] = event_id;
        req["data"] = data;


        if (_type == "request") {
            awaitList[req["msg_id"]] = cb;
        }

        onSendToFrontend(req.dump());

        return req["msg_id"];
    }
  
    /** Makes a request to the frontend and awaits a response (currently indefinitely)
    * @param event_id Name of the event
    * @param data Data to send
    * @param cb Callback of the response*/
    void request(std::string event_id, nlohmann::json data, SK_IPC_BackendCallback cb) {
        sendToFE(event_id, data, "request", cb);
    }

    /** Sends a response-less message to the frontend. This function does NOT expect or wait for a response.
    * @param data Data to send*/
    void message(nlohmann::json data) {
        sendToFE("SK_iPlug2_IPC_Message", data, "message", NULL);
    }

private:

   
};

END_SK_NAMESPACE
