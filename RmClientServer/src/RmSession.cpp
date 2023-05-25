#include "RmSession.hpp"

RmSession::RmSession()
{
    sessionId = random(0, 63);
}

//---------------------------------------------
RmSession *rmSession;