/*
* File: leds.cpp
* Project 01_helloworld_blinky
* Description: Modified fork of blinky tutorial program
* Author: Daniel K. Vinther Wolf
* Date: 10-09-2020
*
* Usage: 
*/
#include "Particle.h"
#include "leds.h"
#include "pins.h"

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