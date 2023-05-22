#include "RmClient.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"
#include "RmProtocol.hpp"
#if PROTOCOL == 2
#include <WiFi.h>
#include "RmProtocolMqtt.hpp"
#endif
#include "RmVehicle.hpp"
#if VEHICLE == 1
#include "RmVehicleV1.hpp"
#endif

RmClient::RmClient()
{
    rmCommands = new RmCommands();
#if PROTOCOL == 1
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolMqtt();
    WiFi.mode(WIFI_STA);
    startWiFi(WIFI_SSID, WIFI_PWD);
#elif PROTOCOL == 3
#endif

    PinsDriver = new RmPinsDriver(rmConfig->clientPcfs, NUMB_OF_CLIENT_PCF);

#if VEHICLE == 1
    rmVehicle = new RmVehicleV1();
#endif
}

void RmClient::Begin()
{
    Serial.println("RmClient::Begin()");
    rmProtocol->Begin();
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, RMEVENT_CMD_RECEIVED,
                                        commandReceived, NULL, NULL);
}

void RmClient::startWiFi(String ssid, String password)
{

    Serial.printf(F("Connecting to WiFi network: %s(%s)\n"), ssid.c_str(), password.c_str());
    WiFi.mode(WIFI_STA);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 { 
                    switch (event)
                    {
                        case SYSTEM_EVENT_STA_GOT_IP:
                        {
                            Serial.println("WiFi connected");
                            Serial.printf(F("IP address: %s\n"), WiFi.localIP().toString().c_str());
                            Begin();
                            break;
                        }
                        case SYSTEM_EVENT_STA_DISCONNECTED:
                        {
                            Serial.printf(F("WiFi connection error: %d\n"), info.wifi_sta_disconnected.reason);
                            if (isConnected)
                            {
                                rmProtocol->Reconnect();
                            }
                        }
                        break;
                    } });
    WiFi.begin(ssid.c_str(), password.c_str());
}

void RmClient::commandReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("responseEventHandler");
    RmCommandPkg *cmdPkg = (RmCommandPkg *)event_data;
    rmVehicle->RunCmd(*cmdPkg);
}

//--------------------------------
RmClient *rmClient = NULL;
