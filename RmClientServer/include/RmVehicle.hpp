#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include "RmTypes.hpp"

ESP_EVENT_DECLARE_BASE(RMVEHICLE_EVENT);
typedef enum
{
    RMVEHICLE_ERROR
} RmVehicleEvent;

typedef enum
{
    VEHICLE_OK,
    VEHICLE_NOT_STARTED,
    VEHICLE_BAD_STATE
} VehicleStatus;

class RmVehicle
{
public:
    RmVehicle();
    virtual void Begin() = 0;
    virtual VehicleStatus ApplyState(CommandState &state) = 0;
    bool IsReady() { return isReady; };

protected:
    bool isReady = false;
    //int directionPower = 0;
    //int turnPower = 0;
    // int power = 0;
    //Direction direction = DIRECTION_NODIRECTION;
    //Direction turn = DIRECTION_NODIRECTION;
};

extern RmVehicle *rmVehicle;