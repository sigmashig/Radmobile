#include "RMServer.hpp"
#include "RmProtocol.hpp"
#if PROTOCOL == 1
#include "RmProtocolWs.hpp"
#elif PROTOCOL == 2
#include "RmProtocolMqtt.hpp"
#endif

RmServer::RmServer()
{
}

void RmServer::Begin()
{
    Serial.println("RmServer::Begin()");
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, RMEVENT_CMD_RECEIVED, responseEventHandler, NULL, NULL);
    esp_event_handler_instance_register(RMRC_EVENT, RMRC_CMD, commandEventHandler, NULL, NULL);

#if PROTOCOL == 1
    rmProtocol = new RmProtocolWs();
    rmProtocol->Begin();
#elif PROTOCOL == 2
    rmProtocol = new RmProtocolMqtt();
    rmProtocol->Begin();
#endif
}

void RmServer::SendCommand(String command)
{
    rmProtocol->SendCommand(command);
}

void RmServer::responseEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("responseEventHandler");
}

void RmServer::commandEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("commandEventHandler");
    CommandPkg *command = (CommandPkg *)event_data;
    String commandString;
    RmCommands::CommandToString(*command, commandString);
    Serial.println("Command received: " + commandString);
    rmServer->SendCommand(commandString);
    // rmProtocol->Send("Hello from WS Server");
}

//--------------------------------
RmServer *rmServer = NULL;
