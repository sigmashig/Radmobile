#include "RmProtocolUart.hpp"
#include <esp_event.h>
#include "RmTypes.hpp"
#include "RmPackageValidator.hpp"

//ESP_EVENT_DECLARE_BASE(RMCONFIG_EVENT);
ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

RmProtocolUart::RmProtocolUart(HardwareSerial *uart) : RmProtocol(), uart(uart)
{
    timer = xTimerCreateStatic("uartTimer", pdMS_TO_TICKS(1000), pdTRUE, this, uartTimerCallback, &uartTimer);
}

void RmProtocolUart::Begin()
{
    isReady = true;
    xTimerStart(timer, 0);
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

void RmProtocolUart::uartTimerCallback(TimerHandle_t xTimer)
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
    xTimerStop(&uartTimer, 0);
    //esp_event_handler_unregister(RMCONFIG_EVENT, RMCONFIG_EVENT_LOOP, &RmProtocolUart::loopEventHandler);
}

//--------------------------------------------------------------------------------
RmProtocolUart *rmProtocolUart = NULL;