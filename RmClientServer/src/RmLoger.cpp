#include "RmLoger.hpp"

void RmLoger::Log(DebugLevel level)
{
	if (level <= RM_DEBUG_LEVEL)
	{
		Serial.println(loger->c_str());
		if(publisher != NULL)
		{
			if (level != D_INTERNAL)
			{
				publisher(level, loger->c_str());
			}
		}
	}
	loger->clear();
}

RmLoger::RmLoger(int size, RmLogPublisher externalPublisher)
{
	this->size = size;
	this->publisher = externalPublisher;

	loger_SAFEBUFFER = new char[size + 1];
	loger = new SafeString(size, loger_SAFEBUFFER, "");
}

RmLoger &RmLoger::printf(const char *format, va_list args)
{
	int maxSize = size - loger->length() - 1;	// -1 for the trailing '\0'
	int len = vsnprintf(NULL, 0, format, args); // calculate size of the string
	if (len <= maxSize)
	{
		char *buffer = new char[len + 1];
		vsnprintf(buffer, len + 1, format, args);
		loger->concat(buffer);
		delete[] buffer;
	}
	else
	{
		char *buffer = new char[maxSize + 1];
		vsnprintf(buffer, maxSize + 1, format, args);
		loger->concat(buffer);
		delete[] buffer;
	}
	return *this;
}

RmLoger *rmLoger;