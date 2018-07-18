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
#if defined(ARDUINO_ARCH_SAMD)
#define vsnprintf_P vsnprintf
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

	void error(const char* message, ...);
	void info(const char* message, ...);
	void debug(const char* message, ...);
	void trace(const char* message, ...);
	void traceStart(const char* message, ...);
	void tracePart(const char* message, ...);
	void tracePartHexDump(void* buffer, uint32_t size);
	void traceEnd(const char* message, ...);

private:
	LogLevel _logLevel = LogLevel::Info;
	Print* _serialLog = nullptr;
	Print* _fileLog = nullptr;
	uint32_t _lastFlush = 0;
	bool _includeTimestamp = false;
	bool _includeLogLevel = false;
	bool _isActive = false;

	void log(LogLevel logLevel, bool isPart, bool writeLinefeed, const char* message, va_list* args);
	void printFormat(Print* logger, const char format, va_list* args);
	void print(const char *logLine);
};

extern Logger Log;

#endif