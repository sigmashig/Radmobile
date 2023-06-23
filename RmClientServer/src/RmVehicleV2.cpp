#include "RmVehicleV2.hpp"
#include "SigmaLoger.hpp"
#include "RmConfiguration.hpp"
#include "RmEngineYellow.hpp"
#include "RmEngineQS909.hpp"

RmVehicleV2::RmVehicleV2()
{
    if (rmConfig->Vehicle.frontLeft.controllerType == EngineControllerType::L298N)
    {
        frontLeft = new RmEngineYellow(rmConfig->Vehicle.frontLeft);
    }
    else if (rmConfig->Vehicle.frontLeft.controllerType == EngineControllerType::QS909)
    {
        frontLeft = new RmEngineQS909(rmConfig->Vehicle.frontLeft);
    }
    if (rmConfig->Vehicle.frontRight.controllerType == EngineControllerType::L298N)
    {
        frontRight = new RmEngineYellow(rmConfig->Vehicle.frontRight);
    }
    else if (rmConfig->Vehicle.frontRight.controllerType == EngineControllerType::QS909)
    {
        frontRight = new RmEngineQS909(rmConfig->Vehicle.frontRight);
    }
    if (rmConfig->Vehicle.rearLeft.controllerType == EngineControllerType::L298N)
    {
        rearLeft = new RmEngineYellow(rmConfig->Vehicle.rearLeft);
    }
    else if (rmConfig->Vehicle.rearLeft.controllerType == EngineControllerType::QS909)
    {
        rearLeft = new RmEngineQS909(rmConfig->Vehicle.rearLeft);
    }
    if (rmConfig->Vehicle.rearRight.controllerType == EngineControllerType::L298N)
    {
        rearRight = new RmEngineYellow(rmConfig->Vehicle.rearRight);
    }
    else if (rmConfig->Vehicle.rearRight.controllerType == EngineControllerType::QS909)
    {
        rearRight = new RmEngineQS909(rmConfig->Vehicle.rearRight);
    }

    // relay1 = new RmRelay(rmConfig->Vehicle.r1);
    // relay2 = new RmRelay(rmConfig->Vehicle.r2);
}

void RmVehicleV2::Begin()
{
    Log->Debug(F("RmVehicleV2::Begin"));
    frontLeft->Begin();
    Log->Debug(F("RmVehicleV2::Begin 1"));
    frontRight->Begin();
    rearLeft->Begin();
    rearRight->Begin();
    // relay1->Begin();
    // relay2->Begin();
}
void RmVehicleV2::buttons(ButtonsSet buttons)
{
    if (buttons.button.b1)
    {
        // relay1->On();
    }
    else
    {
        // relay1->Off();
    }
    if (buttons.button.b2)
    {
        // relay2->On();
    }
    else
    {
        // relay2->Off();
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

VehicleStatus RmVehicleV2::ApplyCorrection(Direction direction, int power)
{
    Log->Printf("RmVehicleV2::ApplyCorrection: %d, %d\n", direction, power).Debug();
    double turnPower = 0;
    double straightPower = powerFL;
    Direction dirStraight = directionFL;

    if (power < 200)
    {
        turnPower = 30;
    }
    else if (power < 300)
    {
        turnPower = 50;
    }
    else if (power < 400)
    {
        turnPower = 70;
    }
    else
    {
        turnPower = 100;
    }
    go(dirStraight, straightPower, direction, turnPower);

    return VEHICLE_OK;
}

void RmVehicleV2::go(Direction dirStraight, int powerStraight, Direction dirTurn, int powerTurn)
{
    Log->Printf("RmVehicleV2::go: %d, %d, %d, %d\n", dirStraight, powerStraight, dirTurn, powerTurn).Debug();
    int prevPowerFL = powerFL;
    int prevPowerFR = powerFR;
    int prevPowerRL = powerRL;
    int prevPowerRR = powerRR;

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
    if (powerFL - prevPowerFL > rmConfig->Vehicle.limitSlowRun)
    {
        powerFL = prevPowerFL + rmConfig->Vehicle.limitSlowRun;
    }
    if (powerFR - prevPowerFR > rmConfig->Vehicle.limitSlowRun)
    {
        powerFR = prevPowerFR + rmConfig->Vehicle.limitSlowRun;
    }
    if (powerRL - prevPowerRL > rmConfig->Vehicle.limitSlowRun)
    {
        powerRL = prevPowerRL + rmConfig->Vehicle.limitSlowRun;
    }
    if (powerRR - prevPowerRR > rmConfig->Vehicle.limitSlowRun)
    {
        powerRR = prevPowerRR + rmConfig->Vehicle.limitSlowRun;
    }
    Log->Printf("RmVehicleV2::power: %d, %d, %d, %d\n", powerFL, powerFR, powerRL, powerRR).Debug();
    Log->Printf("RmVehicleV2::dir: %d, %d, %d, %d\n", directionFL, directionFR, directionRL, directionRR).Debug();
    frontLeft->Run(directionFL, ACTION_RUN, powerFL);
    frontRight->Run(directionFR, ACTION_RUN, powerFR);
    rearLeft->Run(directionRL, ACTION_RUN, powerRL);
    rearRight->Run(directionRR, ACTION_RUN, powerRR);
}
