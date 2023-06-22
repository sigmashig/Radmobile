#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include "RmRemoteControl.hpp"
#include "SigmaLoger.hpp"
#include "RmProtocol.hpp"

class RmServer
{
public:
    RmServer();
    void Begin();
    void SendCommand(String command);
    bool IsReady() { return isReady; };

private:
    bool alreadyConnected = false;
    bool isReady = false;
    bool isBeginRequired = false;
    void startWiFi(String ssid, String password);
    void startExchange();
    RmProtocol *logProtocol;
    static void commandEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    static void log_publisher(SigmaLogLevel level, const char *msg);
    // void ReceivedCommand(String command);
    static void responseEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    //    static void loopEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};

//--------------------------------
extern RmServer *rmServer;
