#include "M2M_Logger.h"

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

void Logger::error(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	log(LogLevel::Error, false, true, message, &args);
	va_end(args);	
}

void Logger::info(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	log(LogLevel::Info, false, true, message, &args);
	va_end(args);	
}

void Logger::debug(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	log(LogLevel::Debug, false, true, message, &args);
	va_end(args);	
}

void Logger::trace(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	log(LogLevel::Trace, false, true, message, &args);
	va_end(args);	
}

void Logger::traceStart(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	log(LogLevel::Trace, false, false, message, &args);
	va_end(args);	
}

void Logger::tracePart(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	log(LogLevel::Trace, true, false, message, &args);
	va_end(args);	
}

void Logger::tracePartHexDump(void* buffer, uint32_t size)
{
	char hexValue[4];
	uint8_t* pointer = static_cast<uint8_t*>(buffer); 
	for (int i=0; i < size; i++)
	{
		snprintf(hexValue, sizeof(hexValue), "%2x ", pointer[i]);
		log(LogLevel::Trace, true, false, hexValue, nullptr);
	}
}

void Logger::traceEnd(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	log(LogLevel::Trace, true, true, message, &args);
	va_end(args);	
}

// Private

void Logger::log(LogLevel logLevel, bool isPart, bool writeLinefeed, const char* format, va_list* args)
{
	char buffer[128];

	if (!_isActive || _logLevel < logLevel)
	{
		return;
	}
	if (!isPart)
	{
		if (_includeTimestamp)
		{
			unsigned long timestamp = millis();			
			snprintf(buffer, sizeof(buffer), "%011u ", timestamp);
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
	vsnprintf(buffer, sizeof(buffer), format, *args);

	print(buffer);

	if (writeLinefeed)
	{
		print("\r\n");
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

Logger Log;
