#include "RmClient.hpp"
#include "RmProtocol.hpp"
#include "RmProtocolMqtt.hpp"

RmClient::RmClient(String host, uint16_t port)
{
    this->host = host;
    this->port = port;
}

void RmClient::Begin()
{
    Serial.println("RmClient::Begin()");
#if PROTOCOL == 2
    rmProtocol = new RmProtocolMqtt();
    rmProtocol->Begin();
#endif
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, RMEVENT_CMD_RECEIVED,
                                        commandReceived, NULL, NULL);
}

void RmClient::commandReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("responseEventHandler");
    // rmProtocol->Send("Hello from WS Server");
}

//--------------------------------
RmClient *rmClient = NULL;
