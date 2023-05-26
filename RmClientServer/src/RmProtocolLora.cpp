#include "RmProtocolLora.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"

#if LORA == SX1278
SX1278 *RmProtocolLora::radio;
#endif

RmProtocolLora::RmProtocolLora()
{
    LoraConfig *cfg;
#if MODE == SERVER
    cfg = &rmConfig->srvLoraConfig;
#else
    cfg = &rmConfig->clientLoraConfig;
#endif
#if LORA == SX1278
    radio = new SX1278(new Module(cfg->connection.LoraSx1278.nss,
                                  cfg->connection.LoraSx1278.dio0,
                                  cfg->connection.LoraSx1278.nrst,
                                  cfg->connection.LoraSx1278.dio1));
#endif
    esp_event_handler_register(RMPROTOCOL_EVENT, RMEVENT_LORA_RECEIVED, packageReceived, NULL);
}

void RmProtocolLora::Begin()
{
    int state = radio->begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println(F("LORA is initiated successfully!"));
    }
    else
    {
        Serial.print(F("LORA initialization failed, code "));
        Serial.println(state);
    }
    radio->setDio0Action([]()
                         { 
                            uint status = radio->getIRQFlags();
                            if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_RX_DONE)
                            {
                                esp_event_isr_post(RMPROTOCOL_EVENT, RMEVENT_LORA_RECEIVED, NULL, 0, NULL);
                            }
                            else if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_TX_DONE)
                            {
                                esp_event_isr_post(RMPROTOCOL_EVENT, RMEVENT_LORA_SENT, NULL, 0, NULL);} },
                         RISING);
}

void RmProtocolLora::Reconnect()
{
}

void RmProtocolLora::ReceivedCommand(String command)
{
    Serial.println("RmProtocolLora::ReceivedCommand()");
    Serial.println(command);
    RmCommandPkg cmd;
    cmd = RmCommands::StringToCommand(command);
    esp_event_post(RMPROTOCOL_EVENT, RMEVENT_CMD_RECEIVED, &cmd, sizeof(cmd), portMAX_DELAY);
}

bool RmProtocolLora::SendCommand(String command)
{
    String pkg = String(rmSession->GetSessionId());
    pkg += command;
    Serial.printf("Sending: %s(%u)\n", pkg.c_str(), pkg.length());
    int state = radio->startTransmit(pkg.c_str(), pkg.length());
    return false;
}

void RmProtocolLora::packageReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Serial.println("RmProtocolLora::packageReceived()");
    String str;
    RmProtocolLora::radio->readData(str);
    Serial.printf("Received: %s(%u)\n", str.c_str(), str.length());
    if (str[0] != rmSession->GetSessionId())
    {
        Serial.printf("Received for other device, ignoring.");
        return;
    }
    RmCommandPkg cmd = RmCommands::StringToCommand(str.substring(1));
    esp_event_post(RMPROTOCOL_EVENT, RMEVENT_CMD_RECEIVED, &cmd, sizeof(&cmd), portMAX_DELAY);
}
