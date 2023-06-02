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
private:
    static TinyGPSPlus gps;
    int minDistance;
    GpsPosition lastPosition;
    bool isGpsAvailable = false;
    static void configEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};
extern RmGPS *rmGPS;