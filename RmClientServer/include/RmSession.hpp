#pragma once
#include <Arduino.h>
#include "RmTypes.hpp"

class RmSession
{
public:
    IdConfig GetSessionId() { return sessionId; };
    void SetSessionId(IdConfig sessionId);
    RmSession();

private:
    IdConfig sessionId;
    bool GetSessionIdFromEEPROM();
    void SetSessionIdToEEPROM();
};

extern RmSession *rmSession;