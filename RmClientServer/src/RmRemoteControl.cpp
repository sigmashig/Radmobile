#include "RmRemoteControl.hpp"
#include "SigmaLoger.hpp"
#include "RmCommands.hpp"

ESP_EVENT_DEFINE_BASE(RMRC_EVENT);

RmRemoteControl::RmRemoteControl()
{
    CurrentState.isValid = false;
    CurrentState.straight = DIRECTION_NODIRECTION;
    CurrentState.turn = DIRECTION_NODIRECTION;
    CurrentState.powerStraight = 0;
    CurrentState.powerTurn = 0;
    CurrentState.buttons.buttonPacked = 0x0000;
}

void RmRemoteControl::CmdToServer(RmCommandPkg command)
{
    Log->Append("RmRemoteControl::Received command: ").Append(RmCommands::CommandToString(command)).Debug();
    CommandState oldState;
    memcpy(&oldState, &CurrentState, sizeof(CommandState));
    CurrentState.isValid = false;
    switch (command.command)
    {
    case CMD_START:
    {
        CurrentState.straight = DIRECTION_START;
        CurrentState.turn = DIRECTION_START;
        CurrentState.powerStraight = 0;
        CurrentState.powerTurn = 0;
        CurrentState.buttons.buttonPacked = 0x0000;
        CurrentState.isValid = true;
    }
    break;
    case CMD_FORWARD:
    {
        CurrentState.straight = DIRECTION_FORWARD;
        CurrentState.powerStraight = command.value;
        CurrentState.isValid = true;
    }
    break;
    case CMD_BACKWARD:
    {
        CurrentState.straight = DIRECTION_BACKWARD;
        CurrentState.powerStraight = command.value;
        CurrentState.isValid = true;
    }
    break;
    case CMD_LEFT:
    {
        CurrentState.turn = (command.value == 0 ? DIRECTION_NODIRECTION : DIRECTION_LEFT);
        CurrentState.powerTurn = command.value;
        CurrentState.isValid = true;
    }
    break;
    case CMD_RIGHT:
    {
        CurrentState.turn = (command.value == 0 ? DIRECTION_NODIRECTION : DIRECTION_RIGHT);
        CurrentState.powerTurn = command.value;
        CurrentState.isValid = true;
    }
    break;
    case CMD_STOP:
    {
        CurrentState.straight = DIRECTION_STOP;
        CurrentState.turn = DIRECTION_STOP;
        CurrentState.powerStraight = 0;
        CurrentState.powerTurn = 0;
        CurrentState.buttons.buttonPacked = 0x0000;
        CurrentState.isValid = true;
    }
    break;
    case CMD_PAUSE:
    {
        CurrentState.straight = DIRECTION_NODIRECTION;
        CurrentState.powerStraight = 0;
        CurrentState.isValid = true;
    }
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
    case CMD_BUTTON13:
    case CMD_BUTTON14:
    case CMD_BUTTON15:
    case CMD_BUTTON16:
    {
        CurrentState.isValid = true;
        byte n = command.command - CMD_BUTTON1;
        byte v = command.value == 0 ? 0 : 1;
        uint16_t mask = 1 << n;
        if (v == 0)
        {
            CurrentState.buttons.buttonPacked &= ~mask;
        }
        else
        {
            CurrentState.buttons.buttonPacked |= mask;
        }
    }
    break;
    case CMD_SPECIAL1:
    case CMD_SPECIAL2:
    case CMD_SPECIAL3:
    {
        Log->Append("RmRemoteControl::Received special command: ").Debug();
        CurrentState.isValid = false;
        CurrentState.buttons.buttonPacked = 0x0000;
        CurrentState.special = command.command - CMD_SPECIAL1 + 1;
        break;
    }
    }
    if (memcmp(&oldState, &CurrentState, sizeof(CurrentState)) != 0)
    {
        Log->Append("RmRemoteControl::New state: ").Append(RmCommands::StateAsString(CurrentState)).Debug();
        auto err = esp_event_post(RMRC_EVENT, RMRC_NEWSTATE, &CurrentState, sizeof(CurrentState), portMAX_DELAY);
        if (err!=ESP_OK)
        {
            Log->Append("RmRemoteControl::Error posting event: ").Append(err).Error();
        }
    }
}

//---------------------------------------------------------------------------------------------
RmRemoteControl *remoteControl;