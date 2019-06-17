#include <M2M_Logger.h>
#include <SD.h>

// Change the following line to the serial port to use for logging.
#define serial          SerialUSB
// Change the following line to the pin used as CE for the SD card module
#define PIN_SD_CE       3
// Change the following line 
#define LOG_FILENAME   "log.txt"

const char* text = "1234567890";
Logger Log;
File logFile;

void setup() {
  unit64_t timeout = millis() + 2000;
  while (!serial && timeout < millis()) {}
  serial.begin(115200);
  
  Log.begin(&serial, LogLevel::Trace);
  Log.info(F("Test M2M_Logger"));
  
  Log.info(F("Initializing SD card..."));
  if (!SD.begin(PIN_SD_CE)) 
  {
    Log.error(F("SD card initialization failed!"));
    while (1);
  }
  Log.info(F("Opening log.text file"));
  logFile = SD.open(LOG_FILENAME, FILE_WRITE);
  if (!logFile)
  {
    Log.error(F("Error opening log file %s"), LOG_FILENAME);
    while (1);
  }
  Log.setFileLogger(&logFile);
  Log.info(F("This is the first log line to be logged to the file %s"), LOG_FILENAME);
  
  Log.info(F("---Default settings-----------"));
  WriteLogLines();

  Log.info(F("---With timestamp-------------"));
  Log.setIncludeTimestamp(true);
  WriteLogLines();

  Log.info(F("---With timestamp and level---"));
  Log.setIncludeLogLevel(true);   
  WriteLogLines();

  Log.info(F("---Log level Error------------"));
  Log.setLogLevel(LogLevel::Error);
  WriteLogLines();

  Log.info(F("---Log level Info-------------"));
  Log.setLogLevel(LogLevel::Info);
  WriteLogLines();

  Log.info(F("---Log level Debug------------"));
  Log.setLogLevel(LogLevel::Debug);
  WriteLogLines();

  Log.info(F("---Log level Trace------------"));
  Log.setLogLevel(LogLevel::Trace);
  WriteLogLines();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void WriteLogLines()
{
  Log.info(F("Test M2M_Logger"));
  Log.error(F("Error message: '%s'"), "string");
  Log.info(F("Info message %u"), 1024);
  Log.debug(F("Debug message: %f"), 31.23);
  Log.trace(F("Trace message: %t"), false);
  
  Log.traceStart(F("Start trace %u"), UINT32_MAX);
  Log.tracePart(F(" - Trace part - "));
  Log.traceEnd(F(" End of trace"));
  
  Log.traceStart(F("Hex dump of text variable: 0x"));
  Log.tracePartHexDump(text, 10);
  Log.traceEnd(F(""));  
}
