#include "RmVehicleV2.hpp"
#include "SigmaLoger.hpp"
#include "RmConfiguration.hpp"
#include "RmEngineYellow.hpp"

RmVehicleV2::RmVehicleV2()
{
    if (rmConfig->Vehicle.frontLeft.controllerType == EngineControllerType::L298N)
    {
        frontLeft = new RmEngineYellow(rmConfig->Vehicle.frontLeft);
    }
    if (rmConfig->Vehicle.frontRight.controllerType == EngineControllerType::L298N)
    {
        frontRight = new RmEngineYellow(rmConfig->Vehicle.frontRight);
    }
    if (rmConfig->Vehicle.rearLeft.controllerType == EngineControllerType::L298N)
    {
        rearLeft = new RmEngineYellow(rmConfig->Vehicle.rearLeft);
    }
    if (rmConfig->Vehicle.rearRight.controllerType == EngineControllerType::L298N)
    {
        rearRight = new RmEngineYellow(rmConfig->Vehicle.rearRight);
    }

    relay1 = new RmRelay(rmConfig->Vehicle.r1);
    relay2 = new RmRelay(rmConfig->Vehicle.r2);
}

void RmVehicleV2::Begin()
{
    frontLeft->Begin();
    frontRight->Begin();
    rearLeft->Begin();
    rearRight->Begin();
    relay1->Begin();
    relay2->Begin();
}
void RmVehicleV2::buttons(ButtonsSet buttons)
{
    if (buttons.button.b1)
    {
        relay1->On();
    }
    else
    {
        relay1->Off();
    }
    if (buttons.button.b2)
    {
        relay2->On();
    }
    else
    {
        relay2->Off();
    }
}

VehicleStatus RmVehicleV2::ApplyState(CommandState &state)
{
    VehicleStatus status = VEHICLE_OK;
    Log->Debug(F("RmVehicleV2::RunCmd"));
    if (!isStarted)
    {
        if (state.straight == DIRECTION_START)
        {
            Log->Info(F("Vehicle V2 started"));
            isStarted = true;
            return VEHICLE_OK;
        }
        else
        { // skip command
            Log->Warn(F("Vehicle V2 DO NOT started"));
            return VEHICLE_NOT_STARTED;
        }
    }
    if (state.straight == DIRECTION_STOP)
    {
        Log->Info(F("Vehicle V2 stoped"));
        isStarted = false;
        return VEHICLE_OK;
    }
    go(state.straight, state.powerStraight, state.turn, state.powerTurn);
    buttons(state.buttons);
    return status;
}

void RmVehicleV2::go(Direction dirStraight, int powerStraight, Direction dirTurn, int powerTurn)
{
    Log->Printf("RmVehicleV2::go: %d, %d, %d, %d\n", dirStraight, powerStraight, dirTurn, powerTurn).Debug();
    if (dirStraight != DIRECTION_FORWARD && dirStraight != DIRECTION_BACKWARD)
    {
        dirStraight = DIRECTION_FORWARD;
    }
    directionFL = dirStraight;
    directionFR = dirStraight;
    directionRL = dirStraight;
    directionRR = dirStraight;

    if (powerTurn > 0)
    {
        if (dirTurn == DIRECTION_LEFT)
        {
            powerFL = powerStraight - powerTurn / 2;
            powerFR = powerStraight + powerTurn / 2;
            powerRL = powerStraight - powerTurn / 2;
            powerRR = powerStraight + powerTurn / 2;
        }
        else if (dirTurn == DIRECTION_RIGHT)
        {
            powerFL = powerStraight + powerTurn / 2;
            powerFR = powerStraight - powerTurn / 2;
            powerRL = powerStraight + powerTurn / 2;
            powerRR = powerStraight - powerTurn / 2;
        }
    }
    else
    {
        powerFL = powerStraight;
        powerFR = powerStraight;
        powerRL = powerStraight;
        powerRR = powerStraight;
    }
    if (powerFL < 0)
    {
        powerFL = -powerFL;
        directionFL = (directionFL == DIRECTION_FORWARD) ? DIRECTION_BACKWARD : DIRECTION_FORWARD;
    }
    if (powerFR < 0)
    {
        powerFR = -powerFR;
        directionFR = (directionFR == DIRECTION_FORWARD) ? DIRECTION_BACKWARD : DIRECTION_FORWARD;
    }
    if (powerRL < 0)
    {
        powerRL = -powerRL;
        directionRL = (directionRL == DIRECTION_FORWARD) ? DIRECTION_BACKWARD : DIRECTION_FORWARD;
    }
    if (powerRR < 0)
    {
        powerRR = -powerRR;
        directionRR = (directionRR == DIRECTION_FORWARD) ? DIRECTION_BACKWARD : DIRECTION_FORWARD;
    }
    Log->Printf("RmVehicleV2::power: %d, %d, %d, %d\n", powerFL, powerFR, powerRL, powerRR).Debug();
    Log->Printf("RmVehicleV2::dir: %d, %d, %d, %d\n", directionFL, directionFR, directionRL, directionRR).Debug();
    frontLeft->Run(directionFL, ACTION_RUN, powerFL);
    frontRight->Run(directionFR, ACTION_RUN, powerFR);
    rearLeft->Run(directionRL, ACTION_RUN, powerRL);
    rearRight->Run(directionRR, ACTION_RUN, powerRR);
}
