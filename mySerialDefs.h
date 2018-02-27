#ifndef MYSERIALDEFS_H
#define MYSERIALDEFS_H
 
//#define SERIAL_DEBUG     // comment out to remove all print statements
 
#ifdef SERIAL_DEBUG
  #define SERIAL_BEGIN(x)      Serial.begin(x)
  #define SERIAL_PRINT(x)      Serial.print(x)
  #define SERIAL_PRINTLN(x)    Serial.println(x)
  #define SERIAL_END()         Serial.end()
  #define SERIAL_PRINT2(x,y)   Serial.print(x,y)
  #define SERIAL_PRINTLN2(x,y) Serial.println(x,y)
  #define SERIAL_PRINTF(x,y)   Serial.printf(x,y)
#else 
  #define SERIAL_BEGIN(x)
  #define SERIAL_PRINT(x)
  #define SERIAL_PRINTLN(x)
  #define SERIAL_END()
  #define SERIAL_PRINT2(x,y)
  #define SERIAL_PRINTLN2(x,y)
  #define SERIAL_PRINTF(x,y)
#endif
 
#endif
