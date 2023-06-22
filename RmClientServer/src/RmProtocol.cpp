#include "RmProtocol.hpp"

RmProtocol::RmProtocol()
{
}

//--------------------------------
RmProtocol *rmProtocol = NULL;
ESP_EVENT_DEFINE_BASE(RMPROTOCOL_EVENT);