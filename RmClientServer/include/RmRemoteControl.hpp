#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include <RmTypes.hpp>
// #include <RmCommands.hpp>

ESP_EVENT_DECLARE_BASE(RMRC_EVENT);

typedef enum
{
    RMRC_NEWSTATE
} RmCommandEvent;

class RmRemoteControl
{
public:
    RmRemoteControl();
    virtual void Begin() = 0;
    void CmdToServer(RmCommandPkg command);
    static String StateAsString(CommandState& state);
    static String CommandToString(RmCommandPkg command);
    // static RmCommandPkg StringToCommand(String commandString);
protected:
    CommandState CurrentState;
    static char cmdTxt[CMD_NOCOMMAND + 1];
};

extern RmRemoteControl *remoteControl;