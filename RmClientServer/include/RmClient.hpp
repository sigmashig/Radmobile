#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include "RmConfiguration.hpp"

class RmClient
{
public:
    RmClient();
    void Begin();
    bool IsReady() { return isReady; };

private:
    bool isConnected = false;
    bool isReady = false;
    bool isBeginRequired = false;

    void startWiFi(String ssid, String password);
    static void commandReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};

extern RmClient *rmClient;