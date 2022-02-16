#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h> 
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <RTClib.h>  
#include "Credentials.h"

const int LEDS_COUNT = 144;
const int LEDS_SIDE = 12;

int ch,cm,cs,cdy,cmo,cyr,cdw;          // current time & date variables
int om = -1;
int nh,nm,ns,ndy,nmo,nyr,ndw;          // NTP-based time & date variables

boolean updateDisplay = true;
float brightness = 30;
int red = 0, green = 255, blue = 255;
int baseTimezone = 0;   //local base timezone - UK = 0
boolean updateTimezone = false;

void setup() {
  Serial.begin(9600);
  
  setupLEDs();
  setupCayenne();
  setupNTP();
}

void loop() {
  loopCayenne();
  loopNTP();
  
  if (updateDisplay) {
    displayTime(ch, cm, cs);
    updateDisplay = false;
  }

  delay(1000);
}

CAYENNE_IN_DEFAULT() {

  spln("into Cayenne in default");

  if (request.channel == 0) {
    red = getValue.asInt();
    updateDisplay = true;
   
    
  } else if (request.channel == 1) {
    green = getValue.asInt();
    updateDisplay = true;
     
  } else if (request.channel == 2) {
    blue = getValue.asInt();
    updateDisplay = true;
    
     
  } else if (request.channel == 3) {
    brightness = getValue.asInt();
    updateDisplay = true;
         
  } else if (request.channel == 4) {
    baseTimezone = getValue.asInt();
    updateTimezone = true;
    updateDisplay = true;
    
  }
}

CAYENNE_OUT_DEFAULT()
{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  // Cayenne.virtualWrite(0, millis());
  // Some examples of other functions you can use to send data.
  //Cayenne.celsiusWrite(1, 22.0);
  //Cayenne.luxWrite(2, 700);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

