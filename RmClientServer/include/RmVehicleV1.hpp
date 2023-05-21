#pragma once
#include <Arduino.h>
#include "RmVehicle.hpp"
#include "RmEngine.hpp"

class RmVehicleV1 : public RmVehicle
{
public:
    RmVehicleV1();
    void Begin();
    VehicleStatus RunCmd(RmCommandPkg cmd);

private:
    RmEngine *frontLeft;
    RmEngine *frontRight;
    RmEngine *rearLeft;
    RmEngine *rearRight;

    void turn(RmCommandPkg cmd);
    void stop();
    void processButton(RmCommandPkg cmd);
};
