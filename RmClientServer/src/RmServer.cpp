#include "RMServer.hpp"
#include "RmProtocol.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"

#if PROTOCOL == 1
#include "WiFi.h"
#include "RmProtocolMqtt.hpp"
#elif PROTOCOL == 2
#include "RmProtocolLora.hpp"
#endif
#if RC == 1
#include "RmRcEmulator.hpp"
#elif RC == 2
#include "RmRcPS2.hpp"
#endif

void RmServer::startWiFi(String ssid, String password)
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
                            if (alreadyConnected)
                            {
                                rmProtocol->Reconnect();
                            }
                        }
                        break;
                    } });
    WiFi.begin(ssid.c_str(), password.c_str());
#endif
}

RmServer::RmServer()
{
    Serial.println("RmServer::RmServer()");
    // TODO: session should be transferred from server to client
    rmCommands = new RmCommands();

    rmSession = new RmSession();
    Serial.println("Point 1");
#if RC == 1
    remoteControl = new RmRcEmulator();
#elif RC == 2
    PS2 = new RmRcPS2();
    remoteControl = PS2;
#endif
    Serial.println("Point 2");

#if PROTOCOL == 1
    isBeginRequired = false;
    rmProtocol = new RmProtocolMqtt();
    WiFi.mode(WIFI_STA);
    startWiFi(WIFI_SSID, WIFI_PWD);
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolLora();
    isBeginRequired = true;
#endif
    Serial.println("RmServer::RmServer() end");
    if (isBeginRequired)
    {
        Begin();
    }
}

void RmServer::Begin()
{
    alreadyConnected = true;
    Serial.println("RmServer::Begin()");
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, responseEventHandler, NULL, NULL);
    esp_event_handler_instance_register(RMRC_EVENT, RMRC_NEWSTATE, commandEventHandler, NULL, NULL);

    rmProtocol->Begin();
    remoteControl->Begin();
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
    CommandState *commandState = (CommandState *)event_data;
    String stateString;
    stateString = RmRemoteControl::StateAsString(*commandState);
    Serial.println("State Ready: " + stateString);
    rmServer->SendCommand(stateString);
    // rmProtocol->Send("Hello from WS Server");
}

//--------------------------------
RmServer *rmServer = NULL;
