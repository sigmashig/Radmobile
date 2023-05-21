#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include "RmRemoteControl.hpp"

class RmServer
{
public:
    RmServer();
    void Begin();
    void SendCommand(String command);
    bool IsReady() { return isReady; };

private:
    bool isConnected = false;
    bool isReady = false;
    void startWiFi(String ssid, String password);
    static void commandEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    // void ReceivedCommand(String command);
    static void responseEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    //    static void loopEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};

//--------------------------------
extern RmServer *rmServer;
