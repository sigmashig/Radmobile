#include "RMServer.hpp"
#include "RmProtocol.hpp"
#include "RmSession.hpp"

#if PROTOCOL == 2
#include "WiFi.h"
#include "RmProtocolMqtt.hpp"
#endif
#if RC == 1
#include "RmRcEmulator.hpp"
#elif RC == 2
#include "RmRcPS2.hpp"
#endif

void RmServer::startWiFi(String ssid, String password)
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
                    }
                }
            );
    WiFi.begin(ssid.c_str(), password.c_str());
}

RmServer::RmServer()
{
    rmCommands = new RmCommands();
    // TODO: session should be transferred from server to client
    rmSession = new RmSession();

#if PROTOCOL == 1
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolMqtt();
    startWiFi(WIFI_SSID, WIFI_PWD);
    WiFi.mode(WIFI_STA);
    startWiFi(WIFI_SSID, WIFI_PWD);
#elif PROTOCOL == 3
#endif

#if RC == 1
    remoteControl = new RmRcEmulator();
#elif RC == 2
    PS2 = new RmRcPS2();
    remoteControl = PS2;
#endif
}

void RmServer::Begin()
{
    isConnected = true;
    Serial.println("RmServer::Begin()");
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, RMEVENT_CMD_RECEIVED, responseEventHandler, NULL, NULL);
    esp_event_handler_instance_register(RMRC_EVENT, RMRC_CMD, commandEventHandler, NULL, NULL);

    rmProtocol->Begin();
    isReady = true;
}

void RmServer::SendCommand(String command)
{
    rmProtocol->SendCommand(command);
}

void RmServer::responseEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("responseEventHandler");
}

void RmServer::commandEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("commandEventHandler");
    RmCommandPkg *command = (RmCommandPkg *)event_data;
    String commandString;
    RmCommands::CommandToString(*command, commandString);
    Serial.println("Command received: " + commandString);
    rmServer->SendCommand(commandString);
    // rmProtocol->Send("Hello from WS Server");
}

//--------------------------------
RmServer *rmServer = NULL;
