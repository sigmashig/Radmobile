#include "RmRcEmulator.hpp"
#include "Configuration.hpp"

RmRcEmulator::RmRcEmulator()
{
}

void RmRcEmulator::Begin()
{
    //Serial.println("RmRcEmulator::Begin");
    esp_event_handler_instance_register(CONFIG_EVENT, CONFIG_EVENT_3_SECONDS, loopEventHandle, NULL, NULL);
}

void RmRcEmulator::loopEventHandle(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    //Serial.println("RmRcEmulator::loopEventHandle");
    long c = random(0, CMD_NOCOMMAND);
    CommandPkg command;
    command.command = (CommandType)c;
    command.value = random(0, 100);
    remoteControl->ReceivedCommand(command);
}
