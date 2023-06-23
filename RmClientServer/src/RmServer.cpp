#include "RMServer.hpp"
#include <SigmaIO.hpp>
#include "RmConfiguration.hpp"
#include "RmProtocol.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"
#include "SigmaLoger.hpp"
#if PROTOCOL == 1
#include "RmProtocolMqtt.hpp"
#elif PROTOCOL == 2
#include "RmProtocolLora.hpp"
#endif
#if RC == 1
#include "RmRcEmulator.hpp"
#elif RC == 2
#include "RmRcPS2.hpp"
#endif
#include "RmExchange.hpp"
#if LOGER == 1
#include "RmProtocolMqtt.hpp"
#endif

//RmProtocol *RmServer::logProtocol = NULL;
/*
void RmServer::startWiFi(String ssid, String password)
{
#if LOGER == 1
    Log->Append(F("Server. Connecting to WiFi network: ")).Append(ssid).Info();

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 {
                    Log->Append(F("Server. WiFi event: ")).Append(event).Debug();
                    switch (event)
                    {
                        case SYSTEM_EVENT_STA_GOT_IP:
                        {
                            Log->Append(F("WiFi connected. IP address:")).Append(WiFi.localIP().toString()).Info();
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
    Log->Debug("Point 9");
    WiFi.mode(WIFI_STA);
    Log->Debug("Point 10");
    WiFi.begin(ssid.c_str(), password.c_str());
    Log->Debug("Point 11");
    #endif
}
*/
void RmServer::startExchange()
{
    ulong lastOp = millis();
    Log->Debug(F("RmServer::startExchange()"));
    RmExchange rmExchange;
    rmExchange.Begin();
    IdConfig idConfig = rmExchange.GenerateId();
    while (millis() - lastOp < rmExchange.ExchangeDuration)
    {
        Log->Append(F("RmServer::startExchange().")).Append(F("SendId")).Debug();
        if (rmExchange.SendId())
        {
            // lastOp = millis();
            while (millis() - lastOp < rmExchange.ExchangeDuration && rmExchange.IsReadAvailable() == 0)
            {
                delay(100);
            }
            if (rmExchange.IsReadAvailable() > 0)
            {
                Log->Append(F("RmServer::startExchange().")).Append(F("ReceiveAck")).Debug();

                if (rmExchange.IsAckReceived)
                {
                    Log->Append(F("RmServer::startExchange().")).Append(F("AckReceived")).Debug();
                    if (rmExchange.IsAckGood)
                    {
                        Log->Append(F("RmServer::startExchange().")).Append(F("AckGood")).Debug();
                        break;
                    }
                    else
                    {
                        Log->Append(F("RmServer::startExchange().")).Append(F("AckBad")).Debug();
                        // continue; // repeat send id
                    }
                }
            }
        }
    }
    if (rmExchange.IsAckGood)
    {
        Log->Append(F("RmServer::startExchange().")).Append(F("SendEnd")).Debug();
        rmExchange.SendEnd();
        rmSession->SetSessionId(idConfig);
    }
}

RmServer::RmServer()
{
    Log = new SigmaLoger(512, log_publisher);

    Log->Debug(F("SERVER"));
    Log->Debug(F("Point 0.1"));
    // Init confguration
    rmConfig = new RmConfiguration();
    Log->Debug(F("Point 0"));
    rmConfig->BoardId = ESP.getEfuseMac();
    Log->Printf("ID:%lx", rmConfig->BoardId).Info();

#if LOGER == 1
    rmProtocolMqtt = new RmProtocolMqtt();
    rmProtocol = rmProtocolMqtt;
    logProtocol = rmProtocolMqtt;
#endif
    Log->Debug(F("Point 1"));
    sigmaIO = new SigmaIO(false);
    for (int i = 0; i < NUMBER_PORT_EXPANDERS; i++)
    {
        sigmaIO->RegisterPinDriver(rmConfig->portExpanders[i].drvCode, rmConfig->portExpanders[i].params,
                                   rmConfig->portExpanders[i].beg, rmConfig->portExpanders[i].end);
    }
    sigmaIO->Begin();
    Log->Debug(F("Point 2"));
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
    // isBeginRequired = false;
    rmProtocol = new RmProtocolMqtt();
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolLora();
    // isBeginRequired = true;
#endif

    // if (isBeginRequired)
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
    Log->Debug("commandEventHandler");
    CommandState *commandState = (CommandState *)event_data;
    if (commandState->special != 0)
    {
        Log->Append(F("Special command: ")).Append(commandState->special).Info();
        if (commandState->special == 1)
        {
            rmServer->startExchange();
        }
        return;
    }
    else
    {
        String stateString;
        stateString = RmCommands::StateAsString(*commandState);
        Log->Append(F("State Ready: ")).Append(stateString).Info();
        rmServer->SendCommand(stateString);
    }
}
void RmServer::log_publisher(SigmaLogLevel level, const char *msg)
{
    Serial.println(msg);

    if (rmServer != NULL && level > SigmaLogLevel::SIGMALOG_INTERNAL && rmServer->logProtocol != NULL && rmServer->logProtocol->IsReady())
    {
        rmServer->logProtocol->PublishLog(level, msg);
    }
}

//--------------------------------
RmServer *rmServer = NULL;
