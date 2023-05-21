#include "RmRemoteControl.hpp"

ESP_EVENT_DEFINE_BASE(RMRC_EVENT);

void RmRemoteControl::CmdToServer(CommandPkg command)
{
    Serial.println("RmRemoteControl::ReceivedCommand");
    esp_event_post(RMRC_EVENT, RMRC_CMD, &command, sizeof(command), portMAX_DELAY);
}

//---------------------------------------------------------------------------------------------
RmRemoteControl *remoteControl;