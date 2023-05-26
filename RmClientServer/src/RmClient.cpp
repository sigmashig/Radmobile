#include "RmClient.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"
#include "RmProtocol.hpp"
#if PROTOCOL == 1
#include <WiFi.h>
#include "RmProtocolMqtt.hpp"
#elif PROTOCOL == 2
#include "RmProtocolLora.hpp"
#endif
#include "RmVehicle.hpp"
#if VEHICLE == 1
#include "RmVehicleV1.hpp"
#endif

RmClient::RmClient()
{
    rmCommands = new RmCommands();
    // TODO: session should be transferred from server to client
    rmSession = new RmSession();
    rmPinsDriver = new RmPinsDriver(rmConfig->clientPcfs, NUMB_OF_CLIENT_PCF);
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, RMEVENT_CMD_RECEIVED,
                                        commandReceived, NULL, NULL);
#if PROTOCOL == 1
    isBeginRequired = false;
    rmProtocol = new RmProtocolMqtt();
    startWiFi(WIFI_SSID, WIFI_PWD);
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolLora();
    isBeginRequired = true;
#endif
#if VEHICLE == 1
    rmVehicle = new RmVehicleV1();
#endif
    if (isBeginRequired)
    {
        Begin();
    }
}

void RmClient::Begin()
{
    Serial.println("RmClient::Begin()");
    rmPinsDriver->Begin();
    Serial.println("Point 1");
    rmProtocol->Begin();
    Serial.println("Point 2");
    rmVehicle->Begin();
    Serial.println("Point 3");
    
}

void RmClient::startWiFi(String ssid, String password)
{
#if PROTOCOL == 1
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
#endif
}

void RmClient::commandReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("responseEventHandler");
    RmCommandPkg *cmdPkg = (RmCommandPkg *)event_data;
    rmVehicle->RunCmd(*cmdPkg);
}

//--------------------------------
RmClient *rmClient = NULL;
