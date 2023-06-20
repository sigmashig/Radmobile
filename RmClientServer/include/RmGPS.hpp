#pragma once
#include <Arduino.h>
#include <RmTypes.hpp>
#include <TinyGPS++.h>
#include <esp_event.h>


class RmGPS
{
public:
    RmGPS(int minDistance = 10);
    GpsPosition GetPosition();
    bool IsGpsAvailable() { return isGpsAvailable; }
    static String GpsAsString(GpsPosition position);
    static GpsPosition StringAsGps(String gpsString);

private:
    static TinyGPSPlus gps;
    int minDistance;
    GpsPosition lastPosition;
    bool isGpsAvailable = false;
    StaticTimer_t gpsTimer;
//    static void configEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    static void gpsTimerCallback(TimerHandle_t xTimer);
};
extern RmGPS *rmGPS;