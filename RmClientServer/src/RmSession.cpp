#include "RmSession.hpp"

RmSession::RmSession()
{
    //sessionId = ' ' + random(0, 63);
    sessionId = '@';
}

//---------------------------------------------
RmSession *rmSession;