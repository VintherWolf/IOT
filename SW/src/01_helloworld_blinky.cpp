/*
* Project 01_helloworld_blinky
* Description: Modified fork of blinky tutorial program
* Author: Daniel K. Vinther Wolf
* Date: 07-09-2020
*
* Usage: go to API @ https://console.particle.io/devices 
*              
*/

#include "Particle.h"
#include "leds.h"
#include "pins.h"
/*  Pre-setup configuration */

// Store value from PHOTO_SENSOR:
int analog_value; 

// blue LED return value
int blueLED = LOW;

// Subscriptions Prototypes and Associated Variables
void eventHandler(const char *event, const char *data);

void jinxedEventHandler(const char *event, const char *data);
long int i_data = 0;

void brightnesstHandler(const char *event, const char *data);

// States to avoid publish bursts (ensure "singleshot" publish)
enum State { READY, RUNNING };
State state = READY;

// Boolean vars to check if a publish was successful
bool success = false;
bool thingsspeak = false;

// Install loghandler with default baudrate 115200
SerialLogHandler logToUsb;
int setBlueLed(String command);
int setWhiteLed(String command);

/*
===================================================================
    Setup Constructor:
    setup() runs once, when the device is first turned on.
===================================================================
 */
void setup() {
    // Argon Pinout setup 
    pinMode(BLUE_LED, OUTPUT);
    pinMode(WHITE_LED, OUTPUT);
    pinMode(PHOTO_SENSOR, INPUT);
    Log.info("Particle setting up");

    // Cloud Subscriptions
    Particle.subscribe("brightlight_from_dkwv", eventHandler, ALL_DEVICES);
    Particle.subscribe("Jinxed_sensor_interrupted", jinxedEventHandler, ALL_DEVICES);
    Particle.subscribe("brightness adjusted", brightnesstHandler, ALL_DEVICES);
    
    // Cloud Functions
    // From "leds.h":
    Particle.function("setBlueLed", setBlueLed);
    Particle.function("setWhiteLed", setWhiteLed);

    // Cloud Variables
    Particle.variable("analog_value", &analog_value, INT);
}
/*
===================================================================
    MAIN:
    loop() runs over and over again, as quickly as it can execute.
===================================================================
 */
void loop() {
    // Connect to Particle Cloud, Check
    if (Particle.connected() == false) {
        Particle.connect();
    }

   // Get analog value from Photo sensor
   analog_value = analogRead(PHOTO_SENSOR);

   thingsspeak = Particle.publish("Brightness",  String(analog_value), PRIVATE);

   if (thingsspeak == true)
   {
       Log.info("Succssfully published brightness to Thingsspeak");
       thingsspeak = false;
   }
   else {
       Log.info("Failed to publish brightness to Thingsspeak");
   }

   if (analog_value > 50 && state == READY) {
       digitalWrite(BLUE_LED, LOW);
       state = RUNNING;
       success = Particle.publish("brightlight_from_dkwv","seen", PUBLIC);
   }
   // Casual delay , may be omitted
   delay(10000);
}

/*
===================================================================
    Subscriptions (max 4):
    ---------------------------------------
    Name                Scope
1   eventHandler        ALL_DEVICES
2   jinxedEventHandler  ALL_DEVICES
3   brightnesstHandler  ALL_DEVICES
4   -
===================================================================
 */

// Local event that toggles external White LED off
void eventHandler(const char *event, const char *data) {
    Log.info("Received %s %s", event, (data ? data : "NULL"));
    if (strcmp(data, "seen") == 0) {
        digitalWrite(WHITE_LED, LOW);
        digitalWrite(BLUE_LED, HIGH);
        state = READY;
    }

    else {
        // Do nothing
    }
}

// buddy event that should turn blue LED on if large int is received
void jinxedEventHandler(const char *event, const char *data) {
    Log.info("Received %s %s", event, (data ? data : "NULL"));

    i_data = strtol(data, NULL, 10);

    if (i_data > 250) {
        digitalWrite(WHITE_LED, LOW);
        digitalWrite(BLUE_LED, HIGH);
        state = READY;
    }
    else {
        digitalWrite(WHITE_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
        state = READY;
    }
}

void brightnesstHandler(const char *event, const char *data) {
    Log.info("Received %s %s", event, (data ? data : "NULL"));

    i_data = strtol(data, NULL, 10);

    if (i_data > 10) {
        digitalWrite(WHITE_LED, LOW);
        digitalWrite(BLUE_LED, HIGH);
        state = READY;
    }
    else {
        digitalWrite(WHITE_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
        state = READY;
    }
}
