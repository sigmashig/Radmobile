#include "RmProtocolMqtt.hpp"
#include <esp_event.h>
#include "RmLoger.hpp"
#include "RmCommands.hpp"
#include "RmConfiguration.hpp"

String RmProtocolMqtt::topic = "RadMobile/Command";
AsyncMqttClient RmProtocolMqtt::mqttClient;

void RmProtocolMqtt::Begin()
{
    mqttClient.setServer(MQTT_URL, MQTT_PORT);
    String clientId = "RM_" + String(rmConfig->Id, HEX);
    mqttClient.setClientId(clientId.c_str());
    mqttClient.setCleanSession(true);
    mqttClient.setKeepAlive(15);

    mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason)
                            { 
                                rmLoger->append("Disconnected from MQTT. Reason=").append((uint)reason).Error(); 
                                connectToMqtt(); });
    mqttClient.onMessage(messageReceived);
    mqttClient.onConnect(_onConnect);
    connectToMqtt();
}

void RmProtocolMqtt::ReceivedState(String stateString)
{
    CommandState state;
    state = RmCommands::StringToState(stateString);
    if (state.isValid)
    {
        esp_event_post(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, &state, sizeof(state), portMAX_DELAY);
    }
}

bool RmProtocolMqtt::SendCommand(String command)
{
    uint res = mqttClient.publish(topic.c_str(), 0, false, command.c_str());
    return res != 0;
}

void RmProtocolMqtt::Reconnect()
{
    rmLoger->Warn("Mqtt Reconnect");
    connectToMqtt();
}

bool RmProtocolMqtt::_onConnect(bool sessionPresent)
{
    rmLoger->Info(F("Connected to MQTT."));
    mqttClient.subscribe(topic.c_str(), 0);
    return true;
}

void RmProtocolMqtt::connectToMqtt()
{
    mqttClient.connect();
}

void RmProtocolMqtt::messageReceived(char *topic, char *payload,
                                     AsyncMqttClientMessageProperties properties, size_t len,
                                     size_t index, size_t total)
{
    if (len > 0)
    {
        payload[len] = '\0';
        rmLoger->append(F("[")).append(topic).append(F("]:")).append(payload).Debug();
        rmProtocol->ReceivedState(String(payload));
    }
    else
    {
        rmLoger->Error(F("MQTT:Empty message received!"));
    }
}
