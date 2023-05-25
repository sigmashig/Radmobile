#pragma once
#include <Arduino.h>

class RmSession
{
public:
    int GetSessionId() { return sessionId; };
    RmSession();

private:
    int sessionId;
};

extern RmSession *rmSession;