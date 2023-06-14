#include "RmSession.hpp"
#include "SigmaEEPROM.hpp"
#include "RmConfiguration.hpp"

void RmSession::SetSessionId(IdConfig sessionId)
{
    this->sessionId = sessionId;
    SetSessionIdToEEPROM();
}

RmSession::RmSession()
{
    GetSessionIdFromEEPROM();
}

bool RmSession::GetSessionIdFromEEPROM()
{
    sessionId.id = SigmaEEPROM::Read8(RM_EEPROM_SESSION_ID);
    SigmaEEPROM::ReadBytes(RM_EEPROM_KEY, sessionId.key, KEY_LENGTH);
    sessionId.isValid = false;
    if (sessionId.id >= CHARSET_BEGIN && sessionId.id <= CHARSET_END)
    {
        sessionId.isValid = true;
        for (int i = 0; i < KEY_LENGTH; i++)
        {
            if (sessionId.key[i] < CHARSET_BEGIN || sessionId.key[i] > CHARSET_END)
            {
                sessionId.isValid = false;
                break;
            }
        }
    }

    return sessionId.isValid;
}

void RmSession::SetSessionIdToEEPROM()
{
    SigmaEEPROM::Write8(RM_EEPROM_SESSION_ID, sessionId.id);
    SigmaEEPROM::WriteBytes(RM_EEPROM_KEY, sessionId.key, KEY_LENGTH);
}

//---------------------------------------------
RmSession *rmSession;