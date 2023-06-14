#include "RMServer.hpp"
#include <SigmaIO.hpp>
#include "RmConfiguration.hpp"
#include "RmProtocol.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"
#include "SigmaLoger.hpp"
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
    Log->Append(F("Server. Connecting to WiFi network: ")).Append(ssid).Info();
    WiFi.mode(WIFI_STA);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 { 
                    switch (event)
                    {
                        case SYSTEM_EVENT_STA_GOT_IP:
                        {
                            Log->Append(F("WiFi connected. IP address:")),append(WiFi.localIP().toString()).Info();
                            Begin();
                            break;
                        }
                        case SYSTEM_EVENT_STA_DISCONNECTED:
                        {
                            Log->Append(F("WiFi connection error:")).Append(info.wifi_sta_disconnected.reason).Error();
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

    Log->Debug(F("SERVER"));
    // Init confguration
    sigmaIO = new SigmaIO(true);
    I2CParams i2cParms = {.address = I2C_ADDRESS, .pWire = NULL, .sda = 0, .scl = 0};

    sigmaIO->RegisterPinDriver(SIGMAIO_PCF8575, &(i2cParms), I2C_BEGIN, I2C_END);
    sigmaIO->Begin();
    // TODO: session should be transferred from server to client
    rmCommands = new RmCommands();

    rmSession = new RmSession();
#if RC == 1
    remoteControl = new RmRcEmulator();
#elif RC == 2
    PS2 = new RmRcPS2();
    remoteControl = PS2;
#endif

#if PROTOCOL == 1
    isBeginRequired = false;
    rmProtocol = new RmProtocolMqtt();
    WiFi.mode(WIFI_STA);
    startWiFi(WIFI_SSID, WIFI_PWD);
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolLora();
    isBeginRequired = true;
#endif
    if (isBeginRequired)
    {
        Begin();
    }

}

void RmServer::Begin()
{

    alreadyConnected = true;
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, responseEventHandler, NULL, NULL);
    esp_event_handler_instance_register(RMRC_EVENT, RMRC_NEWSTATE, commandEventHandler, NULL, NULL);

    rmProtocol->Begin();
    remoteControl->Begin();
    isReady = true;
}

void RmServer::SendCommand(String command)
{
    rmProtocol->SendPkg(command);
}

void RmServer::responseEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Log->Debug("responseEventHandler");
}

void RmServer::commandEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    CommandState *commandState = (CommandState *)event_data;
    String stateString;
    stateString = RmCommands::StateAsString(*commandState);
    Log->Append(F("State Ready: ")).Append(stateString).Info();
    rmServer->SendCommand(stateString);
}

//--------------------------------
RmServer *rmServer = NULL;
