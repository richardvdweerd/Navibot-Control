/*!
 
 Samsung Navibot
 
 *  \brief     Samsung Navibot ESP8266 MQTT Domoticz
 *  \details   Little app control your Samsung Navibot via MQTT and Domoticz
 *  \author    Richard van der Weerd
 *  \version   0.1
 *  \date      2018-02-27
 *  \pre       First initialize the system.
 *  \bug       No bugs discovered yet
 *  \warning   No warnings yet
 *  \copyright MIT License.
 *  
 *  
 *  
 */
 
 
//#define SERIAL_DEBUG     // comment out in mySerfialDefs.h to remove all print statements
 
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "PubSubClient.h"           // mqtt client
#include "mySerialDefs.h"
#include <Credentials.h>
#include "pins.h"
 
#ifdef CREDENTIALS_H
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
#else
const char* ssid = "your ssid";
const char* password = "your password";
const char* mqtt_server = "mqtt server ip";
const int mqtt_port = 1883;
#endif
 
/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);
 
/* topics */
#define TOPIC_OUT    "domoticz/out"
#define TOPIC_IN     "domoticz/in"
char msg[MQTT_MAX_PACKET_SIZE];
 
#define NavibotStartStop() SwitchNavibot(PIN_STARTSTOP)
#define NavibotRecharge()  SwitchNavibot(PIN_RECHARGE)
 
#define TIMER 20   // flash every TIMER loops
int counter = 0;
 
/*******************************************************************************
 * 
 * MQTT functions
 * 
2 file changes in working directory
View changes
commit:8ed877
WIP on master