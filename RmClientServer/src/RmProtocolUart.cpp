#include "RmProtocolUart.hpp"
#include <esp_event.h>
#include "RmTypes.hpp"
#include "RmPackageValidator.hpp"

ESP_EVENT_DECLARE_BASE(RMCONFIG_EVENT);
ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

RmProtocolUart::RmProtocolUart(HardwareSerial *uart) : RmProtocol(), uart(uart)
{
    esp_event_handler_register(RMCONFIG_EVENT, RMCONFIG_EVENT_LOOP, &RmProtocolUart::loopEventHandler, NULL);
}

void RmProtocolUart::Begin()
{
    isReady = true;
}

void RmProtocolUart::Reconnect()
{
   
}


bool RmProtocolUart::SendPkg(String pkg)
{
    if (uart->availableForWrite())
    {
        uart->println(pkg);
        return true;
    }
    return false;
}

void RmProtocolUart::loopEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (rmProtocolUart->uart->available())
    {
        String pkg = rmProtocolUart->uart->readStringUntil('\n');
        if (pkg.length() > 0)
        {
            RmPackageValidator::Validate(pkg);
        }
    }
}

RmProtocolUart::~RmProtocolUart()
{
    esp_event_handler_unregister(RMCONFIG_EVENT, RMCONFIG_EVENT_LOOP, &RmProtocolUart::loopEventHandler);
}

//--------------------------------------------------------------------------------
RmProtocolUart *rmProtocolUart = NULL;