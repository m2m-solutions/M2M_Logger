#include "M2M_Logger.h"
#include <avr/pgmspace.h>
#include <stdio.h>

#ifndef vsnprintf_P
#define vsnprintf_P(s, f, ...) vsnprintf((s), (f), __VA_ARGS__)
#endif

void Logger::begin(Print *logger, LogLevel logLevel)
{
	_serialLog = logger;
	_logLevel = logLevel;
	_isActive = true;
}

void Logger::end()
{
	_isActive = false;
}

void Logger::setFileLogger(File *logger)
{
	_fileLog = logger;
	_lastFlush = millis();
}

void Logger::setLogLevel(LogLevel logLevel)
{
	_logLevel = logLevel;
}

void Logger::setIncludeTimestamp(bool value)
{
	_includeTimestamp = value;
}

void Logger::setIncludeLogLevel(bool value)
{
	_includeLogLevel = value;
}

void Logger::tracePartHexDump(const void* buffer, uint32_t size)
{
	char hexValue[4];
	const uint8_t* pointer = static_cast<const uint8_t*>(buffer); 
	for (uint32_t i=0; i < size; i++)
	{
		sprintf_P(hexValue, "%02x ", pointer[i]);
		log(LogLevel::Trace, true, false, hexValue, nullptr);
	}
}

void Logger::tracePartAsciiDump(const void* buffer, uint32_t size)
{
	const uint8_t* pointer = static_cast<const uint8_t*>(buffer);	
	char output[2];
	output[1] = 0;
	for (uint32_t i=0; i < size; i++)
	{
		output[0] = char(pointer[i]);

		if (pointer[i] == 0x0a)
		{
			log(LogLevel::Trace, true, false, "<LF>", nullptr);
			continue;
		}
		if (pointer[i] == 0x0d)
		{
			log(LogLevel::Trace, true, false, "<CR>", nullptr);
			continue;
		}
		if (pointer[i] < 0x20 || (pointer[i] > 0x7e && pointer[i] < 0xa0))
		{
			output[0] = '.';	
		}
		log(LogLevel::Trace, true, false, output, nullptr);
	}
}

// Private
void Logger::log(LogLevel logLevel, bool isPart, bool writeLinefeed, const char* format, ...)
{
	char buffer[128];

	if (!_isActive || _logLevel < logLevel)
	{
		return;
	}
	if (!isPart)
	{
		printPrefix(logLevel);
	}
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	print(buffer);

	if (writeLinefeed)
	{
		print("\r\n");
	}
}

void Logger::log(LogLevel logLevel, bool isPart, bool writeLinefeed, const __FlashStringHelper* format, ...)
{
	char buffer[128];

	if (!_isActive || _logLevel < logLevel)
	{
		return;
	}
	if (!isPart)
	{
		printPrefix(logLevel);
	}

	PGM_P pointer = reinterpret_cast<PGM_P>(format);
	va_list args;
	va_start(args, format);
	vsnprintf_P(buffer, sizeof(buffer), pointer, args);
	va_end(args);

	print(buffer);

	if (writeLinefeed)
	{
		print("\r\n");
	}
}

void Logger::printPrefix(LogLevel logLevel)
{
	char buffer[16];
	if (_includeTimestamp)
	{
		unsigned long timestamp = millis();			
		snprintf(buffer, sizeof(buffer), "%011lu ", timestamp);
		print(buffer);
	}
	if (_includeLogLevel)
	{
		switch (logLevel)
		{
			case LogLevel::Debug:
			print("DBG ");
			break;
		case LogLevel::Error:
			print("ERR ");
			break;
		case LogLevel::Info:
			print("INF ");
			break;
		case LogLevel::Trace:
			print("TRC ");
			break;
		case LogLevel::NoOutput:
			break;
		}
	}
}

void Logger::print(const char *logLine)
{
	if (_serialLog != nullptr)
	{
		_serialLog->print(logLine);
	}
	if (_fileLog != nullptr)
	{
		_fileLog->print(logLine);
		if (_lastFlush < millis() + SD_FLUSH_TIMEOUT)
		{
			_fileLog->flush();
			_lastFlush = millis();
		}
	}
}
