#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include <WiFi.h>

ESP_EVENT_DECLARE_BASE(RMSERVER_EVENT);

typedef enum
{
    RMEVENT_RESPONSE
} RmServerEvent;

class RmServer
{
public:
    RmServer();
    RmServer(String ssid, String password);
    void Begin();
 //   void SendCommand(String command);
private:
    String ssid;
    String password;

    void startWiFi(String ssid, String password);

    static void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

};

//--------------------------------
extern RmServer *rmServer;
