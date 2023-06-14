#pragma once

#include <Arduino.h>

//#include <IPAddress.h>

#define RM_EEPROM_SESSION_ID 0
#define RM_EEPROM_KEY 1
#define RM_EEPROM_KEY_LENGTH 16

class SigmaEEPROM  
{

 public:

  	//  static void WriteIp(IPAddress& ip, uint16_t addr);
	//	static void ReadIp(IPAddress& ip, uint16_t addr);

	static uint16_t Read16(uint16_t addr);
	static byte Read8(uint16_t addr);
	static void Write8(uint16_t addr, byte val);
	static void Write16(uint16_t addr, uint16_t val);
	static void WriteBytes(uint16_t addr, byte* val, byte len);
	static void ReadBytes(uint16_t addr, byte* val, byte len);
private:

};



