#include "RmProtocolWs.hpp"

WebSocketsServer *RmProtocolWs::wsServer = NULL;
WebSocketsClient *RmProtocolWs::wsClient = NULL;

RmProtocolWs::RmProtocolWs()
{
}

RmProtocolWs::RmProtocolWs(String host, uint16_t port)
{
    this->host = host;
    this->port = port;
}

void RmProtocolWs::Begin()
{
    Serial.println("RmProtocolWs::Begin()");
#if MODE == SERVER
    Serial.println("MODE = 1");
    wsServer = new WebSocketsServer(80 /*SERVER_PORT*/);
    Serial.println("WebSocketsServer created");
    wsServer->begin();
    Serial.println("WebSocketsServer started");
    wsServer->onEvent(wsServerEventHandler);
    // wsServer->broadcastTXT("Hello World!");

#else
    bool res;
    Serial.println("MODE = 2");
    wsClient = new WebSocketsClient();
    // wsClient->begin(host, port, "/");
    wsClient->begin("192.168.0.88", 80);
    wsClient->onEvent(wsClientEventHandler);
    wsClient->setReconnectInterval(5000);
    while (!wsClient->isConnected())
    {
        Serial.println("Trying to connect...");
        wsClient->loop();
        delay(1000);
    }
    res = wsClient->isConnected();
    Serial.println("Is connected = " + String(res));

    res = SendCommand("Hello World!");
    Serial.println("Command result = " + String(res));
#endif
    // loopTaskHandle = xTaskCreateStatic(wsLoopTask, "wsLoopTask", WS_STACK_SIZE, NULL, 2, loopTaskStack, &loopTaskBuffer);
}

bool RmProtocolWs::SendCommand(String command)
{
    bool res;
    Serial.println("RmProtocolWs::SendCommand()");
#if MODE == SERVER
    res = wsServer->broadcastTXT(command);
#else
    res = wsClient->sendTXT(command);
#endif
    return res;
}

void RmProtocolWs::Loop()
{
#if MODE == SERVER
    // Serial.println("RmProtocolWs::Loop()");
    wsServer->loop();
#else
    wsClient->loop();
#endif
}

void RmProtocolWs::wsClientEventHandler(WStype_t type, uint8_t *payload, size_t length)
{
    Serial.println("CLIENT Event happens");
}

void RmProtocolWs::wsLoopTask(void *params)
{
    while (true)
    {
#if MODE == SERVER
        wsServer->loop();
#else
        wsClient->loop();
#endif
        delay(50);
    }
}

void RmProtocolWs::wsServerEventHandler(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    Serial.println("SERVER Event happens");
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = wsServer->remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        break;
    }
    case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);
        break;
    case WStype_BIN:
        Serial.printf("[%u] get binary length: %u\n", num, length);
        break;
    }
}
