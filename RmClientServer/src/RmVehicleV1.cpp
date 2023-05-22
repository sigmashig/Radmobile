#include "RmVehicleV1.hpp"
#include "RmConfiguration.hpp"
#include "RmEngineJY01.hpp"

RmVehicleV1::RmVehicleV1()
{
    if (rmConfig->Vehicle.frontLeft.controllerType == EngineControllerType::JY01)
    {
        frontLeft = new RmEngineJY01(rmConfig->Vehicle.frontLeft);
    }
    if (rmConfig->Vehicle.frontRight.controllerType == EngineControllerType::JY01)
    {
        frontRight = new RmEngineJY01(rmConfig->Vehicle.frontRight);
    }
    if (rmConfig->Vehicle.rearLeft.controllerType == EngineControllerType::JY01)
    {
        rearLeft = new RmEngineJY01(rmConfig->Vehicle.rearLeft);
    }
    if (rmConfig->Vehicle.rearRight.controllerType == EngineControllerType::JY01)
    {
        rearRight = new RmEngineJY01(rmConfig->Vehicle.rearRight);
    }
}

void RmVehicleV1::Begin()
{
}

void RmVehicleV1::turn(RmCommandPkg cmd)
{
    direction = ENGINE_FORWARD;
    power = cmd.value;
    frontLeft->Run(direction, ACTION_RUN, power);
    frontRight->Run(direction, ACTION_RUN, power);
    rearLeft->Run(direction, ACTION_RUN, power);
    rearRight->Run(direction, ACTION_RUN, power);
}

void RmVehicleV1::stop()
{
    direction = ENGINE_NODIRECTION;
    power = 0;
    frontLeft->Run(direction, ACTION_STOP, 0);
    frontRight->Run(direction, ACTION_STOP, 0);
    rearLeft->Run(direction, ACTION_STOP, 0);
    rearRight->Run(direction, ACTION_STOP, 0);
}

void RmVehicleV1::processButton(RmCommandPkg cmd)
{
    switch (cmd.command)
    {
    case CMD_BUTTON1:
        break;
    case CMD_BUTTON2:
        break;
    case CMD_BUTTON3:
        break;
    case CMD_BUTTON4:
        break;
    case CMD_BUTTON5:
        break;
    case CMD_BUTTON6:
        break;
    case CMD_BUTTON7:
        break;
    case CMD_BUTTON8:
        break;
    case CMD_BUTTON9:
        break;
    case CMD_BUTTON10:
        break;
    case CMD_BUTTON11:
        break;
    case CMD_BUTTON12:
        break;
    }
}

VehicleStatus RmVehicleV1::RunCmd(RmCommandPkg cmd)
{
    VehicleStatus status = VEHICLE_OK;
    switch (cmd.command)
    {
    case CMD_FORWARD:
        direction = ENGINE_FORWARD;
        // No break here
    case CMD_BACKWARD:
        direction = ENGINE_BACKWARD;
        power = cmd.value;
        frontLeft->Run(direction, ACTION_RUN, power);
        frontRight->Run(direction, ACTION_RUN, power);
        rearLeft->Run(direction, ACTION_RUN, power);
        rearRight->Run(direction, ACTION_RUN, power);
        break;
    case CMD_LEFT:
    case CMD_RIGHT:
        turn(cmd);
        break;
    case CMD_STOP:
        direction = ENGINE_NODIRECTION;
        power = 0;
        stop();
        break;
        case CMD_BUTTON1:
        case CMD_BUTTON2:
        case CMD_BUTTON3:
        case CMD_BUTTON4:
        case CMD_BUTTON5:
        case CMD_BUTTON6:
        case CMD_BUTTON7:
        case CMD_BUTTON8:
        case CMD_BUTTON9:
        case CMD_BUTTON10:
        case CMD_BUTTON11:
        case CMD_BUTTON12:
            processButton(cmd);
            break;

    }
    return status;
}
