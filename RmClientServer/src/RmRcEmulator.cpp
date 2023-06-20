#include "RmRcEmulator.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"

RmRcEmulator::RmRcEmulator()
{
}

void RmRcEmulator::Begin()
{
    TimerHandle_t timer = xTimerCreateStatic(
        "loopTimer", pdMS_TO_TICKS(3000), pdTRUE, NULL,
        [](TimerHandle_t xTimer)
        {
            long c = random(0, CMD_NOCOMMAND);
            RmCommandPkg command;
            command.command = (RmCommandType)c;
            command.value = random(0, MAX_COMMAND_VALUE);
            remoteControl->CmdToServer(command);
        },
        &loopTimerBuffer);
    xTimerStart(timer, 0);
}
