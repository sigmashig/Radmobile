#pragma once
#include <Arduino.h>

class RmSession
{
public:
    char GetSessionId() { return sessionId; };
    RmSession();

private:
    char sessionId;
};

extern RmSession *rmSession;