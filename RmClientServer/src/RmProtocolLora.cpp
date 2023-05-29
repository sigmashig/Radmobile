#include "RmProtocolLora.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"

LORA *RmProtocolLora::radio;
bool RmProtocolLora::isChannelFree = true;
String RmProtocolLora::pkgForTransmit = "";

RmProtocolLora::RmProtocolLora()
{
    LoraConfig *cfg;
#if MODE == 1
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
    esp_event_handler_register(RMPROTOCOL_EVENT, RMEVENT_LORA_SOMETHING_HAPPENS, packageReceived, NULL);
}

ICACHE_RAM_ATTR void RmProtocolLora::loraISR()
{
    esp_event_isr_post(RMPROTOCOL_EVENT, RMEVENT_LORA_SOMETHING_HAPPENS, NULL, 0, NULL);
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
    radio->setDio0Action(loraISR, RISING);
    radio->startReceive();
}

void RmProtocolLora::Reconnect()
{
}

void RmProtocolLora::ReceivedState(String state)
{
    Serial.println("RmProtocolLora::ReceivedCommand: " + state);
    Serial.println(state);
    CommandState st = RmCommands::StringToState(state);
    if (st.isValid)
    {
        Serial.println("Valid command!");
        Serial.printf("State to processing: %d, %d, %d, %d, 0x%04x\n", st.straight, st.powerStraight, st.turn, st.powerTurn, st.buttons.buttonPacked);
        esp_event_post(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, &st, sizeof(st), portMAX_DELAY);
    }
    else
    {
        Serial.println("Invalid command!");
    }
}

bool RmProtocolLora::SendCommand(String command)
{
    pkgForTransmit = String(rmSession->GetSessionId());
    pkgForTransmit += command;
    if (isChannelFree)
    {
        Serial.printf("Sending MAIN: %s(%u)\n", pkgForTransmit.c_str(), pkgForTransmit.length());
        isChannelFree = false;
        int state = radio->startTransmit(pkgForTransmit.c_str(), pkgForTransmit.length());

        if (state != RADIOLIB_ERR_NONE)
        {
            Serial.print(F("LORA transmission failed, code "));
            Serial.println(state);
            return false;
        }
    }
    else
    {
        Serial.printf("Queued: %s(%u)\n", pkgForTransmit.c_str(), pkgForTransmit.length());
    }
    return true;
}

void RmProtocolLora::packageReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    // Serial.println("RmProtocolLora::packageReceived()");
    uint status = radio->getIRQFlags();
    Serial.printf("Something happened, status: %u\n", status);
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_RX_DONE)
    { // received a packet
        String str;
        RmProtocolLora::radio->readData(str);
        Serial.printf("Received: %s(%u)\n", str.c_str(), str.length());
        if (str[0] != rmSession->GetSessionId())
        {
            Serial.printf("Received for other device, ignoring.");
            return;
        }
        rmProtocol->ReceivedState(str.substring(1));
    }
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_TX_DONE)
    { // transfer completed
        radio->finishTransmit();
        pkgForTransmit = "";
        Serial.println("Transfer completed.");
        // radio->startReceive();
    }
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_VALID_HEADER)
    {
        Serial.println("Valid header.");
    }
    if (pkgForTransmit.length() > 0)
    {
        Serial.println("Sending...");
        isChannelFree = false;
        radio->startTransmit(pkgForTransmit.c_str(), pkgForTransmit.length());
    }
    else
    {
        radio->startReceive();
        isChannelFree = true;
    }
}