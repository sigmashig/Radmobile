#pragma once
#include <Arduino.h>
#include "RmTypes.hpp"

class RmId
{
public:
    static String IdToString(char id, byte key[]);
    static IdConfig StringToId(String idString);
};
