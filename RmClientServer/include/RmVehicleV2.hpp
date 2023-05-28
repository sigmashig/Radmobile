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
    VehicleStatus ApplyState(CommandState& state);

private:
    bool isStarted = false;
    RmEngine *frontLeft;
    RmEngine *frontRight;
    RmEngine *rearLeft;
    RmEngine *rearRight;

    Direction directionFL = DIRECTION_NODIRECTION;
    Direction directionFR = DIRECTION_NODIRECTION;
    Direction directionRL = DIRECTION_NODIRECTION;
    Direction directionRR = DIRECTION_NODIRECTION;

    int powerFL = 0;
    int powerFR = 0;
    int powerRL = 0;
    int powerRR = 0;

    RmRelay *relay1;
    RmRelay *relay2;

    //void turn(RmCommandPkg cmd);
    void go(Direction dirStraight, int powerStraight, Direction dirTurn, int powerTurn);
    void buttons(ButtonsSet buttons);

};
