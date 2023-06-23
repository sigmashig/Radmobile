#include "RmClient.hpp"
#include "SigmaLoger.hpp"
#include "SigmaIO.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"
#include "RmProtocol.hpp"
#include "RmPID.hpp"
#include "RmTelemetry.hpp"
#include "RmGPS.hpp"
#if LOGER == 1
#include "RmProtocolMqtt.hpp"
#endif

//#if WIFI==1
//#include <WiFi.h>
//#endif
#if PROTOCOL == 1
#include "RmProtocolMqtt.hpp"
#elif PROTOCOL == 2
#include "RmProtocolLora.hpp"
#endif
#include "RmVehicle.hpp"
#if VEHICLE == 1
#include "RmVehicleV2.hpp"
#elif VEHICLE == 2
#include "RmVehicleV2.hpp"
#endif

//RmProtocol *RmClient::logProtocol = NULL;

RmClient::RmClient()
{
    Log = new SigmaLoger(512, log_publisher);

    Log->Debug(F("CLIENT"));
    // Init confguration
    rmConfig = new RmConfiguration();
    rmConfig->BoardId = ESP.getEfuseMac();
    Log->Printf("ID:%lx", rmConfig->BoardId).Info();

#if LOGER == 1
    rmProtocolMqtt = new RmProtocolMqtt();
    rmProtocol = rmProtocolMqtt;
    logProtocol = rmProtocolMqtt;
#endif

    // TODO: session should be transferred from server to client
    sigmaIO = new SigmaIO(false);
    for (int i = 0; i < NUMBER_PORT_EXPANDERS; i++)
    {
        sigmaIO->RegisterPinDriver(rmConfig->portExpanders[i].drvCode, rmConfig->portExpanders[i].params,
                                   rmConfig->portExpanders[i].beg, rmConfig->portExpanders[i].end);
    }
    sigmaIO->Begin();

    rmCommands = new RmCommands();
    rmSession = new RmSession();
//#if WIFI == 1
//    WiFi.mode(WIFI_STA);
//    startWiFi(WIFI_SSID, WIFI_PWD);
//#endif

#if PROTOCOL == 1
    isBeginRequired = false;
    rmProtocol = new RmProtocolMqtt();
//    startWiFi(WIFI_SSID, WIFI_PWD);
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolLora();
    isBeginRequired = true;
#endif
    Log->Debug("Point 10");
#if VEHICLE == 1
// It seems, like no diffrence between V1 and V2. The V1 is obsolete
    rmVehicle = new RmVehicleV2();
#elif VEHICLE == 2
    rmVehicle = new RmVehicleV2();
#endif
    Log->Debug("Point 11");
    rmPID = new RmPID(rmConfig->pidSettings.limitPitch, rmConfig->pidSettings.limitRoll, rmConfig->pidSettings.limitYaw,
                      rmConfig->pidSettings.straightPeriod, rmConfig->pidSettings.checkPeriod);
    rmGPS = new RmGPS(10);
    rmTelemetry = new RmTelemetry();

    //if (isBeginRequired)
    {
        Begin();
    }
}

void RmClient::Begin()
{
    Log->Debug(F("RmClient::Begin()"));
    rmProtocol->Begin();
    Log->Debug("Point 12");
    rmVehicle->Begin();
    Log->Debug("Point 13");
    rmTelemetry->Begin();
    Log->Debug("Point 14");
}
/*
void RmClient::startWiFi(String ssid, String password)
{
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 { 
                    switch (event)
                    {
                        case SYSTEM_EVENT_STA_GOT_IP:
                        {
                            Log->Info(F("WiFi connected"));
                            Log->Append("IP address: ").Append(WiFi.localIP().toString()).Info();
                            //Begin();
                            break;
                        }
                        case SYSTEM_EVENT_STA_DISCONNECTED:
                        {
                            Log->Append(F("WiFi connection error: ")).Append(info.wifi_sta_disconnected.reason).Error();
                            if (isConnected)
                            {
                                rmProtocol->Reconnect();
                            }
                        }
                        break;
                    } });
    if (WiFi.isConnected())
    {
        Log->Info(F("WiFi already connected"));
        Begin();
        return;
    }
    else
    {
        Log->Info(F("WiFi not connected"));

        Log->Append(F("Connecting to WiFi network: ")).Append(ssid).Info();
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
    }
}
*/
void RmClient::log_publisher(SigmaLogLevel level, const char *msg)
{
    Serial.println(msg);

    if (rmClient != NULL && level > SigmaLogLevel::SIGMALOG_INTERNAL && rmClient->logProtocol != NULL && rmClient->logProtocol->IsReady())
    {
        rmClient->logProtocol->PublishLog(level, msg);
    }
}

//--------------------------------
RmClient *rmClient = NULL;
