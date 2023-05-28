#include "RmRemoteControl.hpp"

ESP_EVENT_DEFINE_BASE(RMRC_EVENT);

RmRemoteControl::RmRemoteControl()
{
}

void RmRemoteControl::CmdToServer(RmCommandPkg command)
{
    Serial.printf("RmRemoteControl::ReceivedCommand - %s\n", RmRemoteControl::CommandToString(command).c_str());
    bool isChanged = false;
    CommandState newState;
    switch (command.command)
    {
    case CMD_START:
    {
        isChanged = !(CurrentState.straight == DIRECTION_START || CurrentState.turn == DIRECTION_START);
        CurrentState.straight = DIRECTION_START;
        CurrentState.turn = DIRECTION_START;
        CurrentState.powerStraight = 0;
        CurrentState.powerTurn = 0;
        CurrentState.buttons.buttonPacked = 0x0000;
    }
    break;
    case CMD_FORWARD:
    {
        isChanged = !(CurrentState.straight == DIRECTION_FORWARD);
        CurrentState.straight = DIRECTION_FORWARD;
        CurrentState.powerStraight = command.value;
    }
    break;
    case CMD_BACKWARD:
    {
        isChanged = !(CurrentState.straight == DIRECTION_BACKWARD);
        CurrentState.straight = DIRECTION_BACKWARD;
        CurrentState.powerStraight = command.value;
    }
    break;
    case CMD_LEFT:
    {
        isChanged = !(CurrentState.turn == DIRECTION_LEFT);
        CurrentState.turn = (command.value == 0 ? DIRECTION_NODIRECTION : DIRECTION_LEFT);
        CurrentState.powerTurn = command.value;
    }
    break;
    case CMD_RIGHT:
    {
        isChanged = !(CurrentState.turn == DIRECTION_RIGHT);
        CurrentState.turn = (command.value == 0 ? DIRECTION_NODIRECTION : DIRECTION_RIGHT);
        CurrentState.powerTurn = command.value;
    }
    break;
    case CMD_STOP:
    {
        isChanged = !(CurrentState.straight == DIRECTION_STOP || CurrentState.turn == DIRECTION_STOP);
        CurrentState.straight = DIRECTION_STOP;
        CurrentState.turn = DIRECTION_STOP;
        CurrentState.powerStraight = 0;
        CurrentState.powerTurn = 0;
        CurrentState.buttons.buttonPacked = 0x0000;
    }
    break;
    case CMD_PAUSE:
    {
        isChanged = !(CurrentState.straight == DIRECTION_NODIRECTION);
        CurrentState.straight = DIRECTION_NODIRECTION;
        CurrentState.powerStraight = 0;
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
        byte n = command.value - CMD_BUTTON1;
        byte v = command.value == 0 ? 0 : 1;
        uint16_t mask = 1 << n;
        byte prevValue = (CurrentState.buttons.buttonPacked & mask) == 0 ? 0 : 1;
        isChanged = !(v == prevValue);
        CurrentState.buttons.buttonPacked |= mask;
    }
    break;
    }
    if (isChanged)
    {
        esp_event_post(RMRC_EVENT, RMRC_NEWSTATE, &CurrentState, sizeof(CurrentState), portMAX_DELAY);
    }
}

//---------------------------------------------------------------------------------------------
RmRemoteControl *remoteControl;