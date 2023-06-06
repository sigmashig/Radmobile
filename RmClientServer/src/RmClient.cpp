#include "RmClient.hpp"
#include "SigmaLoger.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"
#include "RmProtocol.hpp"
#include "RmPID.hpp"
#include "RmTelemetry.hpp"
#include "RmGPS.hpp"

#if PROTOCOL == 1
#include <WiFi.h>
#include "RmProtocolMqtt.hpp"
#elif PROTOCOL == 2
#include "RmProtocolLora.hpp"
#endif
#include "RmVehicle.hpp"
#if VEHICLE == 1
#include "RmVehicleV1.hpp"
#elif VEHICLE == 2
#include "RmVehicleV2.hpp"
#endif

RmClient::RmClient()
{
    // TODO: session should be transferred from server to client
    rmCommands = new RmCommands();
    rmSession = new RmSession();

    rmPinsDriver = new RmPinsDriver(rmConfig->clientPcfs, NUMB_OF_CLIENT_PCF);
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
#elif VEHICLE == 2
    rmVehicle = new RmVehicleV2();
#endif
    rmPID = new RmPID(rmConfig->limitPitch, rmConfig->limitRoll, rmConfig->limitYaw, rmConfig->straightPeriod, rmConfig->checkPeriod);
    rmGPS = new RmGPS(10);
    rmTelemetry = new RmTelemetry();

    if (isBeginRequired)
    {
        Begin();
    }
}

void RmClient::Begin()
{
    Log->Debug(F("RmClient::Begin()"));
    rmPinsDriver->Begin();
    rmProtocol->Begin();
    rmVehicle->Begin();
    rmTelemetry->Begin();
}

void RmClient::startWiFi(String ssid, String password)
{
#if PROTOCOL == 1

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 { 
                    switch (event)
                    {
                        case SYSTEM_EVENT_STA_GOT_IP:
                        {
                            Log->Info(F("WiFi connected"));
                            Log->Append(F("IP address: ").Append(WiFi.localIP().toString()).Info());
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
#endif
}

//--------------------------------
RmClient *rmClient = NULL;
