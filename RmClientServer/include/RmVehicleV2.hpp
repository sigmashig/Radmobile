#pragma once
#include <Arduino.h>
#include "RmVehicle.hpp"
#include "RmEngine.hpp"
#include "RmRelay.hpp"

class RmVehicleV2 : public RmVehicle
{
public:
    RmVehicleV2();
    void Begin();
    VehicleStatus RunCmd(RmCommandPkg cmd);

private:
    bool isStarted = false;
    RmEngine *frontLeft;
    RmEngine *frontRight;
    RmEngine *rearLeft;
    RmEngine *rearRight;

    EngineDirection directionFL = ENGINE_NODIRECTION;
    EngineDirection directionFR = ENGINE_NODIRECTION;
    EngineDirection directionRL = ENGINE_NODIRECTION;
    EngineDirection directionRR = ENGINE_NODIRECTION;

    int powerFL = 0;
    int powerFR = 0;
    int powerRL = 0;
    int powerRR = 0;

    RmRelay *relay1;
    RmRelay *relay2;

    //void turn(RmCommandPkg cmd);
    void go(RmCommandPkg cmd);
    void stop();
    void processButton(RmCommandPkg cmd);
};
