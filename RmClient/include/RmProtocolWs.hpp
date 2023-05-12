#pragma once

#include <Arduino.h>
//#include <WebSocketsServer.h>
#include <WebSocketsClient.h>
#include "RmProtocol.hpp"

class RmProtocolWs : public RmProtocol
{
public:
    RmProtocolWs(String host, uint16_t port);
    void Begin();

private:
    static WebSocketsClient *wsClient;

    static void wsEventHandler(WStype_t type, uint8_t *payload, size_t length);
};
