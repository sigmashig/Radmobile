#pragma once
#include <Arduino.h>
#include "RmVehicle.hpp"

class RmVehicleV1 : public RmVehicle
{
public:
    RmVehicleV1();
    void Begin();
    VehicleStatus RunCmd(RmCommandPkg cmd);
};
