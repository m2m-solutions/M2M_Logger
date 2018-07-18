#include <M2M_Logger.h>

// Change the following line to the serial port to use for logging.
#define serial SerialUSB

char* text = "1234567890";

void setup() {
  while (!serial) {}
  serial.begin(115200);
  
  Log.begin(&serial, LogLevel::Trace);
  Log.info("---Default settings-----------");
  WriteLogLines();

  Log.info("---With timestamp-------------");
  Log.setIncludeTimestamp(true);
  WriteLogLines();

  Log.info("---With timestamp and level---");
  Log.setIncludeLogLevel(true);   
  WriteLogLines();

  Log.info("---Log level Error------------");
  Log.setLogLevel(LogLevel::Error);
  WriteLogLines();

  Log.info("---Log level Info-------------");
  Log.setLogLevel(LogLevel::Info);
  WriteLogLines();

  Log.info("---Log level Debug------------");
  Log.setLogLevel(LogLevel::Debug);
  WriteLogLines();

  Log.info("---Log level Trace------------");
  Log.setLogLevel(LogLevel::Trace);
  WriteLogLines();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void WriteLogLines()
{
  Log.info("Test M2M_Logger");
  Log.error("Error message: '%s'", "string");
  Log.info("Info message %u", 1024);
  Log.debug("Debug message: %f", 31.23);
  Log.trace("Trace message: %t", false);
  
  Log.traceStart("Start trace %u", UINT32_MAX);
  Log.tracePart(" - Trace part - ");
  Log.traceEnd(" End of trace");
  
  Log.traceStart("Hex dump of text variable: 0x");
  Log.tracePartHexDump(text, 10);
  Log.traceEnd("");  
}
