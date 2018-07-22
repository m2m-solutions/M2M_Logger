//---------------------------------------------------------------------------------------------
//
// Library for logging to Serial port and SD card
//
// Copyright 2016-2018, M2M Solutions AB
// Written by Jonny Bergdahl, 2018-06-12
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __M2M_LOGGER_h__
#define __M2M_LOGGER_h__
#include "Arduino.h"
#include "SD.h"

#include <stdarg.h>
#if defined(ARDUINO_ARCH_AVR)
#include "SoftwareSerial.h"
#endif

#define SD_FLUSH_TIMEOUT	4096

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
    void begin(Print* logger, LogLevel logLevel);
	void end();
	void setFileLogger(File* logger);
    void setLogLevel(LogLevel logLevel);
	void setIncludeTimestamp(bool value);
	void setIncludeLogLevel(bool value);

	template <class T, typename... Args>
	void error(T message, Args... args)
	{
		log(LogLevel::Error, false, true, message, args...);
	}
	template <class T, typename... Args>
	void info(T message, Args... args)
	{
		log(LogLevel::Info, false, true, message, args...);
	}
	template <class T, typename... Args>
	void debug(T message, Args... args)
	{
		log(LogLevel::Debug, false, true, message, args...);
	}
	template <class T, typename... Args>
	void trace(T message, Args... args)
	{
		log(LogLevel::Trace, false, true, message, args...);
	}

	template <class T, typename... Args>
	void traceStart(T message, Args... args)
	{
		log(LogLevel::Trace, false, false, message, args...);
	}
	template <class T, typename... Args>
	void tracePart(T message, Args... args)
	{
		log(LogLevel::Trace, true, false, message, args...);
	}
	void tracePartHexDump(const void* buffer, uint32_t size);
	template <class T, typename... Args>
	void traceEnd(T message, Args... args)
	{
		log(LogLevel::Trace, true, true, message, args...);
	}

private:
	LogLevel _logLevel = LogLevel::Info;
	Print* _serialLog = nullptr;
	Print* _fileLog = nullptr;
	uint32_t _lastFlush = 0;
	bool _includeTimestamp = false;
	bool _includeLogLevel = false;
	bool _isActive = false;

	void log(LogLevel logLevel, bool isPart, bool writeLinefeed, const char* message, ...);
	void log(LogLevel logLevel, bool isPart, bool writeLinefeed, const __FlashStringHelper* message, ...);
	void printPrefix(LogLevel logLevel);
	void print(const char *logLine);
};

#endif