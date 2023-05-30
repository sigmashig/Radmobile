#pragma once
#include <Arduino.h>
#include <SafeString.h>

#ifndef RM_DEBUG_LEVEL
#define RM_DEBUG_LEVEL D_ALL
#endif

typedef enum
{
	D_OFF = 0,
	D_INTERNAL = 1,
	D_FATAL = 2,
	D_ERROR = 3,
	D_WARN = 4,
	D_INFO = 5,
	D_DEBUG = 6,
	D_ALL = 7
} DebugLevel;

typedef void (*RmLogPublisher)(DebugLevel level, const char *message);

class RmLoger
{
public:
	RmLoger(int size, RmLogPublisher externalPublisher = NULL);
	// RmLoger &printf(const char *format, va_list args);
	RmLoger &Printf(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		RmLoger &p = printf(format, args);
		va_end(args);
		return p;
	};
	RmLoger &Printf(const __FlashStringHelper *format, ...)
	{
		va_list args;
		va_start(args, format);
		RmLoger &p = printf((const char *)format, args);
		va_end(args);
		return p;
	};

	RmLoger &clear()
	{
		loger->clear();
		return *this;
	};
	RmLoger &append(const SafeString &str)
	{
		loger->concat((SafeString &)str);
		return *this;
	};
	RmLoger &append(int str)
	{
		loger->concat(str);
		return *this;
	};
	RmLoger &append(long str)
	{
		loger->concat(str);
		return *this;
	};
	RmLoger &append(unsigned int str)
	{
		loger->concat(str);
		return *this;
	};
	RmLoger &append(unsigned long str)
	{
		loger->concat(str);
		return *this;
	};
	RmLoger &append(double str)
	{
		loger->concat(str);
		return *this;
	};
	RmLoger &append(const __FlashStringHelper *str)
	{
		loger->concat(str);
		return *this;
	};
	RmLoger &append(const char *str)
	{
		loger->concat(str);
		return *this;
	};
	RmLoger &append(String str)
	{
		loger->concat(str.c_str());
		return *this;
	};

	void Log(DebugLevel level);
	void Info(const char *message = NULL)
	{
		if (message != NULL)
		{
			loger->concat(message);
		}
		Log(D_INFO);
	}
	void Info(const __FlashStringHelper *message)
	{
		loger->concat(message);
		Log(D_INFO);
	}

	void Warn(const char *message = NULL)
	{
		if (message != NULL)
		{
			loger->concat(message);
		}
		Log(D_WARN);
	}
	void Debug(const char *message = NULL)
	{
		if (message != NULL)
		{
			loger->concat(message);
		}
		Log(D_DEBUG);
	}
	void Error(const char *message = NULL)
	{
		if (message != NULL)
		{
			loger->concat(message);
		}
		Log(D_ERROR);
	}
	void Fatal(const char *message = NULL)
	{
		if (message != NULL)
		{
			loger->concat(message);
		}
		Log(D_FATAL);
	}
	void Internal(const char *message = NULL)
	{
		if (message != NULL)
		{
			loger->concat(message);
		}
		Log(D_INTERNAL);
	}
	void Warn(const __FlashStringHelper *message)
	{
		loger->concat(message);
		Log(D_WARN);
	}
	void Debug(const __FlashStringHelper *message)
	{
		loger->concat(message);
		Log(D_DEBUG);
	}
	void Error(const __FlashStringHelper *message)
	{
		loger->concat(message);
		Log(D_ERROR);
	}
	void Fatal(const __FlashStringHelper *message)
	{
		loger->concat(message);
		Log(D_FATAL);
	}
	void Internal(const __FlashStringHelper *message)
	{
		loger->concat(message);
		Log(D_INTERNAL);
	}

private:
	int size;
	char *loger_SAFEBUFFER;
	SafeString *loger;
	RmLogPublisher publisher;
	RmLoger &printf(const char *format, va_list args);
};

extern RmLoger *rmLoger;