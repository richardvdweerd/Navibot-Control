/*!

 Samsung Navibot Control

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
 *******************************************************************************/


void publishMessage(char* topic, int device, int state) {
  char myMsg[50];
  if (device) {
    sprintf(myMsg, "{ \"idx\" : %d, \"nvalue\" : %d}", device, state);
    client.publish(topic, myMsg);
  }
}

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  payload[length] = 0;

  SERIAL_PRINT("Message received: ");
  SERIAL_PRINTLN(topic);
  SERIAL_PRINT("payload: ");
  SERIAL_PRINTLN((char*)  payload);

  if (strstr((char*)payload, "\"idx\" : 806,") && strstr((char*)payload, "\"nvalue\" : 1,")) {
    SERIAL_PRINTLN("Navibot Start/Stop!!!!!");
    NavibotStartStop();
  }
  if (strstr((char*)payload, "\"idx\" : 834,") && strstr((char*)payload, "\"nvalue\" : 1,")) {
    SERIAL_PRINTLN("Navibot Recharge!!!!!");
    NavibotRecharge();
  }
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    SERIAL_PRINTLN("MQTT connecting ...");
    /* client ID */
    String clientId = "IRClient";
    /* connect now */
    if (client.connect(clientId.c_str())) {
       SERIAL_PRINTLN("connected");
      /* subscribe topic with default QoS 0*/
      client.subscribe(TOPIC_OUT);
    } else {
      SERIAL_PRINT("failed, status code =");
      SERIAL_PRINTLN(client.state());
      SERIAL_PRINTLN("try again in 5 seconds");
/* Wait 5 seconds before retrying */
//      delay(5000);
      /* leave function, try a next time */
      break;
    }
  }
}

/*******************************************************************************
 * 
 * SETUP MQTT
 * 
 *******************************************************************************/

void setupMqtt() {
    /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, mqtt_port);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);
}

/*******************************************************************************
 * 
 * SETUP OTA
 * 
 *******************************************************************************/
 
void setupOTA() {
  // OTA initialization
  ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      SERIAL_PRINTLN("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      SERIAL_PRINTLN("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      SERIAL_PRINTF("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      SERIAL_PRINTF("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) SERIAL_PRINTLN("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) SERIAL_PRINTLN("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) SERIAL_PRINTLN("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) SERIAL_PRINTLN("Receive Failed");
      else if (error == OTA_END_ERROR) SERIAL_PRINTLN("End Failed");
    });
  ArduinoOTA.setHostname("ESP8266_Navibot");
  ArduinoOTA.begin();
}

/*******************************************************************************
 * 
 * SETUP WIFI
 * 
 *******************************************************************************/
void setupWifi(){
  // attempt to connect to Wifi network:
  WiFi.begin((char*)ssid, (char*)password);
  while ( WiFi.status() != WL_CONNECTED ) {
    digitalWrite(5, HIGH);
    delay ( 250 );
    digitalWrite(5, LOW);
    delay( 250 );
    SERIAL_PRINT(".");
  }
  SERIAL_PRINTLN("");
  SERIAL_PRINTLN(WiFi.localIP());
}

/*******************************************************************************
 * 
 * SETUP
 * 
 *******************************************************************************/

void setup() {
  SERIAL_BEGIN(115200);

  // setup the used pins
  pinMode(PIN_STATUSLED, OUTPUT);
  pinMode(PIN_STARTSTOP, OUTPUT);
  pinMode(PIN_RECHARGE, OUTPUT);
  pinMode(PIN_INTERNALLED, OUTPUT);
  digitalWrite(PIN_STATUSLED, LOW);
  digitalWrite(PIN_STARTSTOP, LOW);
  digitalWrite(PIN_RECHARGE, LOW);
  digitalWrite(PIN_INTERNALLED, HIGH);

  // Wifi initialization
  setupWifi();

  // setup OTA
  setupOTA();

  // setup MQQT
  setupMqtt();
}

/*******************************************************************************
 * 
 * SwitchNavibot
 * 
 *******************************************************************************/

void SwitchNavibot(int port) {
  digitalWrite(PIN_STATUSLED, HIGH);   // switch on control led
  digitalWrite(port, HIGH);
  delay(1000);
  digitalWrite(port, LOW);
  digitalWrite(PIN_STATUSLED, LOW);    // switch off control led
}

/*******************************************************************************
 * 
 * flashStatusLED
 * 
 *******************************************************************************/

void flashStatusLED() {
    digitalWrite( PIN_STATUSLED, HIGH);
    delay(50);
    digitalWrite(PIN_STATUSLED, LOW);
    counter = 0;
}

/*******************************************************************************
 * 
 * LOOP
 * 
 *******************************************************************************/

void loop() {

  /* listen for OTA requests */
  ArduinoOTA.handle();

  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) 
    mqttconnect();

  /* this function will listen for incomming 
  subscribed topic-process-invoke receivedCallback */
  client.loop();

  // wait a while
  delay(250);

  /* flash status LED once in TIMER loops */
  if (counter++ >= TIMER) 
    flashStatusLED();
}

