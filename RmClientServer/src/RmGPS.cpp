#include "RmGPS.hpp"
#include <esp_event.h>
#include <SigmaLoger.hpp>
#include "RmConfiguration.hpp"

ESP_EVENT_DECLARE_BASE(RMCONFIG_EVENT);
ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

TinyGPSPlus RmGPS::gps;

RmGPS::RmGPS(int minDistance) : minDistance(minDistance)
{
    Serial1.begin(9600, SERIAL_8N1, rmConfig->gpsRx, rmConfig->gpsTx);
    gps = TinyGPSPlus();
    lastPosition = {0, 0};
    esp_event_handler_register(RMCONFIG_EVENT, RMCONFIG_EVENT_1_SECOND, configEventHandler, this);
}

GpsPosition RmGPS::GetPosition()
{
    GpsPosition currentPosition = {0, 0, 0, 0};
    if (gps.location.isValid())
    {
        currentPosition.latitude = gps.location.lat();
        currentPosition.longitude = gps.location.lng();
        currentPosition.altitude = gps.altitude.meters();
        currentPosition.speed = gps.speed.kmph();
    }
    return currentPosition;
}

void RmGPS::configEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

    static int cnt = 0;
    cnt++;
    if (cnt == 6 && !rmGPS->isGpsAvailable)
    {
        Log->Error("GPS not available");
    }
    else
    {
        if (Serial1.available() > 0)
        {
            rmGPS->isGpsAvailable = true;
            if (gps.encode(Serial1.read()))
            {
                double dist = gps.distanceBetween(gps.location.lat(), gps.location.lng(), rmGPS->lastPosition.latitude, rmGPS->lastPosition.longitude);
                if (dist >= rmGPS->minDistance)
                {
                    rmGPS->lastPosition.latitude = gps.location.lat();
                    rmGPS->lastPosition.longitude = gps.location.lng();
                    rmGPS->lastPosition.altitude = gps.altitude.meters();
                    rmGPS->lastPosition.speed = gps.speed.kmph();
                    Log->Debug("We have got a new GPS position");
                    esp_event_post(RMPROTOCOL_EVENT, RMEVENT_GPS_POSITION, &(rmGPS->lastPosition), sizeof(rmGPS->lastPosition), pdMS_TO_TICKS(1000));
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------
RmGPS *rmGPS;