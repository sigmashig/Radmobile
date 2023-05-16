#include "RmRcEmulator.hpp"

RmRcEmulator::RmRcEmulator()
{
}

void RmRcEmulator::Begin()
{
    Serial.println("RmRcEmulator::Begin");
    loopTaskHandle = xTaskCreateStatic(loopTask, "loopTask", EMULATOR_LOOP_STACK_SIZE, NULL, 1, loopTaskStack, &loopTaskBuffer);
}

void RmRcEmulator::loopTask(void *params)
{
    CommandPkg command;
    Serial.println("RmRcEmulator::loopTask");
    while (true)
    {
        for (int i = 0; i < CMD_NOCOMMAND; i++)
        {
            long c = random(0, CMD_NOCOMMAND);
            Serial.println("Point1.5");
            Serial.printf("c = %d\n", c);
            command.command = (CommandType)c;
            if (command.command == CommandType::CMD_J1_BACKWARD ||
                command.command == CommandType::CMD_J1_FORWARD ||
                command.command == CommandType::CMD_J1_LEFT ||
                command.command == CommandType::CMD_J1_RIGHT ||
                command.command == CommandType::CMD_J2_BACKWARD ||
                command.command == CommandType::CMD_J2_FORWARD ||
                command.command == CommandType::CMD_J2_LEFT ||
                command.command == CommandType::CMD_J2_RIGHT)
            {
                command.value = random(0, 1024);
            }
            else
            {
                command.value = 0;
            }
            Serial.println("Point2");
            remoteControl->ReceivedCommand(command);
            Serial.println("Point3");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
}
