#include "RmProtocolWs.hpp"

WebSocketsServer *RmProtocolWs::wsServer = NULL;

void RmProtocolWs::Begin()
{
    Serial.println("RmProtocolWs::Begin()");
    wsServer = new WebSocketsServer(SERVER_PORT);
    wsServer->begin();
    wsServer->onEvent(wsEventHandler);
}


void RmProtocolWs::wsEventHandler(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    Serial.println("Event happens");
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
