#include "RmRcEmulator.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"

RmRcEmulator::RmRcEmulator()
{
}

void RmRcEmulator::Begin()
{
    // Serial.println("RmRcEmulator::Begin");
    esp_event_handler_instance_register(RMCONFIG_EVENT, RMCONFIG_EVENT_3_SECONDS, loopEventHandle, NULL, NULL);
}

void RmRcEmulator::loopEventHandle(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    // Serial.println("RmRcEmulator::loopEventHandle");
    long c = random(0, CMD_NOCOMMAND);
    RmCommandPkg command;
    command.command = (RmCommandType)c;
    command.value = random(0, MAX_COMMAND_VALUE);
    remoteControl->CmdToServer(command);
}
