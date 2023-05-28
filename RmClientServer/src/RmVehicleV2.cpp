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
/*
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
*/

void RmVehicleV2::go(RmCommandPkg cmd)
{
    Serial.printf("RmVehicleV2::go: %d, %d\n", cmd.command, cmd.value);
    switch (cmd.command)
    {
    case CMD_FORWARD:
        direction = ENGINE_FORWARD;
        directionPower = cmd.value;
        break;
    case CMD_BACKWARD:
        direction = ENGINE_BACKWARD;
        directionPower = cmd.value;
        break;
    case CMD_PAUSE:
        direction = ENGINE_NODIRECTION;
        directionPower = 0;
        break;
    case CMD_LEFT:
        turn = ENGINE_LEFT;
        turnPower = cmd.value;
        break;
    case CMD_RIGHT:
        turn = ENGINE_RIGHT;
        turnPower = cmd.value;
        break;
    case CMD_STOP:
        turn = ENGINE_NODIRECTION;
        turnPower = 0;
        direction = ENGINE_NODIRECTION;
        directionPower = 0;
        break;
    }
    if (turnPower != 0)
    {
        if (turn == ENGINE_LEFT)
        {
            powerFL = directionPower - turnPower;
            if (powerFL < 0)
            {
                powerFL = -powerFL;
                directionFL = directionFL == ENGINE_FORWARD ? ENGINE_BACKWARD : ENGINE_FORWARD;
            }
            powerRL = powerFL;
            directionRL = directionFL;
        }
        else if (turn == ENGINE_RIGHT)
        {
            powerFR = directionPower - turnPower;
            if (powerFR < 0)
            {
                powerFR = -powerFR;
                directionFR = directionFR == ENGINE_FORWARD ? ENGINE_BACKWARD : ENGINE_FORWARD;
            }
            powerRR = powerFR;
            directionRR = directionFR;
        }
    }
    else
    {
        // end of turn
        turn = ENGINE_NODIRECTION;
        //turnPower = 0;
        powerFL = directionPower;
        powerFR = directionPower;
        powerRL = directionPower;
        powerRR = directionPower;
        directionFL = direction;
        directionFR = direction;
        directionRL = direction;
        directionRR = direction;
    }
    Serial.printf("RmVehicleV2::global: %d, %d, %d, %d\n", direction, directionPower, turn, turnPower);
    Serial.printf("RmVehicleV2::power: %d, %d, %d, %d\n", powerFL, powerFR, powerRL, powerRR);
    Serial.printf("RmVehicleV2::dir: %d, %d, %d, %d\n", directionFL, directionFR, directionRL, directionRR);
    frontLeft->Run(directionFL, ACTION_RUN, powerFL);
    frontRight->Run(directionFR, ACTION_RUN, powerFR);
    rearLeft->Run(directionRL, ACTION_RUN, powerRL);
    rearRight->Run(directionRR, ACTION_RUN, powerRR);
}
/*
void RmVehicleV2::stop()
{
    direction = ENGINE_NODIRECTION;
    power = 0;
    frontLeft->Run(direction, ACTION_STOP, 0);
    frontRight->Run(direction, ACTION_STOP, 0);
    rearLeft->Run(direction, ACTION_STOP, 0);
    rearRight->Run(direction, ACTION_STOP, 0);
}
*/
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
    /*
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
        */
    }
    
}

VehicleStatus RmVehicleV2::RunCmd(RmCommandPkg cmd)
{
    VehicleStatus status = VEHICLE_OK;
    Serial.println("RmVehicleV2::RunCmd");
    if (!isStarted)
    {
        if (cmd.command == CMD_START)
        {
            Serial.println("RmVehicleV2::RunCmd - Start");
            isStarted = true;
            return VEHICLE_OK;
        }
        else
        { // skip command
            Serial.println("RmVehicleV2::RunCmd - Not started");
            return VEHICLE_NOT_STARTED;
        }
    }
    switch (cmd.command)
    {
    case CMD_FORWARD:
    case CMD_PAUSE:
    case CMD_BACKWARD:
    case CMD_LEFT:
    case CMD_RIGHT:
        go(cmd);
        break;
    case CMD_STOP:
        isStarted = false;
        go(cmd);
        break;
    case CMD_BUTTON1:
    case CMD_BUTTON2:
        processButton(cmd);
        break;
    default:
        status = VEHICLE_BAD_COMMAND;
        break;
    }
    return status;
}
