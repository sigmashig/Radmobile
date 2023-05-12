#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "RmProtocol.hpp"
#include "RmProtocolWs.hpp"

class RmClient
{
public:
    RmClient(String host, uint16_t port, String ssid, String password);
    RmClient(String host, uint16_t port);
    void Begin();

private:
    String ssid;
    String password;
    String host;
    uint16_t port;

    void startWiFi(String ssid, String password);

    static void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
};

extern RmClient *rmClient;