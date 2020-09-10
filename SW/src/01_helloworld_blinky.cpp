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
/*  Pre-setup configuration */
const int BLUE_LED = D7;
const int WHITE_LED = D2;
const int PHOTO_SENSOR = A0;

// Reads from PHOTO_SENSOR:
int analog_value; 

// blue LED return value
int blueLED = LOW;

// Local event prototype
void eventHandler(const char *event, const char *data);

// Buddy event prototype.. Subscribe to Jan's Event
void jinxedEventHandler(const char *event, const char *data);
long int i_data = 0;

void brightnesstHandler(const char *event, const char *data);

// States to avoid publish bursts (ensure "singleshot" publish)
enum State { READY, RUNNING };
State state = READY;

// Check if publish was successful
bool success = false;
bool thingsspeak = false;

// Install loghandler with baudrate 115200
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
    
    pinMode(BLUE_LED, OUTPUT);
    pinMode(WHITE_LED, OUTPUT);
    pinMode(PHOTO_SENSOR, INPUT);
    Log.info("Particle setting up");

    // Cloud Subscriptions
    Particle.subscribe("brightlight_from_dkwv", eventHandler, ALL_DEVICES);

    // Particle.subscribe("Jinxed sensor interrupted", jinxedEventHandler, "e00fce6864ea86ea7bb98251");
    Particle.subscribe("Jinxed_sensor_interrupted", jinxedEventHandler, ALL_DEVICES);
    
    Particle.subscribe("brightness adjusted", brightnesstHandler, ALL_DEVICES);

    // Cloud Functions
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
    Extra Functions:
    ---------------------------------------
1   setBlueLed
2   setWhiteLed
3   eventHandler        subscription ALL
4   jinxedEventHandler  subscription ALL
5   brightnesstHandler  subscription ALL
===================================================================
 */


// Set on-board Blue LED "on" or "off"
int setBlueLed(String command) {
    
    if (command == "on") {
        digitalWrite(BLUE_LED, HIGH);
        return 1;
    }
    else if (command == "off") {
        digitalWrite(BLUE_LED, LOW);
        return 0;
    }
    else {
        return -1;
    }
}


// Set external white LED "on" or "off" (Ref des: D1)
// Schematic: https://github.com/VintherWolf/IOT/tree/master/HW/Schematic/PhotoSensor
int setWhiteLed(String command) {
    
    if (command == "on") {
        digitalWrite(WHITE_LED, HIGH);
        return 1;
    }
    else if (command == "off") {
        digitalWrite(WHITE_LED, LOW);
        return 0;
    }
    else {
        return -1;
    }
}

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
