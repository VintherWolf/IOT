/*
* Project 01_helloworld_blinky
* Description: Modified fork of blinky tutorial program
* Author: Daniel K. Vinther Wolf
* Date: 01-09-2020
*
* Description: API https://console.particle.io/devices 
*              Functions: "toggleBlueLed"   valid: "on" or "off"
*                         "toggleWhiteLed"  valid: "on" or "off"
*/


/*  Pre-setup configuration */
const int BLUE_LED = D7;
const int WHITE_LED = D2;
const int PHOTO_SENSOR = A0;

// Reads from PHOTO_SENSOR:
int analog_value;

// blue LED return value
int blueLED = LOW;

void eventHandler(const char *event, const char *data);

enum State { READY, RUNNING };
State state = READY;

// setup() runs once, when the device is first turned on.
void setup() {
    
    pinMode(BLUE_LED, OUTPUT);
    pinMode(WHITE_LED, OUTPUT);
    pinMode(PHOTO_SENSOR, INPUT);

    // Cloud Subscriptions
    Particle.subscribe("brightlight", eventHandler);

    // Cloud Functions
    Particle.function("toggleBlueLed", blueLedToggle);
    Particle.function("toggleWhiteLed", whiteLedToggle);

    // Cloud Variables
    Particle.variable("analog_value", &analog_value, INT);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

   // Get analog value from Photo sensor
   analog_value = analogRead(PHOTO_SENSOR);

   if (analog_value > 50 && state == READY) {
       digitalWrite(BLUE_LED, LOW);
       state = RUNNING;
       Particle.publish("brightlight","seen", PUBLIC);
   }
   delay(100);
}

int blueLedToggle(String command) {
    
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

int whiteLedToggle(String command) {
    
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

void eventHandler(const char *event, const char *data) {

    if (strcmp(data, "seen") == 0) {
        digitalWrite(WHITE_LED, LOW);
        digitalWrite(BLUE_LED, HIGH);
        state = READY;
    }

    else {
        // 
    }


}