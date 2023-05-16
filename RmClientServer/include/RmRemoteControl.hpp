#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include <RmCommands.hpp>

ESP_EVENT_DECLARE_BASE(RMRC_EVENT);

typedef enum
{
    RMRC_CMD
} RmCommandEvent;

class RmRemoteControl
{
public:
    RmRemoteControl();
    virtual void Begin() = 0;
    void ReceivedCommand(CommandPkg command);
    static void CommandToString(CommandPkg command, String &commandString);
    static String cmdTxt[CMD_NOCOMMAND+1];
    //   virtual void Reconnect() = 0;
};

extern RmRemoteControl *remoteControl;