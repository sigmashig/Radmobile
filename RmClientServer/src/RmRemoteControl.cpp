#include "RmRemoteControl.hpp"

ESP_EVENT_DEFINE_BASE(RMRC_EVENT);

String RmRemoteControl::cmdTxt[CMD_NOCOMMAND+1] = {
    "L1", // CMD_J1_LEFT,
    "R1", // CMD_J1_RIGHT,
    "F1", // CMD_J1_FORWARD,
    "B1", // CMD_J1_BACKWARD,
    "L2", // CMD_J2_LEFT,
    "R2", // CMD_J2_RIGHT,
    "F2", // CMD_J2_FORWARD,
    "B2", // CMD_J2_BACKWARD,
    "L3", // CMD_PAD1_LEFT,
    "R3", // CMD_PAD1_RIGHT,
    "F3", // CMD_PAD1_FORWARD,
    "B3", // CMD_PAD1_BACKWARD,
    "L4", // CMD_PAD2_LEFT,
    "R4", // CMD_PAD2_RIGHT,
    "F4", // CMD_PAD2_FORWARD,
    "B4", // CMD_PAD2_BACKWARD,
    "T1", // CMD_BUTTON1,
    "T2", // CMD_BUTTON2,
    "T3", // CMD_BUTTON3,
    "T4", // CMD_BUTTON4,
    "T5", // CMD_BUTTON5,
    "T6", // CMD_BUTTON6,
    "T7", // CMD_BUTTON7,
    "T8", // CMD_BUTTON8,
    "T9", // CMD_BUTTON9,
    "TA", // CMD_BUTTON10,
    "ZZ"  // CMD_NOCOMMAND
};
RmRemoteControl::RmRemoteControl() {}

void RmRemoteControl::ReceivedCommand(CommandPkg command)
{
    Serial.println("RmRemoteControl::ReceivedCommand");
    esp_event_post(RMRC_EVENT, RMRC_CMD, &command, sizeof(command), portMAX_DELAY);
    Serial.println("Point4");
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