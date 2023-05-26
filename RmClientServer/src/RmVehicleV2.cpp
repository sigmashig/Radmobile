#include "RmVehicleV2.hpp"
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
    Serial.println("RmVehicleV2::Begin()");
    frontLeft->Begin();
    frontRight->Begin();
    rearLeft->Begin();
    rearRight->Begin();
    relay1->Begin();
    relay2->Begin();
    Serial.println("RmVehicleV2::Begin() - Done");
}

void RmVehicleV2::turn(RmCommandPkg cmd)
{
    direction = ENGINE_FORWARD;
    power = cmd.value;
    if (cmd.command == CMD_LEFT)
    {
        frontLeft->Run(ENGINE_NODIRECTION, ACTION_STOP, 0);
        frontRight->Run(direction, ACTION_RUN, power);
        rearLeft->Run(ENGINE_NODIRECTION, ACTION_STOP, 0);
        rearRight->Run(direction, ACTION_RUN, power);
    }
    else if (cmd.command == CMD_RIGHT)
    {
        frontLeft->Run(direction, ACTION_RUN, power);
        frontRight->Run(ENGINE_NODIRECTION, ACTION_STOP, 0);
        rearLeft->Run(direction, ACTION_RUN, power);
        rearRight->Run(ENGINE_NODIRECTION, ACTION_STOP, 0);
    }
}

void RmVehicleV2::go(RmCommandPkg cmd)
{
    direction = cmd.command == CMD_FORWARD ? ENGINE_FORWARD : ENGINE_BACKWARD;
    power = cmd.value;
    frontLeft->Run(direction, ACTION_RUN, power);
    frontRight->Run(direction, ACTION_RUN, power);
    rearLeft->Run(direction, ACTION_RUN, power);
    rearRight->Run(direction, ACTION_RUN, power);
}

void RmVehicleV2::stop()
{
    direction = ENGINE_NODIRECTION;
    power = 0;
    frontLeft->Run(direction, ACTION_STOP, 0);
    frontRight->Run(direction, ACTION_STOP, 0);
    rearLeft->Run(direction, ACTION_STOP, 0);
    rearRight->Run(direction, ACTION_STOP, 0);
}

void RmVehicleV2::processButton(RmCommandPkg cmd)
{
    switch (cmd.command)
    {
    case CMD_BUTTON1:
    {
        if (cmd.value == 0)
        {
            relay1->Off();
        }
        else
        {
            relay1->On();
        }
    }
    break;

    case CMD_BUTTON2:
    {
        if (cmd.value == 0)
        {
            relay2->Off();
        }
        else
        {
            relay2->On();
        }
    }
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

VehicleStatus RmVehicleV2::RunCmd(RmCommandPkg cmd)
{
    VehicleStatus status = VEHICLE_OK;
    if (!isStarted)
    {
        if (cmd.command == CMD_START)
        {
            isStarted = true;
        }
        else
        { // skip command
            return VEHICLE_NOT_STARTED;
        }
    }
    switch (cmd.command)
    {
    case CMD_FORWARD:
        direction = ENGINE_FORWARD;
        // No break here
    case CMD_BACKWARD:
        direction = ENGINE_BACKWARD;
        go(cmd);
        break;
    case CMD_LEFT:
    case CMD_RIGHT:
        turn(cmd);
        break;
    case CMD_STOP:
        direction = ENGINE_NODIRECTION;
        power = 0;
        stop();
        isStarted = false;
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
    default:
        status = VEHICLE_BAD_COMMAND;
        break;
    }
    return status;
}
