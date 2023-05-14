#include "RMServer.hpp"
#include "RmProtocol.hpp"
#include "RmProtocolWs.hpp"
#include "RmProtocolMqtt.hpp"

RmServer::RmServer()
{
}

void RmServer::Begin()
{
    Serial.println("RmServer::Begin()");
#if PROTOCOL == 1
    rmProtocol = new RmProtocolWs();
    rmProtocol->Begin();
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolMqtt();
    rmProtocol->Begin();
#endif
    esp_event_handler_instance_register(RMSERVER_EVENT, RMEVENT_RESPONSE,
                                        responseEventHandler, NULL, NULL);

}

void RmServer::Loop()
{
#if PROTOCOL == 1
    rmProtocol->Loop();
#endif
}

void RmServer::SendCommand(String command)
{
    rmProtocol->SendCommand(command);
}

void RmServer::Reconnect()
{
    rmProtocol->Reconnect();
}

void RmServer::responseEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("responseEventHandler");
    // rmProtocol->Send("Hello from WS Server");
}

//--------------------------------
RmServer *rmServer = NULL;
