#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include "RmCommands.hpp"

ESP_EVENT_DECLARE_BASE(RMVEHICLE_EVENT);
typedef enum
{
    RMVEHICLE_ERROR
} RmVehicleEvent;

typedef enum
{
    VEHICLE_OK,
    VEHICLE_BAD_COMMAND
} VehicleStatus;

class RmVehicle
{
public:
    RmVehicle();
    virtual void Begin() = 0;
    virtual VehicleStatus RunCmd(RmCommandPkg cmd) = 0;
};

extern RmVehicle *rmVehicle;