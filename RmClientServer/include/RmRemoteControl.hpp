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
    // RmRemoteControl();
    virtual void Begin() = 0;
    void CmdToServer(RmCommandPkg command);
};

extern RmRemoteControl *remoteControl;