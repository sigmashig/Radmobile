#pragma once

#include <Arduino.h>
#include "RmProtocol.hpp"
#include <WebSocketsServer.h>

class RmProtocolWs : public RmProtocol
{
    public:
        void Begin();
    private:
        static WebSocketsServer* wsServer;

        static void wsEventHandler(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
};