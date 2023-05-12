#include "RMServer.hpp"
#include <WiFi.h>
#include "RmProtocol.hpp"
#include "RmProtocolWs.hpp"

RmServer::RmServer()
{
}

RmServer::RmServer(String ssid, String password)
{
    this->ssid = ssid;
    this->password = password;
}

void RmServer::Begin()
{
    Serial.println("RmServer::Begin()");
 #if PROTOCOL == 1
    rmProtocol = new RmProtocolWs();   
#elif PROTOCOL == 2
#endif    
    startWiFi(ssid, password);
}

void RmServer::startWiFi(String ssid, String password)
{
    
        Serial.printf(F("Connecting to WiFi network: %s(%s)\n"), ssid.c_str(), password.c_str());
        WiFi.mode(WIFI_STA);
        WiFi.onEvent(onWiFiEvent);
        WiFi.begin(ssid.c_str(), password.c_str());
}

void RmServer::onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
        switch (event)
        {
        case SYSTEM_EVENT_STA_GOT_IP:
        {
            Serial.println("WiFi connected");
            Serial.printf(F("IP address: %s\n"), WiFi.localIP().toString().c_str());
            rmProtocol->Begin();
            break;
        }
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.printf(F("WiFi connection error: %d\n"),info.wifi_sta_disconnected.reason);
        }
}

//--------------------------------
RmServer *rmServer;
ESP_EVENT_DEFINE_BASE(RMSERVER_EVENT);