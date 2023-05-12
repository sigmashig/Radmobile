#include "RmProtocolWs.hpp"

WebSocketsClient *RmProtocolWs::wsClient = NULL;

RmProtocolWs::RmProtocolWs(String host, uint16_t port)
{
    this->host = host;
    this->port = port;
}

void RmProtocolWs::Begin()
{
    Serial.println("RmProtocolWs::Begin()");
    wsClient = new WebSocketsClient();
    wsClient->begin(host, port);
    wsClient->onEvent(wsEventHandler);
    wsClient->setReconnectInterval(5000);
}

void RmProtocolWs::wsEventHandler(WStype_t type, uint8_t *payload, size_t length)
{
    Serial.println("Event happens");
}
