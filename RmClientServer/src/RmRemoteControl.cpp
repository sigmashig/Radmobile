#include "RmRemoteControl.hpp"

ESP_EVENT_DEFINE_BASE(RMRC_EVENT);

void RmRemoteControl::CmdToServer(RmCommandPkg command)
{
    Serial.printf("RmRemoteControl::ReceivedCommand - %s\n", RmCommands::CommandToString(command).c_str());
    esp_event_post(RMRC_EVENT, RMRC_CMD, &command, sizeof(command), portMAX_DELAY);
}

//---------------------------------------------------------------------------------------------
RmRemoteControl *remoteControl;