#include "RmClient.hpp"

RmClient::RmClient(String host, uint16_t port)
{
    this->host = host;
    this->port = port;
}

void RmClient::Begin()
{
    Serial.println("RmClient::Begin()");
#if PROTOCOL == 1
    rmProtocol = new RmProtocolWs(host, port);
#elif PROTOCOL == 2
#endif
    startWiFi(ssid, password);
}

RmClient::RmClient(String host, uint16_t port, String ssid, String password)
{
    this->host = host;
    this->port = port;
    this->ssid = ssid;
    this->password = password;
}

void RmClient::startWiFi(String ssid, String password)
{

    Serial.printf(F("Connecting to WiFi network: %s(%s)\n"), ssid.c_str(), password.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(onWiFiEvent);
    WiFi.begin(ssid.c_str(), password.c_str());
}

void RmClient::onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
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
        Serial.printf(F("WiFi connection error: %d\n"), info.wifi_sta_disconnected.reason);
    }
}

//--------------------------------
RmClient *rmClient;
