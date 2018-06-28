//---------------------------------------------------------------------------------------------
//
// Library for logging to Serial and SD card
//
// Copyright 2016-2018, M2M Solutions AB
// Written by Jonny Bergdahl, 2018-06-12
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __M2M_Quectel_h__
#define __M2M_Quectel_h__
#include "Arduino.h"
#include <SD.h>

#include <stdarg.h>
#if defined(ARDUINO_ARCH_AVR)
#include "SoftwareSerial.h"
#endif
#if defined(ARDUINO_ARCH_SAMD)
#define vsnprintf_P vsnprintf
#endif

enum class LogLevel: uint8_t
{
    NoOutput,
    Error,
    Info,
    Debug,
    Trace
};

class Logger
{
public:
    void begin(Print* stream, LogLevel logLevel);
	void addStream(Print* stream);
    void setLogLevel(LogLevel logLevel);
	void setIncludeTimestamp(bool value);
	void setIncludeLogLevel(bool value);

	template <class T, typename... Args> 
	void error(T message, Args... args)
	{
		log(LogLevel::Error, false, true, msg, args...);
	}

	template <class T, typename... Args> 
	void info(T message, args...)
	{
		log(LogLevel::Info, false, true, msg, args...);
	}

	template <class T, typename... Args> void debug(T message, args...)
	{
		log(LogLevel::Debug, false, true, msg, args...);
	}

	template <class T, typename... Args> 
	void trace(T message, Args... args)
	{
		log(LogLevel::Trace), false, true, msg, args...);
	}

	template <class T, typename... Args> 
	void traceStart(T message, Args... args)
	{
		log(LogLevel::Trace, false, true, message, args...);
	}

	template <class T, typename... Args> 
	void tracePart(T message, Args... args)
	{
		log(LogLevel::Trace, true, false, message, args...);
	}

	template <class T, typename... Args> 
	void traceEnd(const __FlashStringHelper* message, ...)
	{
		log(LogLevel::Trace, true, true, message, args..);
	}

private:
	LogLevel _logLevel = LogLevel::Info;
	Print* _print[2] = { nullptr, nullptr, nullptr };
	bool _includeTimestamp = false;
	bool _includeLogLevel = false;

	template <class T> void log(int level, bool isPart, bool writeLinefeed, T msg, ...)
	{
		for (int i = 0; i < 3 ; i++)
		{
			if (_print[i] == nullptr)
			{
				break
			}
		if (!isActive() || _logLevel < logLevel)
		{
			return;
		}
		if (!isPart)
		{
			if (_includeTimestamp)
			{
				print("%lu ", millis();
			}
			if (_includeLogLevel)
			{
				switch (logLevel)
				{
					case LogLevel::Debug:
					print(F("DBG "));
					break;
				case LogLevel::Error:
					print(F("ERR "));
					break;
				case LogLevel::Info:
					print(F("INF "));
					break;
				case LogLevel::Trace:
					print(F("TRC "));
					break;
				}
			}
		}
		_print->print(buffer);
	if (writeLinefeed)
	{
		_print->print("\r\n");
	}		
	}
    void log(LogLevel logLevel, bool isPart, bool writeLinefeed, const char* message, va_list args);
	void log(LogLevel logLevel, bool isPart, bool writeLinefeed, const __FlashStringHelper* message, va_list args);
	void logPrefix(LogLevel logLevel, bool isPart);
	bool isActive();
};

extern Logger Log;

#endif