#ifndef Flasher_h
#define Flasher_h

#include "config.h"
#include <Arduino.h>


/**
 * Flashes or permanent turns on/off the LED connected to a pin
 */
class Flasher {
    public:
        Flasher(int ledPin, long OnTime, long OffTime);
        Flasher() {};
        void tick();
        void flashingOn();
        void permanentOn();
        void off();
        bool isFlashing();
        bool isPermanentOn();
        void flashTimes(int howManyTimes);
        void setStatus(int statusIndex);

    private:
        int ledPin; // the number of the LED pin
        unsigned long OnTime; // milliseconds of on-time
        unsigned long OffTime; // milliseconds of off-time
        int ledState; // ledState used to set the LED
        unsigned long previousMillis; // will store last time LED was updated
        bool updateFlshr = false; // flashing on/off
        bool isOn = false; // global on/off
        int timesFlash = 0; // how many times it can flash

        void setInterval(long OnTime, long OffTime);
};

#endif