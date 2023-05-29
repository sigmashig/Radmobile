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
protected:
    CommandState CurrentState;
};

extern RmRemoteControl *remoteControl;