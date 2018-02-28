# Navibot Control


Take control over your Samsung Navibot with this sketch for an ESP8266.
Uses MQTT client from knolleary (https://github.com/knolleary/pubsubclient) 

### Why?
Ever wanted to control your Samsung Navibot via Domoticz? This is a first step in
that direction. In the Navibot is enough place to put a ESP8266 in one of the cavities.
By controlling the tactile switches you get control over the Navibot.
The tactile switches are controlled by 2N2222 transistors. The transistors in turn are 
controlled by the ESP8266.

In this version only two switches are controlled: start/stop and recharge. That seems to be
enough to take basic control over the Navibot.

### How it works
Domoticz senses our absense from the house and issues a start/stop command via a virtual switch. The
Navibot reads the MQTT command and shorts the tactile switch through the transistor. The Navibot
starts. 
When Domoticz senses that somebody came home, it issues a start/stop command to stop the Navibot and 
after that it issues a recharge command via another virtual switch. Both commands are 
detected by the Navibot and the two switches are shorted one after the other. The Navibot returns
to it's station.


### ToDo
- [ ] Adding schematics here
- [ ] Upgrade sketch and hardware to detect movement and 'report' back to Domoticz



