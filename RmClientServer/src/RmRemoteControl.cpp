#include "RmRemoteControl.hpp"

ESP_EVENT_DEFINE_BASE(RMRC_EVENT);

String RmRemoteControl::cmdTxt[CMD_NOCOMMAND + 1];
RmRemoteControl::RmRemoteControl()
{

    cmdTxt[CMD_FORWARD] = "F1";
    cmdTxt[CMD_BACKWARD] = "B1";
    cmdTxt[CMD_LEFT] = "L1";
    cmdTxt[CMD_RIGHT] = "R1";
    cmdTxt[CMD_STOP] = "XX";
    cmdTxt[CMD_BUTTON1] = "T1";
    cmdTxt[CMD_BUTTON2] = "T2";
    cmdTxt[CMD_BUTTON3] = "T3";
    cmdTxt[CMD_BUTTON4] = "T4";
    cmdTxt[CMD_BUTTON5] = "T5";
    cmdTxt[CMD_BUTTON6] = "T6";
    cmdTxt[CMD_BUTTON7] = "T7";
    cmdTxt[CMD_BUTTON8] = "T8";
    cmdTxt[CMD_BUTTON9] = "T9";
    cmdTxt[CMD_BUTTON10] = "TA";
    cmdTxt[CMD_BUTTON11] = "TB";
    cmdTxt[CMD_BUTTON12] = "TC";
    cmdTxt[CMD_NOCOMMAND] = "YY";
}

void RmRemoteControl::ReceivedCommand(CommandPkg command)
{
    Serial.println("RmRemoteControl::ReceivedCommand");
    esp_event_post(RMRC_EVENT, RMRC_CMD, &command, sizeof(command), portMAX_DELAY);
}

void RmRemoteControl::CommandToString(CommandPkg command, String &commandString)
{
    // command string format: "{L2#546}" means: command=CMD_J2_LEFT, value=546
    commandString = "{";
    commandString += cmdTxt[command.command];
    commandString += "#";
    commandString += command.value;
    commandString += "}";
}
//---------------------------------------------------------------------------------------------
RmRemoteControl *remoteControl;