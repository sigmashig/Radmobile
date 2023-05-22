#pragma once
#include <Arduino.h>
#include "RmVehicle.hpp"
#include "RmEngine.hpp"
#include "RmRelay.hpp"

class RmVehicleV1 : public RmVehicle
{
public:
    RmVehicleV1();
    void Begin();
    VehicleStatus RunCmd(RmCommandPkg cmd);

private:
    bool isStarted = false;
    RmEngine *frontLeft;
    RmEngine *frontRight;
    RmEngine *rearLeft;
    RmEngine *rearRight;

    RmRelay *relay1;
    RmRelay *relay2;

    void turn(RmCommandPkg cmd);
    void go(RmCommandPkg cmd);
    void stop();
    void processButton(RmCommandPkg cmd);
};
