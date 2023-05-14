#include "RmProtocol.hpp"
#include <WebSockets.h>

RmProtocol::RmProtocol()
{
}

//--------------------------------
RmProtocol *rmProtocol;
ESP_EVENT_DEFINE_BASE(RMSERVER_EVENT);