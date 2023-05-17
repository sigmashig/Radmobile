#include "RmClient.hpp"
#include "RmProtocol.hpp"
#include "RmProtocolWs.hpp"
#include "RmProtocolMqtt.hpp"

RmClient::RmClient(String host, uint16_t port)
{
    this->host = host;
    this->port = port;
}

void RmClient::Begin()
{
    Serial.println("RmClient::Begin()");
#if PROTOCOL == 1

    rmProtocol = new RmProtocolWs(host, port);
    rmProtocol->Begin();

#elif PROTOCOL == 2
    rmProtocol = new RmProtocolMqtt();
    rmProtocol->Begin();
#endif
    esp_event_handler_instance_register(RMSERVER_EVENT, RMEVENT_RESPONSE,
                                        responseEventHandler, NULL, NULL);
}

void RmClient::responseEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("responseEventHandler");
    // rmProtocol->Send("Hello from WS Server");
}

//--------------------------------
RmClient *rmClient=NULL;
