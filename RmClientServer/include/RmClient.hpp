#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include "RmConfiguration.hpp"
#include "RmProtocol.hpp"
#include <SigmaLoger.hpp>

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
    RmProtocol *logProtocol = NULL;

    // void startWiFi(String ssid, String password);

    static void log_publisher(SigmaLogLevel level, const char *msg);

    // static void stateReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};

extern RmClient *rmClient;