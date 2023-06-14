#include "SigmaEEPROM.hpp"
#include "EEPROM.h"



uint16_t SigmaEEPROM::Read16(uint16_t addr) {
	uint16_t res;
	res = (((uint16_t)EEPROM.read(addr)) << 8 & 0xFF00) + EEPROM.read(addr + 1);
	return res;
}

byte SigmaEEPROM::Read8(uint16_t addr) {
	return EEPROM.read(addr);
}


void SigmaEEPROM::Write16(uint16_t addr, uint16_t val) {
	EEPROM.write(addr, (byte)((val >> 8) & 0x00FF));
	EEPROM.write(addr + 1, (byte)((val & 0x00FF)));

}
void SigmaEEPROM::WriteBytes(uint16_t addr, byte *val, byte len)
{
	for (int i = 0; i<len; i++){
		EEPROM.write(addr, val[i]);
}
}

void SigmaEEPROM::ReadBytes(uint16_t addr, byte *val, byte len)
{
	for (int i = 0; i<len; i++){
		val[i] = EEPROM.read(addr);
	}
}

void SigmaEEPROM::Write8(uint16_t addr, byte val) {
	EEPROM.write(addr, val);
}

/*
void SigmaEEPROM::WriteIp(IPAddress& ip, uint16_t addr) {
	bool res = (ip[0] + ip[1] + ip[2] + ip[3]) != 0;
	if (res) {
		for (int i = 0; i < 4;i++) {
			Write8(addr + i, ip[i]);
		}
	}
}


void SigmaEEPROM::ReadIp(IPAddress& ip, uint16_t addr) {
	for (int i = 0; i < 4; i++) {
		ip[i] = Read8(addr + i);
	}
}
*/