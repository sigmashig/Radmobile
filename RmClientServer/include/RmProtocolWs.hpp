#pragma once

#include <Arduino.h>
#include "RmProtocol.hpp"

#include <WebSocketsClient.h>
#include <WebSocketsServer.h>

#define WS_STACK_SIZE 1000
class RmProtocolWs : public RmProtocol
{
public:
    RmProtocolWs();
    RmProtocolWs(String host, uint16_t port);
    void Begin();
    bool SendCommand(String command);
    void ReceivedCommand(String command){};

private:
    String host;
    uint16_t port;
    StaticTask_t loopTaskBuffer;
    StackType_t loopTaskStack[WS_STACK_SIZE];
    TaskHandle_t loopTaskHandle;

    static WebSocketsServer *wsServer;
    static WebSocketsClient *wsClient;

    static void wsServerEventHandler(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
    static void wsClientEventHandler(WStype_t type, uint8_t *payload, size_t length);
    static void wsLoopTask(void *params);
};